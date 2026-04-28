//==============================================================================
// 麻将游戏 - 微信认证服务实现
// WeChatAuthService.cpp
//
// 微信OAuth2.0 流程说明：
// 1. 生成授权URL -> 用户在浏览器扫码授权 -> 获取授权码
// 2. 发送授权码到后端 -> 后端换取令牌 -> 返回游戏会话
//==============================================================================

#include "Account/WeChatAuthService.h"
#include "Kismet/GameplayStatics.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "PlatformProcess.h"

//==============================================================================
// 静态成员定义
//==============================================================================
const TCHAR* UWeChatAuthService::WeChatAuthURLTemplate = TEXT(
    "https://open.weixin.qq.com/connect/qrconnect?"
    "appid=%s&"
    "redirect_uri=%s&"
    "response_type=code&"
    "scope=snsapi_login&"
    "state=%s#wechat_redirect"
);

const TCHAR* UWeChatAuthService::WeChatQRConnectURLTemplate = TEXT(
    "https://open.weixin.qq.com/connect/qrconnect?"
    "appid=%s&"
    "redirect_uri=%s&"
    "response_type=code&"
    "scope=snsapi_login&"
    "state=%s"
);

//==============================================================================
// 构造函数
//==============================================================================
UWeChatAuthService::UWeChatAuthService()
{
    bInitialized = false;
    BackendAPIURL = TEXT("http://localhost:8080/api/auth");
}

//==============================================================================
// 初始化认证服务
//==============================================================================
void UWeChatAuthService::Initialize(const FString& AppID, const FString& AppSecret)
{
    WeChatAppID = AppID;
    WeChatAppSecret = AppSecret;
    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("[WeChat] 认证服务初始化完成，AppID: %s"), *WeChatAppID);
}

//==============================================================================
// 生成微信授权URL
//==============================================================================
FString UWeChatAuthService::GenerateAuthURL(const FString& RedirectURI, const FString& State)
{
    if (!bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("[WeChat] 认证服务未初始化"));
        return FString();
    }

    // URL编码RedirectURI（简化实现，生产环境应使用完整URL编码）
    FString EncodedRedirectURI = RedirectURI;
    EncodedRedirectURI = EncodedRedirectURI.Replace(TEXT(":"), TEXT("%3A"));
    EncodedRedirectURI = EncodedRedirectURI.Replace(TEXT("/"), TEXT("%2F"));
    EncodedRedirectURI = EncodedRedirectURI.Replace(TEXT("?"), TEXT("%3F"));
    EncodedRedirectURI = EncodedRedirectURI.Replace(TEXT("="), TEXT("%3D"));
    EncodedRedirectURI = EncodedRedirectURI.Replace(TEXT("&"), TEXT("%26"));

    // 生成授权URL
    FString AuthURL = FString::Format(WeChatAuthURLTemplate, {
        *WeChatAppID,
        *EncodedRedirectURI,
        *State
    });

    UE_LOG(LogTemp, Log, TEXT("[WeChat] 生成授权URL: %s"), *AuthURL);

    return AuthURL;
}

//==============================================================================
// 使用微信账号登录
//==============================================================================
void UWeChatAuthService::LoginWithWeChat(FWeChatAuthCallback Callback)
{
    if (!bInitialized)
    {
        FWeChatAuthResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("微信认证服务未初始化，请先调用 Initialize()");
        Callback.ExecuteIfBound(Result);
        UE_LOG(LogTemp, Error, TEXT("[WeChat] 登录失败：服务未初始化"));
        return;
    }

    // 构建授权回调地址
    FString BackendCallback = BackendAPIURL + TEXT("/wechat/callback");

    // 生成随机state防止CSRF
    FString State = FGuid::NewGuid().ToString();

    // 生成授权URL
    FString AuthURL = GenerateAuthURL(BackendCallback, State);

    if (AuthURL.IsEmpty())
    {
        FWeChatAuthResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("生成授权URL失败");
        Callback.ExecuteIfBound(Result);
        return;
    }

    // 在编辑器环境下打开浏览器
#if WITH_EDITOR
    UE_LOG(LogTemp, Log, TEXT("[WeChat] 正在打开微信授权页面..."));
    FPlatformProcess::LaunchURL(*AuthURL);
#endif

    UE_LOG(LogTemp, Log, TEXT("[WeChat] 授权URL已生成，请使用 CompleteLoginWithAuthCode() 完成登录"));
}

