#include "Gameplay/UI/RoomPasswordWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

//==============================================================================
// 房间密码界面实现
//==============================================================================
URoomPasswordWidget::URoomPasswordWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    MaxPasswordLength = 6;
    CurrentPassword = TEXT("");
    TargetRoomName = TEXT("");
}

//==============================================================================
// 界面构建完成
//==============================================================================
void URoomPasswordWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ConfirmButton)
        ConfirmButton->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnConfirmClicked);

    if (CancelButton)
        CancelButton->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnCancelClicked);

    if (DeleteButton)
        DeleteButton->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnDeleteClicked);

    if (ClearButton)
        ClearButton->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnClearClicked);

    if (NumberButton0)
        NumberButton0->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber0Clicked);

    if (NumberButton1)
        NumberButton1->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber1Clicked);

    if (NumberButton2)
        NumberButton2->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber2Clicked);

    if (NumberButton3)
        NumberButton3->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber3Clicked);

    if (NumberButton4)
        NumberButton4->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber4Clicked);

    if (NumberButton5)
        NumberButton5->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber5Clicked);

    if (NumberButton6)
        NumberButton6->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber6Clicked);

    if (NumberButton7)
        NumberButton7->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber7Clicked);

    if (NumberButton8)
        NumberButton8->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber8Clicked);

    if (NumberButton9)
        NumberButton9->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumber9Clicked);

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[RoomPassword] 房间密码界面已构建"));
}

//==============================================================================
// 显示密码输入框
//==============================================================================
void URoomPasswordWidget::ShowPasswordInput(const FString& RoomName)
{
    TargetRoomName = RoomName;
    CurrentPassword = TEXT("");

    if (RoomNameText)
        RoomNameText->SetText(FText::FromString(RoomName));

    if (ErrorText)
        ErrorText->SetText(FText::FromString(TEXT("")));

    UpdatePasswordDisplay();

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Log, TEXT("[RoomPassword] 显示密码输入框: %s"), *RoomName);
}

//==============================================================================
// 隐藏密码输入框
//==============================================================================
void URoomPasswordWidget::HidePasswordInput()
{
    CurrentPassword = TEXT("");

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[RoomPassword] 隐藏密码输入框"));
}

//==============================================================================
// 确认按钮点击
//==============================================================================
void URoomPasswordWidget::OnConfirmClicked()
{
    if (!ValidatePasswordLength())
    {
        SetErrorMessage(TEXT("请输入完整的6位密码"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[RoomPassword] 确认密码输入"));
    OnPasswordSubmit(CurrentPassword);
    HidePasswordInput();
}

//==============================================================================
// 取消按钮点击
//==============================================================================
void URoomPasswordWidget::OnCancelClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomPassword] 取消密码输入"));
    HidePasswordInput();
}

//==============================================================================
// 数字按钮点击
//==============================================================================
void URoomPasswordWidget::OnNumberClicked(int32 Number)
{
    if (CurrentPassword.Len() < MaxPasswordLength)
    {
        CurrentPassword += FString::Printf(TEXT("%d"), Number);
        UpdatePasswordDisplay();
    }
}

//==============================================================================
// 删除按钮点击
//==============================================================================
void URoomPasswordWidget::OnDeleteClicked()
{
    if (!CurrentPassword.IsEmpty())
    {
        CurrentPassword.LeftChopInline(1);
        UpdatePasswordDisplay();
    }
}

//==============================================================================
// 清除按钮点击
//==============================================================================
void URoomPasswordWidget::OnClearClicked()
{
    CurrentPassword = TEXT("");
    UpdatePasswordDisplay();
}

//==============================================================================
// 设置错误消息
//==============================================================================
void URoomPasswordWidget::SetErrorMessage(const FString& ErrorMessage)
{
    if (ErrorText)
        ErrorText->SetText(FText::FromString(ErrorMessage));
}

//==============================================================================
// 获取当前输入的密码
//==============================================================================
FString URoomPasswordWidget::GetInputPassword() const
{
    return CurrentPassword;
}

//==============================================================================
// 更新密码显示
//==============================================================================
void URoomPasswordWidget::UpdatePasswordDisplay()
{
    if (PasswordDisplayText)
    {
        FString Display = TEXT("");
        for (int32 i = 0; i < CurrentPassword.Len(); i++)
        {
            Display += TEXT("*");
        }
        for (int32 i = CurrentPassword.Len(); i < MaxPasswordLength; i++)
        {
            Display += TEXT("-");
        }
        PasswordDisplayText->SetText(FText::FromString(Display));
    }
}

//==============================================================================
// 密码长度验证
//==============================================================================
bool URoomPasswordWidget::ValidatePasswordLength() const
{
    return CurrentPassword.Len() == MaxPasswordLength;
}
