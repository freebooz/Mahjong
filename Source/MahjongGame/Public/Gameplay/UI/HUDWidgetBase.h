#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "HUDWidgetBase.generated.h"
USTRUCT(BlueprintType)
struct FTimerInfo
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TimeRemaining;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsPaused;
};
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UHUDWidgetBase : public UUserWidget
{
    GENERATED_BODY()
public:
    UHUDWidgetBase(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void ShowGameHUD();
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void HideGameHUD();
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void UpdateTimer(int32 Seconds);
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void SetTurnIndicator(int32 PlayerIndex, bool bIsMyTurn);
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void ShowActionPrompt(const FString& ActionText);
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void HideActionPrompt();
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void ShowTileCount(int32 RemainingTiles);
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void SetMyScore(int32 Score);
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void UpdateConnectionStatus(bool bIsConnected);
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void ShowPauseMenu();
    UFUNCTION(BlueprintCallable, Category="Mahjong|HUD")
    virtual void HidePauseMenu();
protected:
    // Event handlers
    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnQuitClicked();

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* HUDPanel;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TimerText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UProgressBar* TimerProgressBar;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TurnIndicatorText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ActionPromptText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TileCountText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* MyScoreText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ConnectionStatusText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* PauseMenuPanel;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ResumeButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* SettingsButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* QuitButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHorizontalBox* PlayerInfoBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* QuickActionsBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* NetworkIndicator;
    FTimerInfo CurrentTimerInfo;
    int32 MaxTimerSeconds;
};