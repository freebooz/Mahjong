#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "RoomPasswordWidget.generated.h"

//==============================================================================
// 房间密码界面
// 用于加入密码房间时的密码输入
// 支持6位数字密码显示和确认
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API URoomPasswordWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 密码输入回调
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Room")
    void OnPasswordSubmit(const FString& Password);

    // 构造函数
    URoomPasswordWidget(const FObjectInitializer& ObjectInitializer);

    // 界面构建完成
    virtual void NativeConstruct() override;

    // 显示密码输入框
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void ShowPasswordInput(const FString& RoomName);

    // 隐藏密码输入框
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void HidePasswordInput();

    // 确认按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnConfirmClicked();

    // 取消按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnCancelClicked();

    // 数字按钮点击（0-9）
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked(int32 Number);

    // 删除按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnDeleteClicked();

    // 清除按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnClearClicked();

    // 设置错误消息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void SetErrorMessage(const FString& ErrorMessage);

    // 获取当前输入的密码
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    FString GetInputPassword() const;

protected:
    // 更新密码显示
    void UpdatePasswordDisplay();

    // 密码长度验证
    bool ValidatePasswordLength() const;

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 背景遮罩
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* BackgroundOverlay;

    // 房间名称文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* RoomNameText;

    // 提示文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* HintText;

    // 错误消息文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ErrorText;

    // 密码显示文本（******格式）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PasswordDisplayText;

    // 密码输入框（EditableTextBox版本）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UEditableTextBox* PasswordInputBox;

    // 确认按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ConfirmButton;

    // 取消按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CancelButton;

    // 数字按钮 0-9
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton0;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton1;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton2;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton3;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton4;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton5;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton6;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton7;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton8;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NumberButton9;

    // 删除按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* DeleteButton;

    // 清除按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ClearButton;

    // 当前输入的密码
    UPROPERTY()
    FString CurrentPassword;

    // 最大密码长度
    UPROPERTY()
    int32 MaxPasswordLength;

    // 目标房间名称
    UPROPERTY()
    FString TargetRoomName;
};
