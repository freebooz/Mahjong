#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "ReconnectionWidget.generated.h"

//==============================================================================
// 麻将专用层 - 断线重连界面
// 显示断线重连状态、进度、网络恢复等
// 支持自动重连和手动重连
//==============================================================================

// 重连状态
UENUM(BlueprintType)
enum class EReconnectionState : uint8
{
    Disconnected,
    Reconnecting,
    Reconnected,
    Failed
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UReconnectionWidget : public UBaseWidget
{
    GENERATED_BODY()

public:

    // 重连结果回调
    DECLARE_DELEGATE_OneParam(FOnReconnectionResult, bool)

    // 构造函数
    UReconnectionWidget(const FObjectInitializer& ObjectInitializer);

    // 显示断线状态
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    void ShowDisconnected(const FString& Reason = TEXT("网络连接已断开"));

    // 显示正在重连
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    void ShowReconnecting(int32 AttemptNumber, int32 MaxAttempts);

    // 显示重连成功
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    void ShowReconnected();

    // 显示重连失败
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    void ShowReconnectionFailed(const FString& ErrorMessage = TEXT("重连失败"));

    // 隐藏重连界面
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    void HideReconnection();

    // 设置重连结果回调
    void SetReconnectionCallback(FOnReconnectionResult Callback);

    // 获取当前状态
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    EReconnectionState GetCurrentState() const;

    // 是否正在重连
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    bool IsReconnecting() const;

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 每帧更新
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 重试按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    void OnRetryClicked();

    // 返回大厅按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Reconnection")
    void OnReturnToLobbyClicked();

    // 更新进度显示
    void UpdateProgressDisplay();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 状态图标
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* StatusIcon;

    // 旋转图标（用于加载）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* LoadingIcon;

    // 状态标题
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusTitleText;

    // 状态详情
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusDetailText;

    // 重连进度文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ProgressText;

    // 进度条
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UProgressBar* ReconnectProgressBar;

    // 重试按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* RetryButton;

    // 返回大厅按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ReturnToLobbyButton;

    // 背景遮罩
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* ReconnectBackgroundOverlay;

    // 当前状态
    UPROPERTY()
    EReconnectionState CurrentState;

    // 当前重试次数
    UPROPERTY()
    int32 CurrentAttempt;

    // 最大重试次数
    UPROPERTY()
    int32 MaxAttempts;

    // 重连结果回调
    FOnReconnectionResult ReconnectionCallback;

    // 旋转角度
    UPROPERTY()
    float RotationAngle;
};
