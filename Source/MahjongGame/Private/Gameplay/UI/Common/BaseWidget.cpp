#include "Gameplay/UI/Common/BaseWidget.h"

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
