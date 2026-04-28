#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Account/SessionService.h"
#include "Account/WeChatAuthService.h"
#include "PlayerAccountService.generated.h"

USTRUCT(BlueprintType)
struct FLoginResult
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;
    UPROPERTY(BlueprintReadOnly)
    FUserSession Session;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FLoginCallback, const FLoginResult&, Result);

UCLASS(Blueprintable)
class MAHJONGGAME_API UPlayerAccountService : public UObject
{
    GENERATED_BODY()
public:
    UPlayerAccountService();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void InitializeService(const FString& ServerURL, const FString& WeChatAppID, const FString& WeChatAppSecret);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void Login(const FString& Username, const FString& Password);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void LoginWithWeChat(FLoginCallback Callback);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void LoginWithToken(FLoginCallback Callback);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void Logout();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void Register(const FString& Username, const FString& Password, const FString& Email);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void TryReconnect(FLoginCallback Callback);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void LoginAsGuest(FLoginCallback Callback);

    UFUNCTION(BlueprintPure, Category="Mahjong|Account")
    FString GetCurrentUsername() const { return CurrentUsername; }

    UFUNCTION(BlueprintPure, Category="Mahjong|Account")
    bool IsAuthenticated() const { return bIsAuthenticated; }

    UFUNCTION(BlueprintPure, Category="Mahjong|Account")
    int32 GetPlayerLevel() const { return PlayerLevel; }

    UFUNCTION(BlueprintPure, Category="Mahjong|Account")
    FString GetSessionToken() const;

    UFUNCTION(BlueprintPure, Category="Mahjong|Account")
    FUserSession GetCurrentUserSession() const;

    UFUNCTION(BlueprintPure, Category="Mahjong|Account")
    bool IsGuest() const { return bIsGuest; }

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    int32 GetCoins() const { return Coins; }

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    void AddCoins(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Account")
    bool SpendCoins(int32 Amount);

protected:
    void OnWeChatAuthComplete(const FWeChatAuthResult& Result);
    void OnSessionLoginComplete(const FUserSession& Session);
    void OnReconnectComplete(const FLoginResult& Result);

    UPROPERTY()
    FString CurrentUsername;

    UPROPERTY()
    bool bIsAuthenticated;

    UPROPERTY()
    bool bIsGuest;

    UPROPERTY()
    int32 PlayerLevel;

    UPROPERTY()
    int32 Coins;

    UPROPERTY()
    USessionService* SessionService;

    UPROPERTY()
    UWeChatAuthService* WeChatAuthService;

    FLoginCallback PendingLoginCallback;
};