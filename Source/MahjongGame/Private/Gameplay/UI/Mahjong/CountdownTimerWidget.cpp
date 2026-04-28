#include "Gameplay/UI/Mahjong/CountdownTimerWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

UCountdownTimerWidget::UCountdownTimerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bIsRunning = false;
    bIsPaused = false;
    TotalTime = 0.0f;
    RemainingTime = 0.0f;
    WarningThreshold = 5.0f;
    bHasTriggeredWarning = false;
}

void UCountdownTimerWidget::NativeConstruct()
{
    Super::NativeConstruct();
    Hide();
}

void UCountdownTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!bIsRunning || bIsPaused)
        return;

    RemainingTime -= InDeltaTime;

    if (RemainingTime <= 0)
    {
        RemainingTime = 0;
        StopCountdown();
        OnCountdownFinished();
    }
    else
    {
        UpdateDisplay();
        CheckWarningThreshold();
    }
}

void UCountdownTimerWidget::StartCountdown(float Duration)
{
    TotalTime = Duration;
    RemainingTime = Duration;
    bIsRunning = true;
    bIsPaused = false;
    bHasTriggeredWarning = false;
    Show();
    UpdateDisplay();
}

void UCountdownTimerWidget::PauseCountdown()
{
    if (bIsRunning)
        bIsPaused = true;
}

void UCountdownTimerWidget::ResumeCountdown()
{
    if (bIsRunning)
        bIsPaused = false;
}

void UCountdownTimerWidget::StopCountdown()
{
    bIsRunning = false;
    bIsPaused = false;
    Hide();
}

float UCountdownTimerWidget::GetRemainingTime() const
{
    return RemainingTime;
}

bool UCountdownTimerWidget::IsCountingDown() const
{
    return bIsRunning && !bIsPaused;
}

void UCountdownTimerWidget::UpdateDisplay()
{
    if (CountdownText)
    {
        int32 DisplaySeconds = FMath::CeilToInt(RemainingTime);
        CountdownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), DisplaySeconds)));
    }

    if (TimerProgressBar && TotalTime > 0)
    {
        TimerProgressBar->SetPercent(RemainingTime / TotalTime);
    }
}

void UCountdownTimerWidget::CheckWarningThreshold()
{
    if (!bHasTriggeredWarning && RemainingTime <= WarningThreshold)
    {
        bHasTriggeredWarning = true;
        OnTimeWarning(RemainingTime);
    }
}
