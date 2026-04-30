#include "Core/UI/Common/ToastWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

UToastWidget::UToastWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UToastWidget::NativeConstruct()
{
    Super::NativeConstruct();
    HideToast();
}

void UToastWidget::ShowSuccess(const FString& Message, float Duration)
{
    ShowToast(Message, EToastType::Success, Duration);
}

void UToastWidget::ShowWarning(const FString& Message, float Duration)
{
    ShowToast(Message, EToastType::Warning, Duration);
}

void UToastWidget::ShowError(const FString& Message, float Duration)
{
    ShowToast(Message, EToastType::Error, Duration);
}

void UToastWidget::ShowInfo(const FString& Message, float Duration)
{
    ShowToast(Message, EToastType::Info, Duration);
}

void UToastWidget::HideToast()
{
    GetWorld()->GetTimerManager().ClearTimer(AutoHideTimerHandle);
    Hide();
}

void UToastWidget::ShowToast(const FString& Message, EToastType Type, float Duration)
{
    CurrentType = Type;

    if (MessageText)
        MessageText->SetText(FText::FromString(Message));

    // 根据类型设置背景纹理
    if (ToastBackground)
    {
        switch (Type)
        {
            case EToastType::Success:
                if (SuccessBackgroundTexture)
                    ToastBackground->SetBrushFromTexture(SuccessBackgroundTexture);
                break;
            case EToastType::Warning:
                if (WarningBackgroundTexture)
                    ToastBackground->SetBrushFromTexture(WarningBackgroundTexture);
                break;
            case EToastType::Error:
                if (ErrorBackgroundTexture)
                    ToastBackground->SetBrushFromTexture(ErrorBackgroundTexture);
                break;
            default:
                break;
        }
    }

    GetWorld()->GetTimerManager().ClearTimer(AutoHideTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(AutoHideTimerHandle, this, &UToastWidget::OnAutoHideTimer, Duration, false);

    Show();
}

void UToastWidget::OnAutoHideTimer()
{
    HideToast();
}

void UToastWidget::SetSuccessBackground(UTexture2D* Texture)
{
    SuccessBackgroundTexture = Texture;
}

void UToastWidget::SetWarningBackground(UTexture2D* Texture)
{
    WarningBackgroundTexture = Texture;
}

void UToastWidget::SetErrorBackground(UTexture2D* Texture)
{
    ErrorBackgroundTexture = Texture;
}
