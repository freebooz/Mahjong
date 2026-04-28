//==============================================================================
// 麻将游戏 - 玩家账户服务实现
// PlayerAccountService.cpp
//
// 负责玩家登录、注册、会话管理等账户相关功能
// 支持：账号密码登录、微信登录、访客登录、会话持久化
//==============================================================================

#include "Account/PlayerAccountService.h"
#include "Kismet/GameplayStatics.h"

//==============================================================================
// 构造函数
//==============================================================================
UPlayerAccountService::UPlayerAccountService()
{
    CurrentUsername = TEXT("");
    bIsAuthenticated = false;
    bIsGuest = false;
    PlayerLevel = 1;
    Coins = 1000;
    SessionService = nullptr;
    WeChatAuthService = nullptr;
}

//==============================================================================
// 初始化服务
//==============================================================================
void UPlayerAccountService::InitializeService(const FString& ServerURL, const FString& WeChatAppID, const FString& WeChatAppSecret)
{
    // 初始化会话服务
    SessionService = NewObject<USessionService>();
    SessionService->Initialize(ServerURL);

    // 初始化微信认证服务
    WeChatAuthService = NewObject<UWeChatAuthService>();
    WeChatAuthService->Initialize(WeChatAppID, WeChatAppSecret);

    UE_LOG(LogTemp, Log, TEXT("[Account] 账户服务初始化完成"));
}

//==============================================================================
// 账号密码登录
//==============================================================================
void UPlayerAccountService::Login(const FString& Username, const FString& Password)
{
    if (Username.IsEmpty() || Password.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Account] 登录失败：用户名或密码为空"));
        bIsAuthenticated = false;
        return;
    }

    // 保存登录信息
    CurrentUsername = Username;
    bIsAuthenticated = true;
    bIsGuest = false;

    UE_LOG(LogTemp, Log, TEXT("[Account] 用户 %s 登录成功"), *CurrentUsername);
}

//==============================================================================
// 微信登录
// 流程：
// 1. 生成授权 URL -> 用户扫码授权 -> 获取授权码
// 2. 使用授权码完成登录 -> 后端验证 -> 返回会话
//==============================================================================
void UPlayerAccountService::LoginWithWeChat(FLoginCallback Callback)
{
    if (!WeChatAuthService)
    {
        FLoginResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("微信认证服务未初始化");
        Callback.ExecuteIfBound(Result);
        return;
    }

    if (!SessionService)
    {
        FLoginResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("会话服务未初始化");
        Callback.ExecuteIfBound(Result);
        return;
    }

    // 保存回调
    PendingLoginCallback = Callback;

    // 生成授权 URL 并打开浏览器
    FString State = FGuid::NewGuid().ToString();
    FString BackendCallback = SessionService->GetCurrentSession().SessionToken.IsEmpty()
        ? TEXT("http://localhost:8080/api/auth/wechat/callback")
        : TEXT("http://localhost:8080/api/auth/wechat/callback");

    FString AuthURL = WeChatAuthService->GenerateAuthURL(BackendCallback, State);

    if (AuthURL.IsEmpty())
    {
        FLoginResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("生成微信授权URL失败");
        PendingLoginCallback.ExecuteIfBound(Result);
        PendingLoginCallback.Clear();
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Account] 正在打开微信授权页面..."));

    // 在编辑器环境下打开浏览器
#if WITH_EDITOR
    FPlatformProcess::LaunchURL(*AuthURL);
#endif

    // 提示用户授权后输入授权码
    // 实际项目中，授权码通常由后端通过 WebSocket 或轮询获取
    UE_LOG(LogTemp, Log, TEXT("[Account] 请在微信授权后，输入返回的授权码"));

    // 创建一个模拟的授权码流程用于演示
    // 实际项目中，授权码应该由后端自动处理
    FString DemoAuthCode = TEXT("DEMO_AUTH_CODE_") + FGuid::NewGuid().ToString().Left(8);

    // 使用授权码完成登录
    WeChatAuthService->CompleteLoginWithAuthCode(DemoAuthCode,
        FWeChatAuthCallback::CreateUObject(this, &UPlayerAccountService::OnWeChatAuthComplete));
}

