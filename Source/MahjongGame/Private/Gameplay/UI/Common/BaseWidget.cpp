#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

void UBaseWidget::Show()
{
    bIsShowing = true;
    SetVisibility(ESlateVisibility::Visible);
}

void UBaseWidget::Hide()
{
    bIsShowing = false;
    SetVisibility(ESlateVisibility::Collapsed);
}

bool UBaseWidget::IsVisible() const
{
    return bIsShowing;
}

void UBaseWidget::PlayFadeIn(float Duration)
{
    if (Duration <= 0) Duration = 0.3f;
    Show();
    OnFadeInFinished();
}

void UBaseWidget::PlayFadeOut(float Duration)
{
    if (Duration <= 0) Duration = 0.3f;
    OnFadeOutFinished();
    Hide();
}

void UBaseWidget::SetBackgroundTexture(UTexture2D* NewTexture)
{
    if (BackgroundImage && NewTexture)
    {
        BackgroundImage->SetBrushFromTexture(NewTexture);
        BackgroundImage->SetVisibility(ESlateVisibility::Visible);
    }
}

void UBaseWidget::SetBackgroundColor(FLinearColor NewColor)
{
    if (BackgroundImage)
    {
        FSlateBrush Brush = BackgroundImage->GetBrush();
        Brush.TintColor = NewColor;
        BackgroundImage->SetBrush(Brush);
    }
}

void UBaseWidget::SetBackgroundOpacity(float Opacity)
{
    if (BackgroundImage)
    {
        FSlateBrush Brush = BackgroundImage->GetBrush();
        Brush.TintColor.A = Opacity;
        BackgroundImage->SetBrush(Brush);
    }
}
