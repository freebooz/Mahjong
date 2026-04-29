#include "Gameplay/UI/GameWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/MahjongGameInstance.h"

UGameWidgetBase::UGameWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGameWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    if (CreateRoomButton)
        CreateRoomButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnCreateRoomClicked);
    if (JoinRoomButton)
        JoinRoomButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnJoinRoomClicked);
    if (LeaveButton)
        LeaveButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnLeaveClicked);
    if (DrawTileButton)
        DrawTileButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnDrawTileClicked);
    if (DiscardButton)
        DiscardButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnDiscardTileClicked);
    if (ChowButton)
        ChowButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnChowClicked);
    if (PongButton)
        PongButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnPongClicked);
    if (KongButton)
        KongButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnKongClicked);
    if (HuButton)
        HuButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnHuClicked);
    if (ZhuaJiButton)
        ZhuaJiButton->OnClicked.AddDynamic(this, &UGameWidgetBase::OnZhuaJiClicked);
    if (RoomDialogPanel)
        RoomDialogPanel->SetVisibility(ESlateVisibility::Collapsed);
    if (TurnInfoText)
        TurnInfoText->SetText(FText::FromString(TEXT("")));
    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("")));
    if (ErrorText)
        ErrorText->SetText(FText::FromString(TEXT("")));
}

void UGameWidgetBase::ShowCreateRoomDialog()
{
    if (RoomDialogPanel)
        RoomDialogPanel->SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] ShowCreateRoomDialog"));
}

void UGameWidgetBase::ShowJoinRoomDialog()
{
    if (RoomDialogPanel)
        RoomDialogPanel->SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] ShowJoinRoomDialog"));
}

void UGameWidgetBase::HideRoomDialog()
{
    if (RoomDialogPanel)
        RoomDialogPanel->SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] HideRoomDialog"));
}

void UGameWidgetBase::SetRoomInfo(int32 RoomID, const FString& RoomName, const FString& Password, int32 PlayerCount, int32 MaxPlayers)
{
    if (RoomIDText)
        RoomIDText->SetText(FText::FromString(FString::Printf(TEXT("房间 #%d"), RoomID)));
    if (RoomNameText)
        RoomNameText->SetText(FText::FromString(RoomName));
    if (PlayerCountText)
        PlayerCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d 玩家"), PlayerCount, MaxPlayers)));
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] SetRoomInfo: RoomID=%d, RoomName=%s, Players=%d/%d"), RoomID, *RoomName, PlayerCount, MaxPlayers);
}

void UGameWidgetBase::UpdatePlayerList(const TArray<FPlayerInfo>& Players)
{
    if (!PlayerListBox) return;
    PlayerListBox->ClearChildren();
    for (const FPlayerInfo& Player : Players)
    {
        UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] Player: Index=%d, Name=%s, Score=%d, Ready=%s, Online=%s"),
            Player.PlayerIndex, *Player.PlayerName, Player.Score,
            Player.bIsReady ? TEXT("true") : TEXT("false"),
            Player.bIsOnline ? TEXT("true") : TEXT("false"));
    }
}

void UGameWidgetBase::UpdateMyInfo(const FPlayerInfo& MyInfo)
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] UpdateMyInfo: Index=%d, Name=%s"), MyInfo.PlayerIndex, *MyInfo.PlayerName);
}

void UGameWidgetBase::ShowMyHandTiles(const TArray<int32>& TileIDs)
{
    if (!HandTilesBox) return;
    HandTilesBox->ClearChildren();
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] ShowMyHandTiles: Count=%d"), TileIDs.Num());
}

void UGameWidgetBase::OnTileClicked(int32 TileIndex)
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnTileClicked: Index=%d"), TileIndex);
}

void UGameWidgetBase::SetSelectedTile(int32 TileIndex)
{
    SelectedTileIndex = TileIndex;
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] SetSelectedTile: Index=%d"), TileIndex);
}

