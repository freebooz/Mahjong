#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "CountdownTimerWidget.generated.h"

//==============================================================================
// 麻将专用层 - 倒计时组件
// 显示回合倒计时、动作选择倒计时
// 支持颜色变化和声音提示
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UCountdownTimerWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UCountdownTimerWidget(const FObjectInitializer& ObjectInitializer);

    // 开始倒计时
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    void StartCountdown(float Duration);

    // 暂停倒计时
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    void PauseCountdown();

    // 恢复倒计时
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    void ResumeCountdown();

    // 停止倒计时
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    void StopCountdown();

    // 获取剩余时间
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    float GetRemainingTime() const;

    // 是否正在倒计时
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    bool IsCountingDown() const;

    // 倒计时结束事件
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Timer")
    void OnCountdownFinished();

    // 时间警告事件（最后几秒）
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Timer")
    void OnTimeWarning(float RemainingSeconds);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 每帧更新
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 更新显示
    void UpdateDisplay();

    // 检查警告阈值
    void CheckWarningThreshold();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 倒计时文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* CountdownText;

    // 进度条
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UProgressBar* TimerProgressBar;

    // 警告文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* WarningText;

    // 总时间
    UPROPERTY()
    float TotalTime;

    // 剩余时间
    UPROPERTY()
    float RemainingTime;

    // 是否运行中
    UPROPERTY()
    bool bIsRunning;

    // 是否暂停
    UPROPERTY()
    bool bIsPaused;

    // 警告阈值（秒）
    UPROPERTY()
    float WarningThreshold;

    // 是否已触发警告
    UPROPERTY()
    bool bHasTriggeredWarning;
};
