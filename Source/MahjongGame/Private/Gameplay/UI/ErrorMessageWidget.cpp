#include "Gameplay/UI/ErrorMessageWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

//==============================================================================
// 错误信息界面实现
//==============================================================================
UErrorMessageWidget::UErrorMessageWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

//==============================================================================
// 界面构建完成
//==============================================================================
void UErrorMessageWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseButton)
        CloseButton->OnClicked.AddDynamic(this, &UErrorMessageWidget::OnCloseClicked);

    UE_LOG(LogTemp, Log, TEXT("[ErrorMessage] 错误消息界面已构建"));
}

//==============================================================================
// 显示错误消息
//==============================================================================
void UErrorMessageWidget::ShowError(const FString& ErrorMessage, float Duration)
{
    if (MessageText)
        MessageText->SetText(FText::FromString(ErrorMessage));

    CurrentErrorType = 0;

    if (ErrorIcon)
        ErrorIcon->SetVisibility(ESlateVisibility::Visible);

    if (WarningIcon)
        WarningIcon->SetVisibility(ESlateVisibility::Collapsed);

    if (Duration > 0)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(AutoHideTimerHandle);
            World->GetTimerManager().SetTimer(AutoHideTimerHandle, this, &UErrorMessageWidget::HideError, Duration, false);
        }
    }

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Log, TEXT("[ErrorMessage] 显示错误: %s"), *ErrorMessage);
}

//==============================================================================
// 显示警告消息
//==============================================================================
void UErrorMessageWidget::ShowWarning(const FString& WarningMessage, float Duration)
{
    if (MessageText)
        MessageText->SetText(FText::FromString(WarningMessage));

    CurrentErrorType = 1;

    if (ErrorIcon)
        ErrorIcon->SetVisibility(ESlateVisibility::Collapsed);

    if (WarningIcon)
        WarningIcon->SetVisibility(ESlateVisibility::Visible);

    if (Duration > 0)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(AutoHideTimerHandle);
            World->GetTimerManager().SetTimer(AutoHideTimerHandle, this, &UErrorMessageWidget::HideError, Duration, false);
        }
    }

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Log, TEXT("[ErrorMessage] 显示警告: %s"), *WarningMessage);
}

//==============================================================================
// 显示网络错误
//==============================================================================
void UErrorMessageWidget::ShowNetworkError(const FString& CustomMessage)
{
    FString ErrorMsg = CustomMessage.IsEmpty() ? TEXT("网络连接已断开，请检查网络设置") : CustomMessage;
    ShowError(ErrorMsg, 0);
}

//==============================================================================
// 显示验证错误
//==============================================================================
void UErrorMessageWidget::ShowValidationError(const FString& FieldName, const FString& ErrorMessage)
{
    FString FullMessage = FieldName + TEXT(": ") + ErrorMessage;
    ShowError(FullMessage, 5.0f);
}

//==============================================================================
// 隐藏错误消息
//==============================================================================
void UErrorMessageWidget::HideError()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(AutoHideTimerHandle);
    }

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[ErrorMessage] 隐藏错误消息"));
}

//==============================================================================
// 关闭按钮点击
//==============================================================================
void UErrorMessageWidget::OnCloseClicked()
{
    HideError();
}
