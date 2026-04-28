#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "ErrorMessageWidget.generated.h"

//==============================================================================
// 错误信息界面
// 显示错误提示、警告、网络断开等消息
// 支持自动消失和手动关闭两种模式
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UErrorMessageWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UErrorMessageWidget(const FObjectInitializer& ObjectInitializer);

    // 界面构建完成
    virtual void NativeConstruct() override;

    // 显示错误消息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Error")
    void ShowError(const FString& ErrorMessage, float Duration = 3.0f);

    // 显示警告消息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Error")
    void ShowWarning(const FString& WarningMessage, float Duration = 5.0f);

    // 显示网络错误
    UFUNCTION(BlueprintCallable, Category="Mahjong|Error")
    void ShowNetworkError(const FString& CustomMessage = TEXT(""));

    // 显示验证错误
    UFUNCTION(BlueprintCallable, Category="Mahjong|Error")
    void ShowValidationError(const FString& FieldName, const FString& ErrorMessage);

    // 隐藏错误消息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Error")
    void HideError();

    // 关闭按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Error")
    void OnCloseClicked();

protected:
    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 错误图标
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* ErrorIcon;

    // 警告图标
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* WarningIcon;

    // 消息文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* MessageText;

    // 详情文本（可选显示）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* DetailText;

    // 关闭按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CloseButton;

    // 自动消失计时器句柄
    FTimerHandle AutoHideTimerHandle;

    // 当前错误类型
    UPROPERTY()
    int32 CurrentErrorType;
};
