#include "Core/UI/Common/LoadingWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

ULoadingWidget::ULoadingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    RotationAngle = 0.0f;
    bShowProgress = false;
}

void ULoadingWidget::NativeConstruct()
{
    Super::NativeConstruct();
    HideLoading();
}

void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (IsVisible())
    {
        RotationAngle += InDeltaTime * 360.0f;
        if (RotationAngle >= 360.0f)
            RotationAngle -= 360.0f;

        if (LoadingIcon)
        {
            LoadingIcon->SetRenderTransformAngle(RotationAngle);
        }
    }
}

void ULoadingWidget::ShowLoading(const FString& Message)
{
    bShowProgress = false;
    SetLoadingText(Message);
    Show();
}

void ULoadingWidget::ShowProgress(float Progress, const FString& Message)
{
    bShowProgress = true;
    if (!Message.IsEmpty())
        SetLoadingText(Message);
    Show();
}

void ULoadingWidget::HideLoading()
{
    Hide();
    RotationAngle = 0.0f;
}

void ULoadingWidget::SetLoadingText(const FString& Message)
{
    if (LoadingText)
        LoadingText->SetText(FText::FromString(Message));
}
