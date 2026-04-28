#include "Account/PlayerAccountService.h"

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

void UPlayerAccountService::InitializeService(const FString& ServerURL, const FString& WeChatAppID, const FString& WeChatAppSecret)
{
    SessionService = NewObject<USessionService>();
    SessionService->Initialize(ServerURL);

    WeChatAuthService = NewObject<UWeChatAuthService>();
    WeChatAuthService->Initialize(WeChatAppID, WeChatAppSecret);

    UE_LOG(LogTemp, Log, TEXT("[Account] Account service initialized"));
}

void UPlayerAccountService::Login(const FString& Username, const FString& Password)
{
    if (Username.IsEmpty() || Password.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Account] Login failed: empty credentials"));
        bIsAuthenticated = false;
        return;
    }
    CurrentUsername = Username;
    bIsAuthenticated = true;
    bIsGuest = false;
    UE_LOG(LogTemp, Log, TEXT("[Account] User %s logged in"), *CurrentUsername);
}

void UPlayerAccountService::LoginWithWeChat(FLoginCallback Callback)
{
    // Stub: WeChat login not implemented yet
    FLoginResult Result;
    Result.bSuccess = false;
    Result.ErrorMessage = TEXT("WeChat login not implemented");
    Callback.ExecuteIfBound(Result);
}

void UPlayerAccountService::LoginWithToken(FLoginCallback Callback)
{
    // Stub: Token-based login not fully implemented
    FLoginResult Result;
    Result.bSuccess = false;
    Result.ErrorMessage = TEXT("Token login not implemented");
    Callback.ExecuteIfBound(Result);
}

void UPlayerAccountService::Logout()
{
    UE_LOG(LogTemp, Log, TEXT("[Account] User %s logged out"), *CurrentUsername);
    CurrentUsername = TEXT("");
    bIsAuthenticated = false;
    bIsGuest = false;
    PlayerLevel = 1;

    if (SessionService)
    {
        SessionService->ClearSession();
    }
}

void UPlayerAccountService::Register(const FString& Username, const FString& Password, const FString& Email)
{
    if (Username.IsEmpty() || Password.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Account] Registration failed: empty credentials"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("[Account] User %s registered with email %s"), *Username, *Email);
    CurrentUsername = Username;
    bIsAuthenticated = true;
    bIsGuest = false;
}

void UPlayerAccountService::TryReconnect(FLoginCallback Callback)
{
    UE_LOG(LogTemp, Log, TEXT("[Account] Attempting reconnection..."));
    LoginWithToken(Callback);
}

void UPlayerAccountService::LoginAsGuest(FLoginCallback Callback)
{
    // Stub: Guest login not fully implemented
    FLoginResult Result;
    Result.bSuccess = false;
    Result.ErrorMessage = TEXT("Guest login not implemented");
    Callback.ExecuteIfBound(Result);
}

FString UPlayerAccountService::GetSessionToken() const
{
    if (SessionService)
    {
        return SessionService->GetCurrentSession().SessionToken;
    }
    return TEXT("");
}

FUserSession UPlayerAccountService::GetCurrentUserSession() const
{
    if (SessionService)
    {
        return SessionService->GetCurrentSession();
    }
    FUserSession EmptySession;
    return EmptySession;
}

void UPlayerAccountService::OnWeChatAuthComplete(const FWeChatAuthResult& Result)
{
    // Stub: WeChat auth complete - just pass through the result
    FLoginResult LoginResult;
    LoginResult.bSuccess = Result.bSuccess;
    LoginResult.ErrorMessage = Result.ErrorMessage;
    PendingLoginCallback.ExecuteIfBound(LoginResult);
}

void UPlayerAccountService::OnSessionLoginComplete(const FUserSession& Session)
{
    FLoginResult LoginResult;
    LoginResult.bSuccess = !Session.SessionToken.IsEmpty();
    LoginResult.Session = Session;

    if (LoginResult.bSuccess)
    {
        CurrentUsername = Session.Nickname;
        PlayerLevel = Session.PlayerLevel;
        bIsAuthenticated = true;
        bIsGuest = Session.bIsGuest;
        SessionService->UpdateSession(Session);
        SessionService->SaveSessionToDisk();
        UE_LOG(LogTemp, Log, TEXT("[Account] Login successful: %s"), *CurrentUsername);
    }
    else
    {
        LoginResult.ErrorMessage = TEXT("Session validation failed");
        UE_LOG(LogTemp, Warning, TEXT("[Account] Session validation failed"));
    }

    PendingLoginCallback.ExecuteIfBound(LoginResult);
}

void UPlayerAccountService::OnReconnectComplete(const FLoginResult& Result)
{
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[Account] Reconnection successful"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Account] Reconnection failed: %s"), *Result.ErrorMessage);
    }
    PendingLoginCallback.ExecuteIfBound(Result);
}

void UPlayerAccountService::AddCoins(int32 Amount)
{
    if (Amount > 0)
    {
        Coins += Amount;
        UE_LOG(LogTemp, Log, TEXT("[Account] Added %d coins, total: %d"), Amount, Coins);
    }
}

bool UPlayerAccountService::SpendCoins(int32 Amount)
{
    if (Amount > 0 && Coins >= Amount)
    {
        Coins -= Amount;
        UE_LOG(LogTemp, Log, TEXT("[Account] Spent %d coins, remaining: %d"), Amount, Coins);
        return true;
    }
    UE_LOG(LogTemp, Warning, TEXT("[Account] Cannot spend %d coins, have %d"), Amount, Coins);
    return false;
}