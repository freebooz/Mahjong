#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "RoomPasswordWidget.generated.h"

//==============================================================================
// 麻将专用层 - 房间密码输入组件
// 用于加入密码房间时输入密码
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API URoomPasswordWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 密码提交回调
    DECLARE_DELEGATE_OneParam(FOnPasswordSubmit, const FString&)

    // 构造函数
    URoomPasswordWidget(const FObjectInitializer& ObjectInitializer);

    // 显示密码输入
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void ShowPasswordInput(const FString& RoomName);

    // 隐藏密码输入
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void HidePasswordInput();

    // 设置错误消息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void SetErrorMessage(const FString& Message);

    // 获取输入密码
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    FString GetInputPassword() const;

    // 设置提交回调
    void SetSubmitCallback(FOnPasswordSubmit Callback);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 确认按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnConfirmClicked();

    // 取消按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnCancelClicked();

    // 数字按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked(int32 Number);

    // 数字按钮 0-9 (供蓝图绑定)
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked0();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked1();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked2();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked3();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked4();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked5();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked6();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked7();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked8();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnNumberClicked9();

    // 删除按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnDeleteClicked();

    // 清除按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnClearClicked();

    // 更新密码显示
    void UpdatePasswordDisplay();

    // 验证密码长度
    bool ValidatePasswordLength() const;

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 房间名称
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* RoomNameText;

    // 提示文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* HintText;

    // 错误文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ErrorText;

    // 密码显示
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PasswordDisplayText;

    // 密码输入框
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

    // 当前密码
    UPROPERTY()
    FString CurrentPassword;

    // 最大密码长度
    UPROPERTY()
    int32 MaxPasswordLength;

    // 目标房间名
    UPROPERTY()
    FString TargetRoomName;

    // 提交回调
    FOnPasswordSubmit SubmitCallback;
};
