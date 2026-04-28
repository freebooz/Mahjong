#include "Gameplay/UI/Common/ConfirmDialogWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Gameplay/UI/Common/UISoundManager.h"

UConfirmDialogWidget::UConfirmDialogWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UConfirmDialogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ConfirmButton)
        ConfirmButton->OnClicked.AddDynamic(this, &UConfirmDialogWidget::OnConfirmClicked);

    if (CancelButton)
        CancelButton->OnClicked.AddDynamic(this, &UConfirmDialogWidget::OnCancelClicked);

    if (BackgroundOverlay)
        BackgroundOverlay->OnClicked.AddDynamic(this, &UConfirmDialogWidget::OnBackgroundClicked);

    HideDialog();
}

void UConfirmDialogWidget::ShowConfirmDialog(
    const FString& Title,
    const FString& Message,
    const FString& ConfirmText,
    const FString& CancelText)
{
    if (TitleText)
        TitleText->SetText(FText::FromString(Title));

    if (MessageText)
        MessageText->SetText(FText::FromString(Message));

    Show();
}

void UConfirmDialogWidget::HideDialog()
{
    Hide();
}

void UConfirmDialogWidget::SetConfirmCallback(FOnConfirmDelegate Callback)
{
    ConfirmCallback = Callback;
}

void UConfirmDialogWidget::SetCancelCallback(FOnCancelDelegate Callback)
{
    CancelCallback = Callback;
}

void UConfirmDialogWidget::OnConfirmClicked()
{
    UUISoundManager::PlayConfirmClick(this);
    ConfirmCallback.ExecuteIfBound();
    HideDialog();
}

void UConfirmDialogWidget::OnCancelClicked()
{
    UUISoundManager::PlayCancelClick(this);
    CancelCallback.ExecuteIfBound();
    HideDialog();
}

void UConfirmDialogWidget::OnBackgroundClicked()
{
    UUISoundManager::PlayCancelClick(this);
    CancelCallback.ExecuteIfBound();
    HideDialog();
}
