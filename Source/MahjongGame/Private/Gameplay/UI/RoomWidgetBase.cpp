#include "Gameplay/UI/RoomWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MahjongGameInstance.h"
URoomWidgetBase::URoomWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
void URoomWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    if (LeaveButton)
        LeaveButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnLeaveClicked);
    if (StartGameButton)
        StartGameButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnStartGameClicked);
    if (DrawTileButton)
        DrawTileButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDrawTileClicked);
    if (DiscardButton)
        DiscardButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDiscardTileClicked);
    if (ChowButton)
        ChowButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDrawTileClicked);
    if (PongButton)
        PongButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDrawTileClicked);
    if (KongButton)
        KongButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDrawTileClicked);
    if (HuButton)
        HuButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDrawTileClicked);

    // 使用默认值，等待实际房间数据更新
    UpdateRoomInfo(0, 0, 4);
    SetGameStatus(TEXT("等待玩家加入..."));
}
void URoomWidgetBase::OnLeaveClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] Leave clicked"));
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}
void URoomWidgetBase::OnStartGameClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] Start game clicked"));
    SetGameStatus(TEXT("游戏开始!"));
}
void URoomWidgetBase::OnDrawTileClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] Draw tile clicked"));
}
void URoomWidgetBase::OnDiscardTileClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] Discard tile clicked"));
}
void URoomWidgetBase::UpdateRoomInfo(int32 RoomID, int32 CurrentPlayers, int32 MaxPlayers)
{
    if (RoomIDText)
        RoomIDText->SetText(FText::FromString(FString::Printf(TEXT("房间 #%d"), RoomID)));
    if (PlayerCountText)
        PlayerCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d 玩家"), CurrentPlayers, MaxPlayers)));
}
void URoomWidgetBase::UpdateTurnInfo(int32 CurrentPlayer, bool bIsMyTurn)
{
    if (TurnInfoText)
    {
        if (bIsMyTurn)
            TurnInfoText->SetText(FText::FromString(TEXT("轮到你了!")));
        else
            TurnInfoText->SetText(FText::FromString(FString::Printf(TEXT("玩家 %d 的回合"), CurrentPlayer)));
    }
}
void URoomWidgetBase::ShowActionButtons(bool bShowDraw, bool bShowDiscard, bool bShowChow, bool bShowPong, bool bShowKong, bool bShowHu)
{
    if (DrawTileButton)
        DrawTileButton->SetVisibility(bShowDraw ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (DiscardButton)
        DiscardButton->SetVisibility(bShowDiscard ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (ChowButton)
        ChowButton->SetVisibility(bShowChow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (PongButton)
        PongButton->SetVisibility(bShowPong ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (KongButton)
        KongButton->SetVisibility(bShowKong ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    if (HuButton)
        HuButton->SetVisibility(bShowHu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
void URoomWidgetBase::SetGameStatus(const FString& Status)
{
    if (StatusText)
        StatusText->SetText(FText::FromString(Status));
}