#pragma once
#include "GameFramework/PlayerController.h"
#include "LoginPlayerControllerBase.generated.h"
class ULoginWidgetBase;
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API ALoginPlayerControllerBase : public APlayerController
{
    GENERATED_BODY()
public:
    ALoginPlayerControllerBase();
    virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void SendLoginRequest(const FString& Username, const FString& Password);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void SendRegisterRequest(const FString& Username, const FString& Password, const FString& Email);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnLoginSuccess(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnLoginFailed(const FString& ErrorMessage);
protected:
    UPROPERTY()
    bool bIsLoggedIn;
    UPROPERTY()
    FString CurrentPlayerName;
};