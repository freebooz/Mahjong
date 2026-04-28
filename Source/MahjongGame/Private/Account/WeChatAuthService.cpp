#include "Account/WeChatAuthService.h"
#include "Kismet/GameplayStatics.h"

// 静态URL模板定义
const TCHAR* UWeChatAuthService::WeChatAuthURLTemplate = TEXT(
    "https://open.weixin.qq.com/connect/qrconnect?"
    "appid=%s&"
    "redirect_uri=%s&"
    "response_type=code&"
    "scope=snsapi_login&"
    "state=%s#wechat_redirect"
);

UWeChatAuthService::UWeChatAuthService()
{
    bInitialized = false;
}

void UWeChatAuthService::Initialize(const FString& AppID, const FString& AppSecret)
{
    WeChatAppID = AppID;
    WeChatAppSecret = AppSecret;
    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("[WeChat] Initialized with AppID: %s"), *WeChatAppID);
}

void UWeChatAuthService::LoginWithWeChat(FWeChatAuthCallback Callback)
{
    if (!bInitialized)
    {
        FWeChatAuthResult Result;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("WeChat auth not initialized");
        Callback.ExecuteIfBound(Result);
        return;
    }

    // Stub: WeChat login not fully implemented
    FWeChatAuthResult Result;
    Result.bSuccess = false;
    Result.ErrorMessage = TEXT("WeChat login not implemented");
    Callback.ExecuteIfBound(Result);
}

void UWeChatAuthService::RefreshSession(FWeChatTokenCallback Callback)
{
    if (CurrentSession.RefreshToken.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[WeChat] No refresh token available"));
        Callback.ExecuteIfBound(TEXT(""));
        return;
    }

    // Stub: Just return empty token
    UE_LOG(LogTemp, Warning, TEXT("[WeChat] RefreshSession stub - returning empty"));
    Callback.ExecuteIfBound(TEXT(""));
}

FSessionInfo UWeChatAuthService::GetSavedSession() const
{
    return CurrentSession;
}

void UWeChatAuthService::SaveSession(const FSessionInfo& Session)
{
    CurrentSession = Session;
    UE_LOG(LogTemp, Log, TEXT("[WeChat] Session saved for OpenID: %s"), *Session.OpenID);
}

void UWeChatAuthService::ClearSession()
{
    CurrentSession = FSessionInfo();
    CurrentSession.bIsValid = false;
    UE_LOG(LogTemp, Log, TEXT("[WeChat] Session cleared"));
}

bool UWeChatAuthService::IsWeChatAvailable() const
{
    return bInitialized;
}

FString UWeChatAuthService::GenerateAuthURL(const FString& RedirectURI, const FString& State)
{
    return FString::Format(WeChatAuthURLTemplate, {*WeChatAppID, *RedirectURI, *State});
}

void UWeChatAuthService::OnAuthCodeReceived(const FString& AuthCode)
{
    // Stub: Just log the auth code
    UE_LOG(LogTemp, Log, TEXT("[WeChat] Auth code received: %s (not processed)"), *AuthCode);
}

void UWeChatAuthService::ExchangeAuthCodeForToken(const FString& AuthCode)
{
    // Stub: WeChat token exchange not implemented
    // Security: Don't log auth code as it may contain sensitive data
    UE_LOG(LogTemp, Warning, TEXT("[WeChat] ExchangeAuthCodeForToken stub - auth code received (not logged)"));
}

void UWeChatAuthService::ParseTokenResponse(const FString& ResponseContent)
{
    // Stub: WeChat token response parsing not implemented
    // Security: Don't log response content as it may contain tokens/secrets
    UE_LOG(LogTemp, Warning, TEXT("[WeChat] ParseTokenResponse stub - response received (length: %d, not logged)"), ResponseContent.Len());
}