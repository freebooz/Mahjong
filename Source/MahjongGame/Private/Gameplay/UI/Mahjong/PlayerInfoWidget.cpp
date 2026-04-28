#include "Gameplay/UI/Mahjong/PlayerInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

UPlayerInfoWidget::UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CurrentPosition = EPlayerPosition::Self;
}

void UPlayerInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayerInfoWidget::SetPlayerInfo(const FPlayerInfo& Info)
{
    PlayerData = Info;

    if (NicknameText)
        NicknameText->SetText(FText::FromString(Info.Nickname));

    if (LevelText)
        LevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), Info.PlayerLevel)));

    if (ScoreText)
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Info.Score)));

    if (ReadyText)
        ReadyText->SetVisibility(Info.bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (OfflineOverlay)
        OfflineOverlay->SetVisibility(Info.bIsOnline ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

    if (BankerIcon)
        BankerIcon->SetVisibility(Info.bIsHost ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

FPlayerInfo UPlayerInfoWidget::GetPlayerInfo() const
{
    return PlayerData;
}

void UPlayerInfoWidget::SetPlayerPosition(EPlayerPosition Position)
{
    CurrentPosition = Position;
}

void UPlayerInfoWidget::SetTurnIndicator(bool bIsMyTurn)
{
    if (TurnIndicator)
        TurnIndicator->SetVisibility(bIsMyTurn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UPlayerInfoWidget::SetReadyStatus(bool bIsReady)
{
    if (ReadyText)
        ReadyText->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UPlayerInfoWidget::SetOnlineStatus(bool bIsOnline)
{
    if (OfflineOverlay)
        OfflineOverlay->SetVisibility(bIsOnline ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UPlayerInfoWidget::UpdateScore(int32 NewScore)
{
    PlayerData.Score = NewScore;
    if (ScoreText)
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewScore)));
}

void UPlayerInfoWidget::AddScore(int32 Delta)
{
    PlayerData.Score += Delta;
    if (ScoreText)
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerData.Score)));
}