//==============================================================================
// 使用授权码完成登录
//==============================================================================
void UWeChatAuthService::CompleteLoginWithAuthCode(const FString& AuthCode, FWeChatAuthCallback Callback)
{
    if (!bInitialized)
    {
        FWeChatAuthResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("微信认证服务未初始化");
        Callback.ExecuteIfBound(Result);
        return;
    }

    if (AuthCode.IsEmpty())
    {
        FWeChatAuthResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("授权码为空");
        Callback.ExecuteIfBound(Result);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[WeChat] 收到授权码，正在换取访问令牌..."));

    // 构建请求
    FString RequestURL = BackendAPIURL + TEXT("/wechat/complete-login");
    FString RequestBody = FString::Printf(
        TEXT("{\"auth_code\":\"%s\",\"app_id\":\"%s\"}"),
        *AuthCode,
        *WeChatAppID
    );

    // 创建HTTP请求
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(RequestURL);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(RequestBody);

    // 使用弱引用避免循环引用
    TWeakObjectPtr<UWeChatAuthService> WeakThis(this);

    // 绑定完成回调
    Request->OnProcessRequestComplete().BindLambda([
        WeakThis, Callback](TSharedPtr<IHttpRequest> Req, TSharedPtr<IHttpResponse> Resp, bool bSuccess)
        {
            if (!WeakThis.IsValid())
            {
                return;
            }

            UWeChatAuthService* This = WeakThis.Get();
            FWeChatAuthResult Result;

            if (!bSuccess || !Resp.IsValid())
            {
                Result.bSuccess = false;
                Result.ErrorMessage = TEXT("网络请求失败");
                UE_LOG(LogTemp, Error, TEXT("[WeChat] 登录请求失败：网络错误"));
            }
            else
            {
                int32 ResponseCode = Resp->GetResponseCode();
                FString ResponseContent = Resp->GetContentAsString();

                UE_LOG(LogTemp, Log, TEXT("[WeChat] 后端响应码: %d"), ResponseCode);

                if (ResponseCode == 200)
                {
                    // 解析响应
                    This->ParseSessionResponse(ResponseContent, Callback);
                    return;
                }
                else
                {
                    Result.bSuccess = false;
                    Result.ErrorMessage = FString::Printf(TEXT("后端返回错误码: %d"), ResponseCode);
                }
            }

            if (Callback.IsBound())
            {
                Callback.ExecuteIfBound(Result);
            }
        });

    Request->ProcessRequest();

    UE_LOG(LogTemp, Log, TEXT("[WeChat] 已向后端发送登录请求: %s"), *RequestURL);
}

//==============================================================================
// 解析后端会话响应
//==============================================================================
void UWeChatAuthService::ParseSessionResponse(const FString& ResponseContent, FWeChatAuthCallback& Callback)
{
    FWeChatAuthResult Result;

    if (ResponseContent.IsEmpty())
    {
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("后端返回空响应");
        Callback.ExecuteIfBound(Result);
        return;
    }

    // 简单JSON解析（生产环境应使用 FJsonObject）
    // 检查成功标志
    if (ResponseContent.Contains(TEXT("\"success\":true")) ||
        ResponseContent.Contains(TEXT("\"session_token\":")))
    {
        FString SessionToken;
        FString RefreshToken;
        FString OpenID;
        int32 ExpiresIn = 7200;

        // 解析会话令牌
        if (ResponseContent.Contains(TEXT("\"session_token\":\"")))
        {
            int32 TokenStart = ResponseContent.Find(TEXT("\"session_token\":\"")) + 16;
            int32 TokenEnd = ResponseContent.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, TokenStart);
            if (TokenStart > 16 && TokenEnd > TokenStart)
            {
                SessionToken = ResponseContent.Mid(TokenStart, TokenEnd - TokenStart);
            }
        }

        // 解析刷新令牌
        if (ResponseContent.Contains(TEXT("\"refresh_token\":\"")))
        {
            int32 TokenStart = ResponseContent.Find(TEXT("\"refresh_token\":\"")) + 15;
            int32 TokenEnd = ResponseContent.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, TokenStart);
            if (TokenStart > 15 && TokenEnd > TokenStart)
            {
                RefreshToken = ResponseContent.Mid(TokenStart, TokenEnd - TokenStart);
            }
        }

        // 解析OpenID
        if (ResponseContent.Contains(TEXT("\"openid\":\"")))
        {
            int32 IDStart = ResponseContent.Find(TEXT("\"openid\":\"")) + 10;
            int32 IDEnd = ResponseContent.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, IDStart);
            if (IDStart > 10 && IDEnd > IDStart)
            {
                OpenID = ResponseContent.Mid(IDStart, IDEnd - IDStart);
            }
        }

        if (!SessionToken.IsEmpty())
        {
            Result.bSuccess = true;
            Result.AccessToken = SessionToken;
            Result.RefreshToken = RefreshToken;
            Result.OpenID = OpenID;
            Result.ExpiresIn = ExpiresIn;

            // 保存会话
            CurrentSession.SessionToken = SessionToken;
            CurrentSession.RefreshToken = RefreshToken;
            CurrentSession.OpenID = OpenID;
            CurrentSession.ExpiresAt = FDateTime::Now().ToUnixTimestamp() + ExpiresIn;
            CurrentSession.bIsValid = true;

            UE_LOG(LogTemp, Log, TEXT("[WeChat] 登录成功，OpenID: %s"), *OpenID);
        }
        else
        {
            Result.bSuccess = false;
            Result.ErrorMessage = TEXT("无法解析会话令牌");
        }
    }
    else
    {
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("登录失败：") + ResponseContent;
    }

    if (Callback.IsBound())
    {
        Callback.ExecuteIfBound(Result);
    }
}

