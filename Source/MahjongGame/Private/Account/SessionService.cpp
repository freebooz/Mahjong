#include "Account/SessionService.h"
#include "Kismet/GameplayStatics.h"

USessionService::USessionService()
{
    bInitialized = false;
    CurrentSession.bIsGuest = false;
}

void USessionService::Initialize(const FString& InServerURL)
{
    ServerURL = InServerURL;
    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("[Session] Initialized with server: %s"), *ServerURL);
}

void USessionService::LoginWithToken(const FString& SessionToken, FSessionLoginCallback Callback)
{
    if (!bInitialized || SessionToken.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Session] Cannot login with empty token"));
        FUserSession EmptySession;
        Callback.ExecuteIfBound(EmptySession);
        return;
    }

    // Stub: Just create a local session
    FUserSession Session;
    Session.SessionToken = SessionToken;
    Session.Nickname = TEXT("Player");
    Session.PlayerLevel = 1;
    Session.bIsGuest = false;
    CurrentSession = Session;
    Callback.ExecuteIfBound(Session);
}

void USessionService::LoginAsGuest(FSessionLoginCallback Callback)
{
    FUserSession GuestSession;
    GuestSession.SessionToken = TEXT("guest_") + FGuid::NewGuid().ToString();
    GuestSession.OpenID = TEXT("guest_") + FGuid::NewGuid().ToString(EGuidFormats::Short);
    GuestSession.Nickname = TEXT("Guest");
    GuestSession.PlayerLevel = 1;
    GuestSession.LoginTime = FDateTime::UtcNow().ToUnixTimestamp();
    GuestSession.ExpiresAt = GuestSession.LoginTime + (60 * 60 * 24);
    GuestSession.bIsGuest = true;

    CurrentSession = GuestSession;
    UE_LOG(LogTemp, Log, TEXT("[Session] Guest login: %s"), *GuestSession.OpenID);
    Callback.ExecuteIfBound(GuestSession);
}

FUserSession USessionService::GetCurrentSession() const
{
    return CurrentSession;
}

bool USessionService::IsSessionValid() const
{
    if (!CurrentSession.SessionToken.IsEmpty() && CurrentSession.ExpiresAt > 0)
    {
        int64 CurrentTime = FDateTime::UtcNow().ToUnixTimestamp();
        return CurrentSession.ExpiresAt > CurrentTime;
    }
    return false;
}

void USessionService::UpdateSession(const FUserSession& Session)
{
    CurrentSession = Session;
    UE_LOG(LogTemp, Log, TEXT("[Session] Session updated for: %s"), *Session.Nickname);
}

void USessionService::ClearSession()
{
    CurrentSession = FUserSession();
    CurrentSession.bIsGuest = false;
    UE_LOG(LogTemp, Log, TEXT("[Session] Session cleared"));
}

bool USessionService::SaveSessionToDisk()
{
    if (!IsSessionValid())
    {
        return false;
    }

    FString SavePath = FPaths::ProjectSavedDir() + TEXT("session.dat");
    TArray<uint8> Data;
    FMemoryWriter Writer(Data);
    FUserSession RestoredSession = CurrentSession;
    Writer << RestoredSession.SessionToken;
    Writer << RestoredSession.OpenID;
    Writer << RestoredSession.Nickname;
    Writer << RestoredSession.PlayerLevel;
    Writer << RestoredSession.LoginTime;
    Writer << RestoredSession.ExpiresAt;
    uint8 bIsGuestByte = RestoredSession.bIsGuest ? 1 : 0;
    Writer << bIsGuestByte;

    return FFileHelper::SaveArrayToFile(Data, *SavePath);
}

bool USessionService::LoadSessionFromDisk()
{
    FString SavePath = FPaths::ProjectSavedDir() + TEXT("session.dat");
    TArray<uint8> Data;
    if (!FFileHelper::LoadFileToArray(Data, *SavePath))
    {
        return false;
    }

    FMemoryReader Reader(Data);
    (void)(Reader << CurrentSession.SessionToken);
    (void)(Reader << CurrentSession.OpenID);
    (void)(Reader << CurrentSession.Nickname);
    (void)(Reader << CurrentSession.PlayerLevel);
    (void)(Reader << CurrentSession.LoginTime);
    (void)(Reader << CurrentSession.ExpiresAt);
    uint8 bIsGuestByte = CurrentSession.bIsGuest ? 1 : 0;
    (void)(Reader << bIsGuestByte);

    UE_LOG(LogTemp, Log, TEXT("[Session] Session loaded from disk"));
    return IsSessionValid();
}

void USessionService::ValidateSessionOnServer(const FString& Token, FSessionLoginCallback Callback)
{
    // Stub: Just invoke callback with current session
    UE_LOG(LogTemp, Warning, TEXT("[Session] ValidateSessionOnServer stub - using current session"));
    Callback.ExecuteIfBound(CurrentSession);
}