#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "ConfirmDialogWidget.generated.h"

//==============================================================================
// 通用层 - 确认对话框
// 用于显示确认提示，如"是否确认退出？"等
// 支持自定义标题、内容和按钮文本
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UConfirmDialogWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 确认回调类型
    DECLARE_DELEGATE(FOnConfirmDelegate)
    DECLARE_DELEGATE(FOnCancelDelegate)

    // 构造函数
    UConfirmDialogWidget(const FObjectInitializer& ObjectInitializer);

    // 显示对话框
    UFUNCTION(BlueprintCallable, Category="Common|Dialog")
    void ShowConfirmDialog(
        const FString& Title,
        const FString& Message,
        const FString& ConfirmText = TEXT("确认"),
        const FString& CancelText = TEXT("取消")
    );

    // 隐藏对话框
    UFUNCTION(BlueprintCallable, Category="Common|Dialog")
    void HideDialog();

    // 设置确认回调
    void SetConfirmCallback(FOnConfirmDelegate Callback);

    // 设置取消回调
    void SetCancelCallback(FOnCancelDelegate Callback);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 确认按钮点击
    UFUNCTION(BlueprintCallable, Category="Common|Dialog")
    void OnConfirmClicked();

    // 取消按钮点击
    UFUNCTION(BlueprintCallable, Category="Common|Dialog")
    void OnCancelClicked();

    // 背景遮罩点击
    UFUNCTION(BlueprintCallable, Category="Common|Dialog")
    void OnBackgroundClicked();

public:
    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 标题文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TitleText;

    // 消息文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* MessageText;

    // 确认按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ConfirmButton;

    // 取消按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CancelButton;

    // 背景遮罩
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* DialogBackgroundOverlay;

protected:
    // 确认回调
    FOnConfirmDelegate ConfirmCallback;

    // 取消回调
    FOnCancelDelegate CancelCallback;
};
