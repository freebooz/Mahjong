#include "Gameplay/UI/RoomWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MahjongGameInstance.h"

//==============================================================================
// 构造函数
//==============================================================================
URoomWidgetBase::URoomWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

//==============================================================================
// 界面构建完成
// 绑定所有按钮点击事件
//==============================================================================
void URoomWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    // 绑定房间操作按钮
    if (LeaveButton)
        LeaveButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnLeaveClicked);

    if (StartGameButton)
        StartGameButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnStartGameClicked);

    // 绑定游戏操作按钮
    if (DrawTileButton)
        DrawTileButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDrawTileClicked);

    if (DiscardButton)
        DiscardButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnDiscardTileClicked);

    // 绑定动作按钮（吃、碰、杠、胡）
    if (ChowButton)
        ChowButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnChowClicked);

    if (PongButton)
        PongButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnPongClicked);

    if (KongButton)
        KongButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnKongClicked);

    if (HuButton)
        HuButton->OnClicked.AddDynamic(this, &URoomWidgetBase::OnHuClicked);

    // 使用默认值，等待实际房间数据更新
    UpdateRoomInfo(0, 0, 4);
    SetGameStatus(TEXT("等待玩家加入..."));
}

//==============================================================================
// 离开房间
//==============================================================================
void URoomWidgetBase::OnLeaveClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 离开房间"));
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}

//==============================================================================
// 开始游戏
//==============================================================================
void URoomWidgetBase::OnStartGameClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 开始游戏"));
    SetGameStatus(TEXT("游戏开始!"));
}

//==============================================================================
// 摸牌
//==============================================================================
void URoomWidgetBase::OnDrawTileClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 摸牌"));
}

//==============================================================================
// 出牌
//==============================================================================
void URoomWidgetBase::OnDiscardTileClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 出牌"));
}

//==============================================================================
// 吃牌
//==============================================================================
void URoomWidgetBase::OnChowClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 吃牌"));
}

//==============================================================================
// 碰牌
//==============================================================================
void URoomWidgetBase::OnPongClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 碰牌"));
}

//==============================================================================
// 杠牌
//==============================================================================
void URoomWidgetBase::OnKongClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 杠牌"));
}

//==============================================================================
// 胡牌
//==============================================================================
void URoomWidgetBase::OnHuClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidgetBase] 胡牌"));
}

//==============================================================================
// 更新房间信息显示
//==============================================================================
void URoomWidgetBase::UpdateRoomInfo(int32 RoomID, int32 CurrentPlayers, int32 MaxPlayers)
{
    if (RoomIDText)
        RoomIDText->SetText(FText::FromString(FString::Printf(TEXT("房间 #%d"), RoomID)));

    if (PlayerCountText)
        PlayerCountText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d 玩家"), CurrentPlayers, MaxPlayers)));
}

//==============================================================================
// 更新回合信息显示
//==============================================================================
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

//==============================================================================
// 显示/隐藏动作按钮
//==============================================================================
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

//==============================================================================
// 设置游戏状态文本
//==============================================================================
void URoomWidgetBase::SetGameStatus(const FString& Status)
{
    if (StatusText)
        StatusText->SetText(FText::FromString(Status));
}