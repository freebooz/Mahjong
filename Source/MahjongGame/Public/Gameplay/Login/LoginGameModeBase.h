#pragma once
#include "Gameplay/Modes/UIGameModeBase.h"
#include "LoginGameModeBase.generated.h"
class ULoginWidgetBase;
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API ALoginGameModeBase : public AUIGameModeBase
{
    GENERATED_BODY()
public:
    ALoginGameModeBase();
    virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void ShowLoginUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void HideLoginUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnLoginSuccess(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnLoginFailed(const FString& ErrorMessage);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void TransitionToLobby();
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Login")
    TSubclassOf<ULoginWidgetBase> LoginWidgetClass;
    UPROPERTY()
    ULoginWidgetBase* LoginWidgetInstance;
};