//==============================================================================
// 刷新会话
//==============================================================================
void UWeChatAuthService::RefreshSession(FWeChatTokenCallback Callback)
{
    if (CurrentSession.RefreshToken.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[WeChat] 无刷新令牌可用"));
        Callback.ExecuteIfBound(TEXT(""));
        return;
    }

    // 构建刷新请求
    FString RequestURL = BackendAPIURL + TEXT("/auth/refresh");
    FString RequestBody = FString::Printf(
        TEXT("{\"refresh_token\":\"%s\"}"),
        *CurrentSession.RefreshToken
    );

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(RequestURL);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(RequestBody);

    // 使用弱引用
    TWeakObjectPtr<UWeChatAuthService> WeakThis(this);

    Request->OnProcessRequestComplete().BindLambda([
        WeakThis, Callback](TSharedPtr<IHttpRequest> Req, TSharedPtr<IHttpResponse> Resp, bool bSuccess)
        {
            FString NewToken;

            if (!WeakThis.IsValid())
            {
                Callback.ExecuteIfBound(NewToken);
                return;
            }

            UWeChatAuthService* This = WeakThis.Get();

            if (bSuccess && Resp.IsValid() && Resp->GetResponseCode() == 200)
            {
                FString Content = Resp->GetContentAsString();

                // 解析新的会话令牌
                if (Content.Contains(TEXT("\"session_token\":\"")))
                {
                    int32 TokenStart = Content.Find(TEXT("\"session_token\":\"")) + 16;
                    int32 TokenEnd = Content.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, TokenStart);
                    if (TokenStart > 16 && TokenEnd > TokenStart)
                    {
                        NewToken = Content.Mid(TokenStart, TokenEnd - TokenStart);

                        // 更新当前会话
                        This->CurrentSession.SessionToken = NewToken;
                        This->CurrentSession.ExpiresAt = FDateTime::Now().ToUnixTimestamp() + 7200;

                        UE_LOG(LogTemp, Log, TEXT("[WeChat] 会话刷新成功"));
                    }
                }
            }

            if (NewToken.IsEmpty())
            {
                UE_LOG(LogTemp, Warning, TEXT("[WeChat] 会话刷新失败"));
                This->CurrentSession.bIsValid = false;
            }

            if (Callback.IsBound())
            {
                Callback.ExecuteIfBound(NewToken);
            }
        });

    Request->ProcessRequest();

    UE_LOG(LogTemp, Log, TEXT("[WeChat] 正在刷新会话..."));
}

//==============================================================================
// 获取保存的会话信息
//==============================================================================
FSessionInfo UWeChatAuthService::GetSavedSession() const
{
    return CurrentSession;
}

//==============================================================================
// 保存会话信息
//==============================================================================
void UWeChatAuthService::SaveSession(const FSessionInfo& Session)
{
    CurrentSession = Session;

    // 检查会话是否过期
    if (CurrentSession.ExpiresAt > 0)
    {
        int64 CurrentTime = FDateTime::Now().ToUnixTimestamp();
        if (CurrentSession.ExpiresAt <= CurrentTime)
        {
            CurrentSession.bIsValid = false;
            UE_LOG(LogTemp, Warning, TEXT("[WeChat] 保存的会话已过期"));
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[WeChat] 会话已保存，OpenID: %s"), *CurrentSession.OpenID);
}

//==============================================================================
// 清除会话
//==============================================================================
void UWeChatAuthService::ClearSession()
{
    CurrentSession = FSessionInfo();
    CurrentSession.bIsValid = false;
    UE_LOG(LogTemp, Log, TEXT("[WeChat] 会话已清除"));
}

//==============================================================================
// 以下为占位方法，保留用于直接微信API调用场景
//==============================================================================

void UWeChatAuthService::OnAuthCodeReceived(const FString& AuthCode)
{
    UE_LOG(LogTemp, Log, TEXT("[WeChat] 收到授权码: %s"), *AuthCode);
}

void UWeChatAuthService::ExchangeAuthCodeForToken(const FString& AuthCode)
{
    UE_LOG(LogTemp, Log, TEXT("[WeChat] 使用授权码换取令牌（通过后端）"));
}

void UWeChatAuthService::ParseTokenResponse(const FString& ResponseContent)
{
    UE_LOG(LogTemp, Log, TEXT("[WeChat] 解析令牌响应: %d bytes"), ResponseContent.Len());
}

void UWeChatAuthService::RegisterSessionWithBackend(const FString& WeChatAccessToken, const FString& OpenID)
{
    UE_LOG(LogTemp, Log, TEXT("[WeChat] 向后端注册会话"));
}

void UWeChatAuthService::HandleBackendSessionResponse(TSharedPtr<IHttpRequest> Request, bool bSuccess)
{
    UE_LOG(LogTemp, Log, TEXT("[WeChat] HTTP响应处理（已由Lambda接管）"));
}