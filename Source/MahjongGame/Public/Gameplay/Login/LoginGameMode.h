#pragma once
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"
class ULoginWidget;
UCLASS()
class MAHJONGGAME_API ALoginGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ALoginGameMode();
    virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    void ShowLoginUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    void HideLoginUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    void OnLoginSuccess(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    void OnLoginFailed(const FString& ErrorMessage);
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Login")
    TSubclassOf<ULoginWidget> LoginWidgetClass;
    UPROPERTY()
    ULoginWidget* LoginWidgetInstance;
};