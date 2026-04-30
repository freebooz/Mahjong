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
    SelfPlayerID = 1; // 默认自己为玩家1
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
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 初始化游戏界面"));

    // 初始化时隐藏所有玩家的高亮状态
    SetCurrentTurnPlayer(0);

    // 初始化弃牌区
    if (DiscardTiles)
        DiscardTiles->ClearAllDiscards();

    // 初始化手牌 - 传空数组
    if (SelfHandTiles)
        SelfHandTiles->SetHandTiles(TArray<FMahjongTile>());

    // 隐藏动作提示
    HideActionPrompt();

    // 设置默认房间信息
    if (RoomIDText)
        RoomIDText->SetText(FText::FromString(TEXT("房间: ---")));

    if (RoundText)
        RoundText->SetText(FText::FromString(TEXT("第 0/8 局")));
}

void UMahjongGameWidget::UpdateHandTiles(const TArray<FMahjongTile>& Tiles)
{
    if (SelfHandTiles)
        SelfHandTiles->SetHandTiles(Tiles);
    else
        UE_LOG(LogTemp, Warning, TEXT("[MahjongGameWidget] SelfHandTiles is null"));
}

void UMahjongGameWidget::UpdateDiscardTiles()
{
    if (DiscardTiles)
        DiscardTiles->ClearAllDiscards();
    else
        UE_LOG(LogTemp, Warning, TEXT("[MahjongGameWidget] DiscardTiles is null"));
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
        default:
            UE_LOG(LogTemp, Warning, TEXT("[MahjongGameWidget] Invalid PlayerID: %d"), PlayerID);
            return;
    }

    if (PlayerWidget)
        PlayerWidget->SetPlayerInfo(Info);
    else
        UE_LOG(LogTemp, Warning, TEXT("[MahjongGameWidget] PlayerWidget for ID %d is null"), PlayerID);
}

void UMahjongGameWidget::ShowActionPrompt(const FAvailableGameActions& Actions, float Countdown)
{
    if (ActionPrompt)
        ActionPrompt->ShowActions(Actions, Countdown);
    else
        UE_LOG(LogTemp, Warning, TEXT("[MahjongGameWidget] ActionPrompt is null"));
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
    else
        UE_LOG(LogTemp, Warning, TEXT("[MahjongGameWidget] TurnCountdown is null"));
}

void UMahjongGameWidget::SetCurrentTurnPlayer(int32 PlayerID)
{
    CurrentTurnPlayerID = PlayerID;

    // 更新所有玩家的回合指示器
    if (Player1Info) Player1Info->SetTurnIndicator(PlayerID == 1);
    if (Player2Info) Player2Info->SetTurnIndicator(PlayerID == 2);
    if (Player3Info) Player3Info->SetTurnIndicator(PlayerID == 3);
    if (Player4Info) Player4Info->SetTurnIndicator(PlayerID == 4);

    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] Current turn: Player %d"), PlayerID);
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

    // TODO: 确认对话框
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}

void UMahjongGameWidget::OnMenuClicked()
{
    UUISoundManager::PlayButtonClick(this);
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 打开菜单"));
    // TODO: 显示暂停菜单
}

void UMahjongGameWidget::OnRulesClicked()
{
    UUISoundManager::PlayButtonClick(this);
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 打开规则"));
    // TODO: 显示规则界面
}