void UGameWidgetBase::ShowActionButtons(bool bShowChow, bool bShowPong, bool bShowKong, bool bShowHu, bool bShowZhuaJi)
{
    if (ChowButton)
        ChowButton->SetVisibility(bShowChow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (PongButton)
        PongButton->SetVisibility(bShowPong ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (KongButton)
        KongButton->SetVisibility(bShowKong ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (HuButton)
        HuButton->SetVisibility(bShowHu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (ZhuaJiButton)
        ZhuaJiButton->SetVisibility(bShowZhuaJi ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] ShowActionButtons: Chow=%d, Pong=%d, Kong=%d, Hu=%d, ZhuaJi=%d"),
        bShowChow, bShowPong, bShowKong, bShowHu, bShowZhuaJi);
}

void UGameWidgetBase::HideAllActionButtons()
{
    if (ChowButton) ChowButton->SetVisibility(ESlateVisibility::Collapsed);
    if (PongButton) PongButton->SetVisibility(ESlateVisibility::Collapsed);
    if (KongButton) KongButton->SetVisibility(ESlateVisibility::Collapsed);
    if (HuButton) HuButton->SetVisibility(ESlateVisibility::Collapsed);
    if (ZhuaJiButton) ZhuaJiButton->SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] HideAllActionButtons"));
}

void UGameWidgetBase::SetCurrentTurnPlayer(int32 PlayerIndex)
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] SetCurrentTurnPlayer: PlayerIndex=%d"), PlayerIndex);
}

void UGameWidgetBase::SetMyTurn(bool bIsMyTurn)
{
    if (TurnInfoText)
    {
        if (bIsMyTurn)
            TurnInfoText->SetText(FText::FromString(TEXT("轮到你了!")));
        else
            TurnInfoText->SetText(FText::FromString(TEXT("等待其他玩家...")));
    }
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] SetMyTurn: %s"), bIsMyTurn ? TEXT("true") : TEXT("false"));
}

void UGameWidgetBase::ShowDrawButton(bool bShow)
{
    if (DrawTileButton)
        DrawTileButton->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] ShowDrawButton: %s"), bShow ? TEXT("true") : TEXT("false"));
}

void UGameWidgetBase::ShowDiscardButton(bool bShow)
{
    if (DiscardButton)
        DiscardButton->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] ShowDiscardButton: %s"), bShow ? TEXT("true") : TEXT("false"));
}

void UGameWidgetBase::AddDiscardedTileToPool(int32 TileID)
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] AddDiscardedTileToPool: TileID=%d"), TileID);
}

void UGameWidgetBase::ShowDiscardPool(const TArray<int32>& TileIDs)
{
    if (!DiscardPoolBox) return;
    DiscardPoolBox->ClearChildren();
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] ShowDiscardPool: Count=%d"), TileIDs.Num());
}

void UGameWidgetBase::ShowStatusMessage(const FString& Message)
{
    if (StatusText)
        StatusText->SetText(FText::FromString(Message));
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] Status: %s"), *Message);
}

void UGameWidgetBase::ShowErrorMessage(const FString& Error)
{
    if (ErrorText)
        ErrorText->SetText(FText::FromString(Error));
    UE_LOG(LogTemp, Error, TEXT("[GameWidgetBase] Error: %s"), *Error);
}

void UGameWidgetBase::OnCreateRoomClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnCreateRoomClicked"));
    ShowCreateRoomDialog();
}

void UGameWidgetBase::OnJoinRoomClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnJoinRoomClicked"));
    ShowJoinRoomDialog();
}

void UGameWidgetBase::OnLeaveClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnLeaveClicked"));
    HideRoomDialog();
}

void UGameWidgetBase::OnDrawTileClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnDrawTileClicked"));
}

void UGameWidgetBase::OnDiscardTileClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnDiscardTileClicked"));
}

void UGameWidgetBase::OnChowClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnChowClicked"));
}

void UGameWidgetBase::OnPongClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnPongClicked"));
}

void UGameWidgetBase::OnKongClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnKongClicked"));
}

void UGameWidgetBase::OnHuClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnHuClicked"));
}

void UGameWidgetBase::OnZhuaJiClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameWidgetBase] OnZhuaJiClicked"));
}