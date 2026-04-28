#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SessionService.generated.h"

USTRUCT(BlueprintType)
struct FUserSession
{
    GENERATED_BODY()
    UPROPERTY()
    FString SessionToken;
    UPROPERTY()
    FString OpenID;
    UPROPERTY()
    FString Nickname;
    UPROPERTY()
    int32 PlayerLevel;
    UPROPERTY()
    int64 LoginTime;
    UPROPERTY()
    int64 ExpiresAt;
    UPROPERTY()
    bool bIsGuest;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FSessionLoginCallback, const FUserSession&, Session);

UCLASS(Blueprintable)
class MAHJONGGAME_API USessionService : public UObject
{
    GENERATED_BODY()
public:
    USessionService();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void Initialize(const FString& InServerURL);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void LoginWithToken(const FString& SessionToken, FSessionLoginCallback Callback);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void LoginAsGuest(FSessionLoginCallback Callback);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    FUserSession GetCurrentSession() const;

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    bool IsSessionValid() const;

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void UpdateSession(const FUserSession& Session);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void ClearSession();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    bool SaveSessionToDisk();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    bool LoadSessionFromDisk();

protected:
    void ValidateSessionOnServer(const FString& Token, FSessionLoginCallback Callback);
    void OnSessionValidated(bool bSuccess, const FUserSession& Session);

    UPROPERTY()
    FUserSession CurrentSession;

    UPROPERTY()
    FString ServerURL;

    UPROPERTY()
    bool bInitialized;
};