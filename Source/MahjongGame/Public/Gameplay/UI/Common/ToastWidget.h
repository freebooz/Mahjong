#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "ToastWidget.generated.h"

//==============================================================================
// 通用层 - 提示消息组件
// 用于显示短暂的操作反馈，如"操作成功"、"网络断开"等
// 支持自动消失和手动关闭
//==============================================================================

// Toast 类型
UENUM(BlueprintType)
enum class EToastType : uint8
{
    Success,
    Warning,
    Error,
    Info
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UToastWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UToastWidget(const FObjectInitializer& ObjectInitializer);

    // 显示成功提示
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void ShowSuccess(const FString& Message, float Duration = 2.0f);

    // 显示警告提示
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void ShowWarning(const FString& Message, float Duration = 3.0f);

    // 显示错误提示
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void ShowError(const FString& Message, float Duration = 4.0f);

    // 显示信息提示
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void ShowInfo(const FString& Message, float Duration = 2.0f);

    // 隐藏提示
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void HideToast();

    // 设置成功背景纹理
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void SetSuccessBackground(UTexture2D* Texture);

    // 设置警告背景纹理
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void SetWarningBackground(UTexture2D* Texture);

    // 设置错误背景纹理
    UFUNCTION(BlueprintCallable, Category="Common|Toast")
    void SetErrorBackground(UTexture2D* Texture);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 显示 Toast
    void ShowToast(const FString& Message, EToastType Type, float Duration);

    // 自动消失计时器回调
    void OnAutoHideTimer();

    // Toast 容器面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* ToastContainer;

    // Toast 项目面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* ToastItem;

    // 消息文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* MessageText;

    // 类型图标
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* TypeIcon;

    // 背景纹理（可选）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
    UImage* ToastBackground;

    // 当前 Toast 类型
    UPROPERTY()
    EToastType CurrentType;

    // 计时器句柄
    FTimerHandle AutoHideTimerHandle;

    // 各类型背景纹理
    UPROPERTY()
    UTexture2D* SuccessBackgroundTexture;

    UPROPERTY()
    UTexture2D* WarningBackgroundTexture;

    UPROPERTY()
    UTexture2D* ErrorBackgroundTexture;
};
