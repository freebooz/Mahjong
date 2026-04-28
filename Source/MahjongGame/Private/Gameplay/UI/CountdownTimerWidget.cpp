#include "Gameplay/UI/CountdownTimerWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

//==============================================================================
// 倒计时界面实现
//==============================================================================
UCountdownTimerWidget::UCountdownTimerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bIsRunning = false;
    bIsPaused = false;
    TotalTime = 0.0f;
    RemainingTime = 0.0f;
    WarningThreshold = 5.0f;
    bHasTriggeredWarning = false;
}

//==============================================================================
// 界面构建完成
//==============================================================================
void UCountdownTimerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[CountdownTimer] 倒计时界面已构建"));
}

//==============================================================================
// 每帧更新
//==============================================================================
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
        CheckTimeWarning();
    }
}

//==============================================================================
// 开始倒计时
//==============================================================================
void UCountdownTimerWidget::StartCountdown(float Duration)
{
    TotalTime = Duration;
    RemainingTime = Duration;
    bIsRunning = true;
    bIsPaused = false;
    bHasTriggeredWarning = false;

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Visible);

    UpdateDisplay();

    UE_LOG(LogTemp, Log, TEXT("[CountdownTimer] 开始倒计时: %.1f 秒"), Duration);
}

//==============================================================================
// 暂停倒计时
//==============================================================================
void UCountdownTimerWidget::PauseCountdown()
{
    if (bIsRunning && !bIsPaused)
    {
        bIsPaused = true;
        UE_LOG(LogTemp, Log, TEXT("[CountdownTimer] 倒计时已暂停"));
    }
}

//==============================================================================
// 恢复倒计时
//==============================================================================
void UCountdownTimerWidget::ResumeCountdown()
{
    if (bIsRunning && bIsPaused)
    {
        bIsPaused = false;
        UE_LOG(LogTemp, Log, TEXT("[CountdownTimer] 倒计时已恢复"));
    }
}

//==============================================================================
// 停止倒计时
//==============================================================================
void UCountdownTimerWidget::StopCountdown()
{
    bIsRunning = false;
    bIsPaused = false;

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[CountdownTimer] 倒计时已停止"));
}

//==============================================================================
// 设置倒计时时间
//==============================================================================
void UCountdownTimerWidget::SetCountdownTime(float Seconds)
{
    TotalTime = Seconds;
    RemainingTime = Seconds;
    UpdateDisplay();
}

//==============================================================================
// 获取剩余时间
//==============================================================================
float UCountdownTimerWidget::GetRemainingTime() const
{
    return RemainingTime;
}

//==============================================================================
// 是否正在倒计时
//==============================================================================
bool UCountdownTimerWidget::IsCountingDown() const
{
    return bIsRunning && !bIsPaused;
}

//==============================================================================
// 更新显示
//==============================================================================
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

//==============================================================================
// 检查时间警告
//==============================================================================
void UCountdownTimerWidget::CheckTimeWarning()
{
    if (!bHasTriggeredWarning && RemainingTime <= WarningThreshold)
    {
        bHasTriggeredWarning = true;
        OnTimeWarning(RemainingTime);
    }
}