//==============================================================================
// Token 登录
// 使用保存的会话 Token 恢复登录状态
//==============================================================================
void UPlayerAccountService::LoginWithToken(FLoginCallback Callback)
{
    if (!SessionService)
    {
        FLoginResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("会话服务未初始化");
        Callback.ExecuteIfBound(Result);
        return;
    }

    // 保存回调
    PendingLoginCallback = Callback;

    // 尝试从磁盘加载会话
    if (SessionService->LoadSessionFromDisk())
    {
        FUserSession Session = SessionService->GetCurrentSession();

        if (SessionService->IsSessionValid())
        {
            UE_LOG(LogTemp, Log, TEXT("[Account] 会话有效，正在验证..."));

            // 在服务器上验证会话
            SessionService->ValidateSessionOnServer(Session.SessionToken,
                FSessionLoginCallback::CreateUObject(this, &UPlayerAccountService::OnSessionLoginComplete));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[Account] 会话已过期"));
            FLoginResult Result;
            Result.bSuccess = false;
            Result.ErrorMessage = TEXT("会话已过期，请重新登录");
            PendingLoginCallback.ExecuteIfBound(Result);
            PendingLoginCallback.Clear();
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Account] 无保存的会话"));
        FLoginResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("无保存的会话");
        Callback.ExecuteIfBound(Result);
    }
}

//==============================================================================
// 登出
//==============================================================================
void UPlayerAccountService::Logout()
{
    UE_LOG(LogTemp, Log, TEXT("[Account] 用户 %s 登出"), *CurrentUsername);

    CurrentUsername = TEXT("");
    bIsAuthenticated = false;
    bIsGuest = false;
    PlayerLevel = 1;

    if (SessionService)
    {
        SessionService->ClearSession();
        SessionService->SaveSessionToDisk();
    }
}

//==============================================================================
// 注册
//==============================================================================
void UPlayerAccountService::Register(const FString& Username, const FString& Password, const FString& Email)
{
    if (Username.IsEmpty() || Password.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Account] 注册失败：用户名或密码为空"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Account] 用户 %s 注册成功（邮箱：%s）"), *Username, *Email);

    // 保存登录信息
    CurrentUsername = Username;
    bIsAuthenticated = true;
    bIsGuest = false;
}

//==============================================================================
// 尝试重连
//==============================================================================
void UPlayerAccountService::TryReconnect(FLoginCallback Callback)
{
    UE_LOG(LogTemp, Log, TEXT("[Account] 尝试恢复连接..."));
    LoginWithToken(Callback);
}

//==============================================================================
// 访客登录
// 为临时玩家创建一个匿名会话
//==============================================================================
void UPlayerAccountService::LoginAsGuest(FLoginCallback Callback)
{
    if (!SessionService)
    {
        FLoginResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("会话服务未初始化");
        Callback.ExecuteIfBound(Result);
        return;
    }

    // 保存回调
    PendingLoginCallback = Callback;

    UE_LOG(LogTemp, Log, TEXT("[Account] 正在创建访客会话..."));

    // 使用会话服务的访客登录功能
    SessionService->LoginAsGuest(
        FSessionLoginCallback::CreateUObject(this, &UPlayerAccountService::OnSessionLoginComplete));
}

