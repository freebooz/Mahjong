//==============================================================================
// 麻将游戏 - 会话服务实现
// SessionService.cpp
//
// 负责玩家会话管理、验证、持久化等功能
// 支持：Token 登录、访客登录、会话验证、会话持久化
//==============================================================================

#include "Account/SessionService.h"
#include "Kismet/GameplayStatics.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

//==============================================================================
// 构造函数
//==============================================================================
USessionService::USessionService()
{
    bInitialized = false;
    CurrentSession.bIsGuest = false;
}

//==============================================================================
// 初始化会话服务
//==============================================================================
void USessionService::Initialize(const FString& InServerURL)
{
    ServerURL = InServerURL;
    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("[Session] 会话服务初始化，服务器: %s"), *ServerURL);
}

//==============================================================================
// 使用 Token 登录
// 向服务器验证 Token 获取会话信息
//==============================================================================
void USessionService::LoginWithToken(const FString& SessionToken, FSessionLoginCallback Callback)
{
    if (!bInitialized || SessionToken.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Session] Token 登录失败：参数无效"));
        FUserSession EmptySession;
        Callback.ExecuteIfBound(EmptySession);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Session] 正在验证 Token..."));

    // 构建验证请求
    FString RequestURL = ServerURL + TEXT("/api/auth/validate");
    FString RequestBody = FString::Printf(TEXT("{\"session_token\":\"%s\"}"), *SessionToken);

    // 创建 HTTP 请求
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(RequestURL);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(RequestBody);

    // 使用弱引用避免循环引用
    TWeakObjectPtr<USessionService> WeakThis(this);
    FSessionLoginCallback WeakCallback = Callback;

    Request->OnProcessRequestComplete().BindLambda([
        WeakThis, WeakCallback](TSharedPtr<IHttpRequest> Req, TSharedPtr<IHttpResponse> Resp, bool bSuccess)
        {
            if (!WeakThis.IsValid())
            {
                FUserSession EmptySession;
                WeakCallback.ExecuteIfBound(EmptySession);
                return;
            }

            USessionService* This = WeakThis.Get();
            FUserSession Session;

            if (bSuccess && Resp.IsValid() && Resp->GetResponseCode() == 200)
            {
                FString Content = Resp->GetContentAsString();
                UE_LOG(LogTemp, Log, TEXT("[Session] 服务器响应: %s"), *Content);

                // 解析会话响应
                This->ParseSessionResponse(Content);

                // 使用解析后的会话
                Session = This->CurrentSession;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[Session] Token 验证失败，使用本地会话"));

                // 验证失败时，返回本地保存的会话（如果有）
                if (!This->CurrentSession.SessionToken.IsEmpty())
                {
                    Session = This->CurrentSession;
                }
            }

            WeakCallback.ExecuteIfBound(Session);
        });

    Request->ProcessRequest();
}

//==============================================================================
// 访客登录
// 创建一个本地访客会话
//==============================================================================
void USessionService::LoginAsGuest(FSessionLoginCallback Callback)
{
    UE_LOG(LogTemp, Log, TEXT("[Session] 创建访客会话..."));

    // 生成唯一的访客 ID
    FString GuestID = TEXT("guest_") + FGuid::NewGuid().ToString(EGuidFormats::Short);

    // 创建访客会话
    FUserSession GuestSession;
    GuestSession.SessionToken = GuestID;  // 访客使用 GuestID 作为临时 Token
    GuestSession.OpenID = GuestID;
    GuestSession.Nickname = TEXT("访客") + GuestID.Left(4);
    GuestSession.PlayerLevel = 1;
    GuestSession.LoginTime = FDateTime::UtcNow().ToUnixTimestamp();
    GuestSession.ExpiresAt = GuestSession.LoginTime + (60 * 60 * 24);  // 24 小时
    GuestSession.bIsGuest = true;

    // 保存会话
    CurrentSession = GuestSession;

    // 持久化到磁盘
    SaveSessionToDisk();

    UE_LOG(LogTemp, Log, TEXT("[Session] 访客会话创建成功: %s"), *GuestSession.Nickname);

    // 执行回调
    Callback.ExecuteIfBound(GuestSession);
}

