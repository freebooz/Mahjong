#include "Gameplay/UI/HUDWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"

UHUDWidgetBase::UHUDWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , MaxTimerSeconds(60)
    , CurrentTimerInfo()
{
    CurrentTimerInfo.TimeRemaining = 60;
    CurrentTimerInfo.bIsPaused = false;
}

void UHUDWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    if (ResumeButton)
        ResumeButton->OnClicked.AddDynamic(this, &UHUDWidgetBase::OnResumeClicked);
    if (SettingsButton)
        SettingsButton->OnClicked.AddDynamic(this, &UHUDWidgetBase::OnSettingsClicked);
    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UHUDWidgetBase::OnQuitClicked);
    if (TimerText)
        TimerText->SetText(FText::FromString(TEXT("60")));
    if (TimerProgressBar)
        TimerProgressBar->SetPercent(1.0f);
    if (TurnIndicatorText)
        TurnIndicatorText->SetText(FText::FromString(TEXT("")));
    if (ActionPromptText)
        ActionPromptText->SetText(FText::FromString(TEXT("")));
    if (TileCountText)
        TileCountText->SetText(FText::FromString(TEXT("牌数: 144")));
    if (MyScoreText)
        MyScoreText->SetText(FText::FromString(TEXT("分数: 0")));
    if (ConnectionStatusText)
        ConnectionStatusText->SetText(FText::FromString(TEXT("已连接")));
    if (PauseMenuPanel)
        PauseMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
    if (HUDPanel)
        HUDPanel->SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] NativeConstruct"));
}

void UHUDWidgetBase::ShowGameHUD()
{
    if (HUDPanel)
        HUDPanel->SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] ShowGameHUD"));
}

void UHUDWidgetBase::HideGameHUD()
{
    if (HUDPanel)
        HUDPanel->SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] HideGameHUD"));
}

void UHUDWidgetBase::UpdateTimer(int32 Seconds)
{
    CurrentTimerInfo.TimeRemaining = Seconds;
    if (TimerText)
    {
        TimerText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Seconds)));
    }
    if (TimerProgressBar && MaxTimerSeconds > 0)
    {
        TimerProgressBar->SetPercent(static_cast<float>(Seconds) / MaxTimerSeconds);
    }
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] UpdateTimer: %d seconds"), Seconds);
}

void UHUDWidgetBase::SetTurnIndicator(int32 PlayerIndex, bool bIsMyTurn)
{
    if (TurnIndicatorText)
    {
        if (bIsMyTurn)
            TurnIndicatorText->SetText(FText::FromString(TEXT(">>> 你的回合 <<<")));
        else
            TurnIndicatorText->SetText(FText::FromString(FString::Printf(TEXT("玩家 %d 的回合"), PlayerIndex)));
    }
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] SetTurnIndicator: Player=%d, IsMyTurn=%s"), PlayerIndex, bIsMyTurn ? TEXT("true") : TEXT("false"));
}

void UHUDWidgetBase::ShowActionPrompt(const FString& ActionText)
{
    if (ActionPromptText)
    {
        ActionPromptText->SetText(FText::FromString(ActionText));
        ActionPromptText->SetVisibility(ESlateVisibility::Visible);
    }
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] ShowActionPrompt: %s"), *ActionText);
}

void UHUDWidgetBase::HideActionPrompt()
{
    if (ActionPromptText)
        ActionPromptText->SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] HideActionPrompt"));
}

void UHUDWidgetBase::ShowTileCount(int32 RemainingTiles)
{
    if (TileCountText)
        TileCountText->SetText(FText::FromString(FString::Printf(TEXT("牌数: %d"), RemainingTiles)));
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] ShowTileCount: %d"), RemainingTiles);
}

void UHUDWidgetBase::SetMyScore(int32 Score)
{
    if (MyScoreText)
        MyScoreText->SetText(FText::FromString(FString::Printf(TEXT("分数: %d"), Score)));
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] SetMyScore: %d"), Score);
}

void UHUDWidgetBase::UpdateConnectionStatus(bool bIsConnected)
{
    if (ConnectionStatusText)
    {
        if (bIsConnected)
            ConnectionStatusText->SetText(FText::FromString(TEXT("已连接")));
        else
            ConnectionStatusText->SetText(FText::FromString(TEXT("断线中...")));
    }
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] UpdateConnectionStatus: %s"), bIsConnected ? TEXT("true") : TEXT("false"));
}

void UHUDWidgetBase::ShowPauseMenu()
{
    if (PauseMenuPanel)
        PauseMenuPanel->SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] ShowPauseMenu"));
}

void UHUDWidgetBase::HidePauseMenu()
{
    if (PauseMenuPanel)
        PauseMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] HidePauseMenu"));
}

void UHUDWidgetBase::OnResumeClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] OnResumeClicked"));
    HidePauseMenu();
}

void UHUDWidgetBase::OnSettingsClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] OnSettingsClicked"));
}

void UHUDWidgetBase::OnQuitClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[HUDWidgetBase] OnQuitClicked"));
}