//==============================================================================
// 微信认证完成回调
//==============================================================================
void UPlayerAccountService::OnWeChatAuthComplete(const FWeChatAuthResult& Result)
{
    FLoginResult LoginResult;

    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[Account] 微信认证成功，OpenID: %s"), *Result.OpenID);

        // 保存会话
        FSessionInfo SessionInfo;
        SessionInfo.SessionToken = Result.AccessToken;
        SessionInfo.RefreshToken = Result.RefreshToken;
        SessionInfo.OpenID = Result.OpenID;
        SessionInfo.ExpiresAt = FDateTime::Now().ToUnixTimestamp() + Result.ExpiresIn;
        SessionInfo.bIsValid = true;

        SessionService->SaveSession(SessionInfo);

        // 创建用户会话
        FUserSession UserSession;
        UserSession.SessionToken = Result.AccessToken;
        UserSession.OpenID = Result.OpenID;
        UserSession.Nickname = TEXT("微信用户_") + Result.OpenID.Left(6);
        UserSession.PlayerLevel = 1;
        UserSession.bIsGuest = false;

        SessionService->UpdateSession(UserSession);
        SessionService->SaveSessionToDisk();

        // 更新状态
        CurrentUsername = UserSession.Nickname;
        bIsAuthenticated = true;
        bIsGuest = false;
        PlayerLevel = UserSession.PlayerLevel;

        LoginResult.bSuccess = true;
        LoginResult.Session = UserSession;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Account] 微信认证失败：%s"), *Result.ErrorMessage);
        LoginResult.bSuccess = false;
        LoginResult.ErrorMessage = Result.ErrorMessage;
    }

    PendingLoginCallback.ExecuteIfBound(LoginResult);
    PendingLoginCallback.Clear();
}

//==============================================================================
// 会话登录完成回调
//==============================================================================
void UPlayerAccountService::OnSessionLoginComplete(const FUserSession& Session)
{
    FLoginResult LoginResult;

    if (!Session.SessionToken.IsEmpty())
    {
        CurrentUsername = Session.Nickname;
        PlayerLevel = Session.PlayerLevel;
        bIsAuthenticated = true;
        bIsGuest = Session.bIsGuest;

        SessionService->UpdateSession(Session);
        SessionService->SaveSessionToDisk();

        UE_LOG(LogTemp, Log, TEXT("[Account] 登录成功：%s (等级: %d)"), *CurrentUsername, PlayerLevel);

        LoginResult.bSuccess = true;
        LoginResult.Session = Session;
    }
    else
    {
        LoginResult.bSuccess = false;
        LoginResult.ErrorMessage = TEXT("会话验证失败");
        UE_LOG(LogTemp, Warning, TEXT("[Account] 会话验证失败"));
    }

    PendingLoginCallback.ExecuteIfBound(LoginResult);
    PendingLoginCallback.Clear();
}

//==============================================================================
// 重连完成回调
//==============================================================================
void UPlayerAccountService::OnReconnectComplete(const FLoginResult& Result)
{
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[Account] 重连成功"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Account] 重连失败：%s"), *Result.ErrorMessage);
    }

    PendingLoginCallback.ExecuteIfBound(Result);
}

//==============================================================================
// 获取会话 Token
//==============================================================================
FString UPlayerAccountService::GetSessionToken() const
{
    if (SessionService)
    {
        return SessionService->GetCurrentSession().SessionToken;
    }
    return TEXT("");
}

//==============================================================================
// 获取当前用户会话
//==============================================================================
FUserSession UPlayerAccountService::GetCurrentUserSession() const
{
    if (SessionService)
    {
        return SessionService->GetCurrentSession();
    }
    FUserSession EmptySession;
    return EmptySession;
}

//==============================================================================
// 添加金币
//==============================================================================
void UPlayerAccountService::AddCoins(int32 Amount)
{
    if (Amount > 0)
    {
        Coins += Amount;
        UE_LOG(LogTemp, Log, TEXT("[Account] 添加 %d 金币，当前总数: %d"), Amount, Coins);
    }
}

//==============================================================================
// 花费金币
//==============================================================================
bool UPlayerAccountService::SpendCoins(int32 Amount)
{
    if (Amount > 0 && Coins >= Amount)
    {
        Coins -= Amount;
        UE_LOG(LogTemp, Log, TEXT("[Account] 花费 %d 金币，剩余: %d"), Amount, Coins);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("[Account] 金币不足：需要 %d，现有 %d"), Amount, Coins);
    return false;
}