//==============================================================================
// 获取当前会话
//==============================================================================
FUserSession USessionService::GetCurrentSession() const
{
    return CurrentSession;
}

//==============================================================================
// 检查会话是否有效
//==============================================================================
bool USessionService::IsSessionValid() const
{
    // 检查 Token 是否存在
    if (CurrentSession.SessionToken.IsEmpty())
    {
        return false;
    }

    // 访客会话检查过期时间
    if (!CurrentSession.bIsGuest && CurrentSession.ExpiresAt > 0)
    {
        int64 CurrentTime = FDateTime::UtcNow().ToUnixTimestamp();
        return CurrentSession.ExpiresAt > CurrentTime;
    }

    return true;
}

//==============================================================================
// 更新会话信息
//==============================================================================
void USessionService::UpdateSession(const FUserSession& Session)
{
    CurrentSession = Session;
    UE_LOG(LogTemp, Log, TEXT("[Session] 会话已更新: %s"), *Session.Nickname);
}

//==============================================================================
// 清除会话
//==============================================================================
void USessionService::ClearSession()
{
    CurrentSession = FUserSession();
    CurrentSession.bIsGuest = false;
    UE_LOG(LogTemp, Log, TEXT("[Session] 会话已清除"));
}

//==============================================================================
// 保存会话到磁盘
// 使用二进制序列化保存会话信息
//==============================================================================
bool USessionService::SaveSessionToDisk()
{
    // 检查会话是否有效
    if (!IsSessionValid() && !CurrentSession.bIsGuest)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Session] 无效会话，跳过保存"));
        return false;
    }

    // 构建保存路径
    FString SavePath = FPaths::ProjectSavedDir() + TEXT("session.dat");

    // 创建二进制数据
    TArray<uint8> Data;
    FMemoryWriter Writer(Data);

    // 序列化会话数据
    Writer << CurrentSession.SessionToken;
    Writer << CurrentSession.OpenID;
    Writer << CurrentSession.Nickname;
    Writer << CurrentSession.PlayerLevel;
    Writer << CurrentSession.LoginTime;
    Writer << CurrentSession.ExpiresAt;
    uint8 bIsGuestByte = CurrentSession.bIsGuest ? 1 : 0;
    Writer << bIsGuestByte;

    // 写入文件
    bool bSuccess = FFileHelper::SaveArrayToFile(Data, *SavePath);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[Session] 会话已保存到: %s"), *SavePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Session] 会话保存失败: %s"), *SavePath);
    }

    return bSuccess;
}

//==============================================================================
// 从磁盘加载会话
//==============================================================================
bool USessionService::LoadSessionFromDisk()
{
    FString SavePath = FPaths::ProjectSavedDir() + TEXT("session.dat");

    // 检查文件是否存在
    if (!FPaths::FileExists(SavePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("[Session] 无保存的会话文件"));
        return false;
    }

    // 加载文件
    TArray<uint8> Data;
    if (!FFileHelper::LoadFileToArray(Data, *SavePath))
    {
        UE_LOG(LogTemp, Error, TEXT("[Session] 会话文件加载失败"));
        return false;
    }

    // 反序列化
    FMemoryReader Reader(Data);
    Reader << CurrentSession.SessionToken;
    Reader << CurrentSession.OpenID;
    Reader << CurrentSession.Nickname;
    Reader << CurrentSession.PlayerLevel;
    Reader << CurrentSession.LoginTime;
    Reader << CurrentSession.ExpiresAt;
    uint8 bIsGuestByte = 0;
    Reader << bIsGuestByte;
    CurrentSession.bIsGuest = (bIsGuestByte == 1);

    UE_LOG(LogTemp, Log, TEXT("[Session] 会话已从磁盘加载: %s"), *CurrentSession.Nickname);

    return true;
}

