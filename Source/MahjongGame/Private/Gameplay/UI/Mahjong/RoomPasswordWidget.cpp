#include "Gameplay/UI/Mahjong/RoomPasswordWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "Gameplay/UI/Common/UISoundManager.h"

URoomPasswordWidget::URoomPasswordWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    MaxPasswordLength = 6;
    CurrentPassword = TEXT("");
}

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

    if (NumberButton0) NumberButton0->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked0);
    if (NumberButton1) NumberButton1->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked1);
    if (NumberButton2) NumberButton2->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked2);
    if (NumberButton3) NumberButton3->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked3);
    if (NumberButton4) NumberButton4->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked4);
    if (NumberButton5) NumberButton5->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked5);
    if (NumberButton6) NumberButton6->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked6);
    if (NumberButton7) NumberButton7->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked7);
    if (NumberButton8) NumberButton8->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked8);
    if (NumberButton9) NumberButton9->OnClicked.AddDynamic(this, &URoomPasswordWidget::OnNumberClicked9);

    HidePasswordInput();
}

void URoomPasswordWidget::ShowPasswordInput(const FString& RoomName)
{
    TargetRoomName = RoomName;
    CurrentPassword = TEXT("");

    if (RoomNameText)
        RoomNameText->SetText(FText::FromString(RoomName));

    if (ErrorText)
        ErrorText->SetText(FText::FromString(TEXT("")));

    UpdatePasswordDisplay();
    Show();
}

void URoomPasswordWidget::HidePasswordInput()
{
    CurrentPassword = TEXT("");
    Hide();
}

void URoomPasswordWidget::SetErrorMessage(const FString& Message)
{
    if (ErrorText)
        ErrorText->SetText(FText::FromString(Message));
}

FString URoomPasswordWidget::GetInputPassword() const
{
    return CurrentPassword;
}

void URoomPasswordWidget::SetSubmitCallback(FOnPasswordSubmit Callback)
{
    SubmitCallback = Callback;
}

void URoomPasswordWidget::OnConfirmClicked()
{
    if (!ValidatePasswordLength())
    {
        UUISoundManager::PlayErrorSound(this);
        SetErrorMessage(TEXT("请输入完整的6位密码"));
        return;
    }

    UUISoundManager::PlayConfirmClick(this);
    SubmitCallback.ExecuteIfBound(CurrentPassword);
    HidePasswordInput();
}

void URoomPasswordWidget::OnCancelClicked()
{
    UUISoundManager::PlayCancelClick(this);
    HidePasswordInput();
}

void URoomPasswordWidget::OnNumberClicked(int32 Number)
{
    UUISoundManager::PlayButtonClick(this);
    if (CurrentPassword.Len() < MaxPasswordLength)
    {
        CurrentPassword += FString::Printf(TEXT("%d"), Number);
        UpdatePasswordDisplay();
    }
}

void URoomPasswordWidget::OnNumberClicked0() { OnNumberClicked(0); }
void URoomPasswordWidget::OnNumberClicked1() { OnNumberClicked(1); }
void URoomPasswordWidget::OnNumberClicked2() { OnNumberClicked(2); }
void URoomPasswordWidget::OnNumberClicked3() { OnNumberClicked(3); }
void URoomPasswordWidget::OnNumberClicked4() { OnNumberClicked(4); }
void URoomPasswordWidget::OnNumberClicked5() { OnNumberClicked(5); }
void URoomPasswordWidget::OnNumberClicked6() { OnNumberClicked(6); }
void URoomPasswordWidget::OnNumberClicked7() { OnNumberClicked(7); }
void URoomPasswordWidget::OnNumberClicked8() { OnNumberClicked(8); }
void URoomPasswordWidget::OnNumberClicked9() { OnNumberClicked(9); }

void URoomPasswordWidget::OnDeleteClicked()
{
    UUISoundManager::PlayButtonClick(this);
    if (!CurrentPassword.IsEmpty())
    {
        CurrentPassword.LeftChopInline(1);
        UpdatePasswordDisplay();
    }
}

void URoomPasswordWidget::OnClearClicked()
{
    UUISoundManager::PlayButtonClick(this);
    CurrentPassword = TEXT("");
    UpdatePasswordDisplay();
}

void URoomPasswordWidget::UpdatePasswordDisplay()
{
    if (PasswordDisplayText)
    {
        FString Display = TEXT("");
        for (int32 i = 0; i < CurrentPassword.Len(); i++)
            Display += TEXT("*");
        for (int32 i = CurrentPassword.Len(); i < MaxPasswordLength; i++)
            Display += TEXT("-");

        PasswordDisplayText->SetText(FText::FromString(Display));
    }
}

bool URoomPasswordWidget::ValidatePasswordLength() const
{
    return CurrentPassword.Len() == MaxPasswordLength;
}
