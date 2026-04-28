#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "CountdownTimerWidget.generated.h"

//==============================================================================
// 倒计时界面
// 显示回合倒计时、动作选择倒计时等
// 支持自定义时间、颜色变化、声音提示等
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UCountdownTimerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UCountdownTimerWidget(const FObjectInitializer& ObjectInitializer);

    // 界面构建完成
    virtual void NativeConstruct() override;

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

    // 设置倒计时时间（不开始）
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    void SetCountdownTime(float Seconds);

    // 获取当前剩余时间
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    float GetRemainingTime() const;

    // 是否正在倒计时
    UFUNCTION(BlueprintCallable, Category="Mahjong|Timer")
    bool IsCountingDown() const;

    // 倒计时结束回调
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Timer")
    void OnCountdownFinished();

    // 时间警告回调（当剩余时间低于阈值时触发）
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Timer")
    void OnTimeWarning(float RemainingSeconds);

protected:
    // 每帧更新
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 更新显示
    void UpdateDisplay();

    // 检查时间警告
    void CheckTimeWarning();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 倒计时文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* CountdownText;

    // 时间警告文本（最后几秒时显示）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* WarningText;

    // 进度条
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UProgressBar* TimerProgressBar;

    // 圆圈图标
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* CircleIcon;

    // 总时间
    UPROPERTY()
    float TotalTime;

    // 剩余时间
    UPROPERTY()
    float RemainingTime;

    // 是否正在运行
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