//==============================================================================
// 在服务器上验证会话
// 向服务器发送请求验证会话的有效性
//==============================================================================
void USessionService::ValidateSessionOnServer(const FString& Token, FSessionLoginCallback Callback)
{
    if (!bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Session] 服务未初始化"));
        Callback.ExecuteIfBound(CurrentSession);
        return;
    }

    if (Token.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Session] Token 为空"));
        Callback.ExecuteIfBound(CurrentSession);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Session] 正在服务器上验证会话..."));

    // 构建验证请求
    FString RequestURL = ServerURL + TEXT("/api/auth/validate");
    FString RequestBody = FString::Printf(TEXT("{\"session_token\":\"%s\"}"), *Token);

    // 创建 HTTP 请求
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(RequestURL);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(RequestBody);

    // 使用弱引用
    TWeakObjectPtr<USessionService> WeakThis(this);
    FSessionLoginCallback WeakCallback = Callback;

    Request->OnProcessRequestComplete().BindLambda([
        WeakThis, WeakCallback](TSharedPtr<IHttpRequest> Req, TSharedPtr<IHttpResponse> Resp, bool bSuccess)
        {
            if (!WeakThis.IsValid())
            {
                WeakCallback.ExecuteIfBound(FUserSession());
                return;
            }

            USessionService* This = WeakThis.Get();
            FUserSession Session;

            if (bSuccess && Resp.IsValid())
            {
                int32 ResponseCode = Resp->GetResponseCode();
                FString Content = Resp->GetContentAsString();

                UE_LOG(LogTemp, Log, TEXT("[Session] 服务器验证响应码: %d"), ResponseCode);

                if (ResponseCode == 200)
                {
                    // 解析会话数据
                    This->ParseSessionResponse(Content);
                    Session = This->CurrentSession;

                    // 更新本地会话
                    This->SaveSessionToDisk();

                    UE_LOG(LogTemp, Log, TEXT("[Session] 服务器验证成功"));
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("[Session] 服务器验证失败: %d"), ResponseCode);

                    // 返回本地会话作为后备
                    Session = This->CurrentSession;
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[Session] 服务器验证请求失败"));

                // 网络错误时返回本地会话
                Session = This->CurrentSession;
            }

            WeakCallback.ExecuteIfBound(Session);
        });

    Request->ProcessRequest();
}

//==============================================================================
// 解析会话响应
// 从 JSON 响应中提取会话信息
//==============================================================================
void USessionService::ParseSessionResponse(const FString& ResponseContent)
{
    if (ResponseContent.IsEmpty())
    {
        return;
    }

    // 简单 JSON 解析（生产环境应使用 FJsonObject）

    // 解析 Nickname
    if (ResponseContent.Contains(TEXT("\"nickname\":")))
    {
        int32 Start = ResponseContent.Find(TEXT("\"nickname\":\"")) + 12;
        int32 End = ResponseContent.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, Start);
        if (Start > 12 && End > Start)
        {
            CurrentSession.Nickname = ResponseContent.Mid(Start, End - Start);
        }
    }

    // 解析 PlayerLevel
    if (ResponseContent.Contains(TEXT("\"player_level\":")))
    {
        int32 Start = ResponseContent.Find(TEXT("\"player_level\":")) + 15;
        FString LevelStr;
        int32 End = Start;
        while (End < ResponseContent.Len() && FChar::IsDigit(ResponseContent[End]))
        {
            End++;
        }
        if (End > Start)
        {
            LevelStr = ResponseContent.Mid(Start, End - Start);
            CurrentSession.PlayerLevel = FCString::Atoi(*LevelStr);
        }
    }

    // 解析 SessionToken（可能更新）
    if (ResponseContent.Contains(TEXT("\"session_token\":")))
    {
        int32 Start = ResponseContent.Find(TEXT("\"session_token\":\"")) + 16;
        int32 End = ResponseContent.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, Start);
        if (Start > 16 && End > Start)
        {
            CurrentSession.SessionToken = ResponseContent.Mid(Start, End - Start);
        }
    }

    // 解析 ExpiresAt
    if (ResponseContent.Contains(TEXT("\"expires_at\":")))
    {
        int32 Start = ResponseContent.Find(TEXT("\"expires_at\":")) + 13;
        FString TimeStr;
        int32 End = Start;
        while (End < ResponseContent.Len() && FChar::IsDigit(ResponseContent[End]))
        {
            End++;
        }
        if (End > Start)
        {
            TimeStr = ResponseContent.Mid(Start, End - Start);
            CurrentSession.ExpiresAt = FCString::Atoi64(*TimeStr);
        }
    }
}