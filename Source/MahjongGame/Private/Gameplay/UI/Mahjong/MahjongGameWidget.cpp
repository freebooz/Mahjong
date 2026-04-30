#include "Gameplay/UI/Mahjong/MahjongGameWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Core/UI/Common/UISoundManager.h"

UMahjongGameWidget::UMahjongGameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CurrentTurnPlayerID = 0;
    SelfPlayerID = 0;
}

void UMahjongGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LeaveButton)
        LeaveButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnLeaveClicked);

    if (MenuButton)
        MenuButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnMenuClicked);

    if (RulesButton)
        RulesButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnRulesClicked);

    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 麻将游戏界面已构建"));
}

void UMahjongGameWidget::InitializeGame()
{
}

void UMahjongGameWidget::UpdateHandTiles(const TArray<FMahjongTile>& Tiles)
{
    if (SelfHandTiles)
        SelfHandTiles->SetHandTiles(Tiles);
}

void UMahjongGameWidget::UpdateDiscardTiles()
{
}

void UMahjongGameWidget::UpdatePlayerInfo(int32 PlayerID, const FPlayerInfoData& Info)
{
    UPlayerInfoWidget* PlayerWidget = nullptr;

    switch (PlayerID)
    {
        case 1: PlayerWidget = Player1Info; break;
        case 2: PlayerWidget = Player2Info; break;
        case 3: PlayerWidget = Player3Info; break;
        case 4: PlayerWidget = Player4Info; break;
    }

    if (PlayerWidget)
        PlayerWidget->SetPlayerInfo(Info);
}

void UMahjongGameWidget::ShowActionPrompt(const FAvailableGameActions& Actions, float Countdown)
{
    if (ActionPrompt)
        ActionPrompt->ShowActions(Actions, Countdown);
}

void UMahjongGameWidget::HideActionPrompt()
{
    if (ActionPrompt)
        ActionPrompt->HideActions();
}

void UMahjongGameWidget::ShowTurnCountdown(float Duration)
{
    if (TurnCountdown)
        TurnCountdown->StartCountdown(Duration);
}

void UMahjongGameWidget::SetCurrentTurnPlayer(int32 PlayerID)
{
    CurrentTurnPlayerID = PlayerID;

    if (Player1Info) Player1Info->SetTurnIndicator(PlayerID == 1);
    if (Player2Info) Player2Info->SetTurnIndicator(PlayerID == 2);
    if (Player3Info) Player3Info->SetTurnIndicator(PlayerID == 3);
    if (Player4Info) Player4Info->SetTurnIndicator(PlayerID == 4);
}

int32 UMahjongGameWidget::GetCurrentTurnPlayer() const
{
    return CurrentTurnPlayerID;
}

bool UMahjongGameWidget::IsMyTurn() const
{
    return CurrentTurnPlayerID == SelfPlayerID;
}

int32 UMahjongGameWidget::GetSelfPlayerID() const
{
    return SelfPlayerID;
}

void UMahjongGameWidget::SetTableBackground(UTexture2D* NewTexture)
{
    if (TableBackgroundImage && NewTexture)
    {
        TableBackgroundImage->SetBrushFromTexture(NewTexture);
        TableBackgroundImage->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMahjongGameWidget::OnLeaveClicked()
{
    UUISoundManager::PlayCancelClick(this);
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 离开房间"));
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}

void UMahjongGameWidget::OnMenuClicked()
{
    UUISoundManager::PlayButtonClick(this);
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 打开菜单"));
}

void UMahjongGameWidget::OnRulesClicked()
{
    UUISoundManager::PlayButtonClick(this);
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 打开规则"));
}
