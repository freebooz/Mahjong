#include "Gameplay/UI/Mahjong/ReconnectionWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/UI/Common/UISoundManager.h"

UReconnectionWidget::UReconnectionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CurrentState = EReconnectionState::Disconnected;
    CurrentAttempt = 0;
    MaxAttempts = 5;
    RotationAngle = 0.0f;
}

void UReconnectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RetryButton)
        RetryButton->OnClicked.AddDynamic(this, &UReconnectionWidget::OnRetryClicked);

    if (ReturnToLobbyButton)
        ReturnToLobbyButton->OnClicked.AddDynamic(this, &UReconnectionWidget::OnReturnToLobbyClicked);

    HideReconnection();
}

void UReconnectionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (CurrentState == EReconnectionState::Reconnecting)
    {
        RotationAngle += InDeltaTime * 360.0f;
        if (RotationAngle >= 360.0f)
            RotationAngle -= 360.0f;

        if (LoadingIcon)
            LoadingIcon->SetRenderTransformAngle(RotationAngle);
    }
}

void UReconnectionWidget::ShowDisconnected(const FString& Reason)
{
    CurrentState = EReconnectionState::Disconnected;

    if (StatusTitleText)
        StatusTitleText->SetText(FText::FromString(TEXT("连接断开")));

    if (StatusDetailText)
        StatusDetailText->SetText(FText::FromString(Reason));

    if (ProgressText)
        ProgressText->SetVisibility(ESlateVisibility::Collapsed);

    if (ReconnectProgressBar)
        ReconnectProgressBar->SetVisibility(ESlateVisibility::Collapsed);

    if (RetryButton)
        RetryButton->SetVisibility(ESlateVisibility::Visible);

    if (ReturnToLobbyButton)
        ReturnToLobbyButton->SetVisibility(ESlateVisibility::Visible);

    UUISoundManager::PlayErrorSound(this);
    Show();
}

void UReconnectionWidget::ShowReconnecting(int32 AttemptNumber, int32 InMaxAttempts)
{
    CurrentState = EReconnectionState::Reconnecting;
    CurrentAttempt = AttemptNumber;
    MaxAttempts = InMaxAttempts;

    if (StatusTitleText)
        StatusTitleText->SetText(FText::FromString(TEXT("正在重连...")));

    if (StatusDetailText)
        StatusDetailText->SetText(FText::FromString(FString::Printf(TEXT("第 %d 次重连中"), AttemptNumber)));

    if (ProgressText)
    {
        ProgressText->SetVisibility(ESlateVisibility::Visible);
        ProgressText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), AttemptNumber, MaxAttempts)));
    }

    if (ReconnectProgressBar)
        ReconnectProgressBar->SetVisibility(ESlateVisibility::Visible);

    if (RetryButton)
        RetryButton->SetVisibility(ESlateVisibility::Collapsed);

    if (ReturnToLobbyButton)
        ReturnToLobbyButton->SetVisibility(ESlateVisibility::Collapsed);

    Show();
    UpdateProgressDisplay();
}

void UReconnectionWidget::ShowReconnected()
{
    CurrentState = EReconnectionState::Reconnected;

    if (StatusTitleText)
        StatusTitleText->SetText(FText::FromString(TEXT("重连成功")));

    if (StatusDetailText)
        StatusDetailText->SetText(FText::FromString(TEXT("正在恢复游戏...")));

    if (ProgressText)
        ProgressText->SetVisibility(ESlateVisibility::Collapsed);

    if (ReconnectProgressBar)
        ReconnectProgressBar->SetVisibility(ESlateVisibility::Collapsed);

    if (RetryButton)
        RetryButton->SetVisibility(ESlateVisibility::Collapsed);

    if (ReturnToLobbyButton)
        ReturnToLobbyButton->SetVisibility(ESlateVisibility::Collapsed);

    UUISoundManager::PlaySuccessSound(this);
    Show();

    ReconnectionCallback.ExecuteIfBound(true);
}

void UReconnectionWidget::ShowReconnectionFailed(const FString& ErrorMessage)
{
    CurrentState = EReconnectionState::Failed;

    if (StatusTitleText)
        StatusTitleText->SetText(FText::FromString(TEXT("重连失败")));

    if (StatusDetailText)
        StatusDetailText->SetText(FText::FromString(ErrorMessage));

    if (ProgressText)
        ProgressText->SetVisibility(ESlateVisibility::Collapsed);

    if (ReconnectProgressBar)
        ReconnectProgressBar->SetVisibility(ESlateVisibility::Collapsed);

    if (RetryButton)
        RetryButton->SetVisibility(ESlateVisibility::Visible);

    if (ReturnToLobbyButton)
        ReturnToLobbyButton->SetVisibility(ESlateVisibility::Visible);

    Show();
}

void UReconnectionWidget::HideReconnection()
{
    Hide();
    RotationAngle = 0.0f;
}

void UReconnectionWidget::SetReconnectionCallback(FOnReconnectionResult Callback)
{
    ReconnectionCallback = Callback;
}

UReconnectionWidget::EReconnectionState UReconnectionWidget::GetCurrentState() const
{
    return CurrentState;
}

bool UReconnectionWidget::IsReconnecting() const
{
    return CurrentState == EReconnectionState::Reconnecting;
}

void UReconnectionWidget::OnRetryClicked()
{
    UUISoundManager::PlayButtonClick(this);
    ReconnectionCallback.ExecuteIfBound(false);
}

void UReconnectionWidget::OnReturnToLobbyClicked()
{
    UUISoundManager::PlayCancelClick(this);
    HideReconnection();
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}

void UReconnectionWidget::UpdateProgressDisplay()
{
    if (ReconnectProgressBar && MaxAttempts > 0)
    {
        ReconnectProgressBar->SetPercent(static_cast<float>(CurrentAttempt) / static_cast<float>(MaxAttempts));
    }
}
