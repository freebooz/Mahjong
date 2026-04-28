//==============================================================================
// 贵阳捉鸡麻将 - 游戏主界面实现
// MahjongGameWidget.cpp
//
// 本文件实现游戏主界面的所有函数
// 负责手牌显示、动作按钮、回合指示、倒计时等UI逻辑
//==============================================================================

#include "Gameplay/UI/MahjongGameWidget.h"
#include "Gameplay/Game/MahjongPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/UniformGridPanel.h"
#include "Components/ScrollBox.h"

//==============================================================================
// 构造函数
// 初始化界面组件的默认值
//==============================================================================
UMahjongGameWidget::UMahjongGameWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , SelectedTileIndex(-1)       // 初始没有选中任何牌
    , CurrentTurnPlayer(0)       // 当前回合玩家默认为0
    , CountdownTime(0.0f)        // 倒计时初始为0
    , bIsMyTurn(false)           // 初始不是自己的回合
    , ResultPanelIndex(1)         // 结果面板默认为MainSwitcher的索引1
{}

//==============================================================================
// NativeConstruct 函数
// 界面构建时调用，用于初始化按钮事件绑定等
//==============================================================================
void UMahjongGameWidget::NativeConstruct()
{
    // 调用父类的 NativeConstruct
    Super::NativeConstruct();

    // 绑定摸牌按钮点击事件
    if (DrawButton)
        DrawButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnDrawButtonClicked);

    // 绑定出牌按钮点击事件
    if (DiscardButton)
        DiscardButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnDiscardButtonClicked);

    // 绑定碰按钮点击事件
    if (PengButton)
        PengButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnPengButtonClicked);

    // 绑定杠按钮点击事件
    if (GangButton)
        GangButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnGangButtonClicked);

    // 绑定胡按钮点击事件
    if (HuButton)
        HuButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnHuButtonClicked);

    // 绑定过按钮点击事件
    if (GuoButton)
        GuoButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnGuoButtonClicked);

    // 绑定报听按钮点击事件
    if (TingButton)
        TingButton->OnClicked.AddDynamic(this, &UMahjongGameWidget::OnTingButtonClicked);

    // 初始状态：隐藏所有动作按钮
    HideAllActionButtons();

    // 初始状态：隐藏摸牌和出牌按钮
    ShowDrawButton(false);
    ShowDiscardButton(false);
}

//==============================================================================
// NativeTick 函数
// 每帧调用，用于更新倒计时等实时元素
//==============================================================================
void UMahjongGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 调用父类的 NativeTick
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 如果倒计时大于0，更新倒计时显示
    if (CountdownTime > 0.0f && CountdownText)
    {
        // 减少倒计时
        CountdownTime -= InDeltaTime;

        // 更新显示（向上取整显示秒数）
        int32 Seconds = FMath::CeilToInt(CountdownTime);
        CountdownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Seconds)));

        // 倒计时结束时清空显示
        if (CountdownTime <= 0.0f)
            CountdownText->SetText(FText::FromString(TEXT("")));
    }
}

//==============================================================================
// 初始化Widget
// 将界面与玩家控制器关联
//==============================================================================
void UMahjongGameWidget::InitializeWidget(AMahjongPlayerController* Controller)
{
    // 保存控制器引用
    PlayerController = Controller;
}

//==============================================================================
// 更新手牌显示
// 刷新玩家手牌的显示
//==============================================================================
void UMahjongGameWidget::UpdateHandTiles(const TArray<int32>& TileIDs)
{
    // 清空现有手牌显示
    ClearHandTiles();

    // 检查手牌网格是否存在
    if (!HandTilesGrid) return;

    // 遍历所有手牌ID，创建对应的显示Widget
    for (int32 i = 0; i < TileIDs.Num(); ++i)
    {
        // 创建牌的Image Widget
        UImage* TileImage = NewObject<UImage>(this);
        if (TileImage)
        {
            // 设置牌的大小
            TileImage->SetDesiredSizeOverride(FVector2D(64.0f, 80.0f));

            // 根据牌值设置颜色（万子红色、条子绿色、筒子蓝色）
            FLinearColor TileColor = GetTileColor(TileIDs[i]);
            TileImage->SetColorAndOpacity(TileColor);

            // 保存到列表
            HandTileWidgets.Add(TileImage);

            // 添加到网格（每行10张牌）
            int32 Row = i / 10;
            int32 Col = i % 10;
            HandTilesGrid->AddChild(TileImage);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 手牌显示已更新，共 %d 张"), TileIDs.Num());
}

//==============================================================================
// 更新弃牌池显示
// 刷新所有玩家打出的牌的显示
//==============================================================================
void UMahjongGameWidget::UpdateDiscardPool(const TArray<int32>& TileIDs)
{
    // 清空现有弃牌显示
    ClearDiscardTiles();

    // 检查弃牌池网格是否存在
    if (!DiscardPoolGrid) return;

    // 遍历所有弃牌ID，创建对应的显示Widget
    for (int32 i = 0; i < TileIDs.Num(); ++i)
    {
        // 创建牌的Image Widget
        UImage* TileImage = NewObject<UImage>(this);
        if (TileImage)
        {
            // 设置牌的大小
            TileImage->SetDesiredSizeOverride(FVector2D(48.0f, 60.0f));  // 弃牌比手牌小

            // 根据牌值设置颜色
            FLinearColor TileColor = GetTileColor(TileIDs[i]);
            TileImage->SetColorAndOpacity(TileColor);

            // 保存到列表
            DiscardTileWidgets.Add(TileImage);

            // 添加到网格（每行12张牌）
            int32 Row = i / 12;
            int32 Col = i % 12;
            DiscardPoolGrid->AddChild(TileImage);
        }

        UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 弃牌[%d]: ID=%d"), i, TileIDs[i]);
    }

    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 弃牌池已更新，共 %d 张"), TileIDs.Num());
}

//==============================================================================
// 更新回合指示
// 显示当前是哪个玩家的回合
//==============================================================================
void UMahjongGameWidget::UpdateTurnIndicator(int32 CurrentPlayer, bool bIsMy)
{
    // 保存当前状态
    CurrentTurnPlayer = CurrentPlayer;
    bIsMyTurn = bIsMy;

    // 检查回合文本是否存在
    if (TurnText)
    {
        if (bIsMy)
            // 如果是自己的回合，显示醒目提示
            TurnText->SetText(FText::FromString(TEXT(">>> 你的回合 <<<")));
        else
            // 如果是其他玩家，显示玩家编号
            TurnText->SetText(FText::FromString(FString::Printf(TEXT("玩家 %d 的回合"), CurrentPlayer)));
    }
}

//==============================================================================
// 显示动作按钮
// 根据当前状态显示/隐藏各动作按钮
//==============================================================================
void UMahjongGameWidget::ShowActionButtons(const FGameActionUI& Actions)
{
    // 设置碰按钮可见性
    if (PengButton)
        PengButton->SetVisibility(Actions.bCanPeng ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    // 设置杠按钮可见性
    if (GangButton)
        GangButton->SetVisibility(Actions.bCanGang ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    // 设置胡按钮可见性
    if (HuButton)
        HuButton->SetVisibility(Actions.bCanHu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    // 设置过按钮可见性
    if (GuoButton)
        GuoButton->SetVisibility(Actions.bCanGuo ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    // 设置报听按钮可见性
    if (TingButton)
        TingButton->SetVisibility(Actions.bCanBaoTing ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

//==============================================================================
// 隐藏所有动作按钮
// 隐藏碰、杠、胡、过、报听等按钮
//==============================================================================
void UMahjongGameWidget::HideAllActionButtons()
{
    // 隐藏所有动作按钮
    if (PengButton) PengButton->SetVisibility(ESlateVisibility::Collapsed);
    if (GangButton) GangButton->SetVisibility(ESlateVisibility::Collapsed);
    if (HuButton) HuButton->SetVisibility(ESlateVisibility::Collapsed);
    if (GuoButton) GuoButton->SetVisibility(ESlateVisibility::Collapsed);
    if (TingButton) TingButton->SetVisibility(ESlateVisibility::Collapsed);
}

//==============================================================================
// 显示/隐藏摸牌按钮
//==============================================================================
void UMahjongGameWidget::ShowDrawButton(bool bShow)
{
    if (DrawButton)
        DrawButton->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

//==============================================================================
// 显示/隐藏出牌按钮
//==============================================================================
void UMahjongGameWidget::ShowDiscardButton(bool bShow)
{
    if (DiscardButton)
        DiscardButton->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

//==============================================================================
// 显示倒计时
// 显示玩家出牌的剩余时间
//==============================================================================
void UMahjongGameWidget::ShowCountdown(int32 Seconds)
{
    // 设置倒计时时间
    CountdownTime = static_cast<float>(Seconds);

    // 更新倒计时文本显示
    if (CountdownText)
        CountdownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Seconds)));
}

//==============================================================================
// 选择牌
// 选中后高亮显示该牌
//==============================================================================
void UMahjongGameWidget::SelectTile(int32 Index)
{
    // 保存选中的索引
    SelectedTileIndex = Index;

    // 高亮显示该牌
    HighlightTile(Index);
}

//==============================================================================
// 高亮选中的牌
// 将指定索引的牌高亮显示
//==============================================================================
void UMahjongGameWidget::HighlightTile(int32 Index)
{
    // 清除所有手牌的高亮状态
    for (int32 i = 0; i < HandTileWidgets.Num(); ++i)
    {
        if (HandTileWidgets[i])
        {
            // 恢复原始颜色
            HandTileWidgets[i]->SetColorAndOpacity(GetTileColor(HandTiles[i]));
            // 恢复原始大小
            HandTileWidgets[i]->SetRenderScale(FVector2D(1.0f, 1.0f));
        }
    }

    // 高亮选中的牌
    if (Index >= 0 && Index < HandTileWidgets.Num() && HandTileWidgets[Index])
    {
        // 设置高亮颜色（金色）
        HandTileWidgets[Index]->SetColorAndOpacity(FLinearColor::Yellow);
        // 放大1.1倍
        HandTileWidgets[Index]->SetRenderScale(FVector2D(1.1f, 1.1f));
        SelectedTileIndex = Index;
    }

    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 高亮牌: 索引=%d"), Index);
}

//==============================================================================
// 显示等待文字
// 在等待其他玩家操作时显示提示
//==============================================================================
void UMahjongGameWidget::ShowWaitingText(const FString& Text)
{
    if (WaitingText)
    {
        // 设置文本内容
        WaitingText->SetText(FText::FromString(Text));

        // 根据文本是否为空设置可见性
        WaitingText->SetVisibility(Text.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    }
}

//==============================================================================
// 显示结果对话框
// 显示胡牌结算或游戏结果
//==============================================================================
void UMahjongGameWidget::ShowResultDialog(const FString& Title, const FString& Message)
{
    // 使用ResultText显示结果（如果存在）
    if (ResultText)
    {
        FString FullMessage = Title + TEXT("\n") + Message;
        ResultText->SetText(FText::FromString(FullMessage));
        ResultText->SetVisibility(ESlateVisibility::Visible);
    }

    // 切换到结果面板（如果MainSwitcher可用，假设索引1是结果面板）
    if (MainSwitcher && ResultPanelIndex >= 0 && ResultPanelIndex < MainSwitcher->GetNumWidgets())
    {
        MainSwitcher->SetActiveWidgetIndex(ResultPanelIndex);
    }

    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 结果: %s - %s"), *Title, *Message);
}

//==============================================================================
// 设置牌堆数量
// 在界面上显示还剩多少张牌
//==============================================================================
void UMahjongGameWidget::SetTilePoolCount(int32 Count)
{
    if (TileCountText)
        TileCountText->SetText(FText::FromString(FString::Printf(TEXT("牌数: %d"), Count)));
}

//==============================================================================
// 设置翻牌鸡
// 显示翻牌鸡的牌值
//==============================================================================
void UMahjongGameWidget::SetFanPaiJi(int32 FanPaiValue)
{
    if (FanPaiJiText)
        FanPaiJiText->SetText(FText::FromString(FString::Printf(TEXT("翻牌鸡: %d"), FanPaiValue)));
}

//==============================================================================
// 显示/隐藏抢杠胡提示
// 当有其他玩家可以抢杠胡时显示提示
//==============================================================================
void UMahjongGameWidget::ShowQiangGangAlert(bool bShow)
{
    if (AlertImage)
        AlertImage->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

//==============================================================================
// 显示/隐藏听牌提示
// 显示玩家已报听的提示
//==============================================================================
void UMahjongGameWidget::ShowTingAlert(bool bShow)
{
    if (AlertImage)
        AlertImage->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

//==============================================================================
// 摸牌按钮点击处理
//==============================================================================
void UMahjongGameWidget::OnDrawButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 摸牌按钮点击"));

    // 调用控制器的摸牌处理
    if (PlayerController)
        PlayerController->OnDrawTileClicked();
}

//==============================================================================
// 出牌按钮点击处理
//==============================================================================
void UMahjongGameWidget::OnDiscardButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 出牌按钮点击"));

    // 调用控制器的出牌处理
    if (PlayerController && SelectedTileIndex >= 0)
        PlayerController->OnDiscardTileClicked(SelectedTileIndex);
}

//==============================================================================
// 碰按钮点击处理
//==============================================================================
void UMahjongGameWidget::OnPengButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 碰按钮点击"));

    // 调用控制器的碰牌处理
    if (PlayerController)
        PlayerController->OnPengClicked();
}

//==============================================================================
// 杠按钮点击处理
//==============================================================================
void UMahjongGameWidget::OnGangButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 杠按钮点击"));

    // 调用控制器的杠牌处理
    if (PlayerController)
        PlayerController->OnGangClicked(EDouType::DianDou);
}

//==============================================================================
// 胡按钮点击处理
//==============================================================================
void UMahjongGameWidget::OnHuButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 胡按钮点击"));

    // 调用控制器的胡牌处理
    if (PlayerController)
        PlayerController->OnHuClicked();
}

//==============================================================================
// 过按钮点击处理
//==============================================================================
void UMahjongGameWidget::OnGuoButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 过按钮点击"));

    // 调用控制器的过处理
    if (PlayerController)
        PlayerController->OnGuoClicked();
}

//==============================================================================
// 报听按钮点击处理
//==============================================================================
void UMahjongGameWidget::OnTingButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 报听按钮点击"));

    // 调用控制器的报听处理
    if (PlayerController)
        PlayerController->OnBaoTingClicked();
}

//==============================================================================
// 牌点击处理
//==============================================================================
void UMahjongGameWidget::OnTileClicked(int32 Index)
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] 牌点击: 索引=%d"), Index);

    // 选中该牌
    SelectTile(Index);

    // 调用控制器的选牌处理
    if (PlayerController)
        PlayerController->OnTileSelected(Index);
}

//==============================================================================
// 更新牌Widget
// 刷新手牌和弃牌池的显示
//==============================================================================
void UMahjongGameWidget::UpdateTileWidgets()
{
    // 清除旧Widget
    ClearHandTiles();
    ClearDiscardTiles();

    // 重新创建手牌Widget
    UpdateHandTiles(HandTiles);
    UpdateDiscardPool(DiscardedTiles);

    UE_LOG(LogTemp, Log, TEXT("[MahjongGameWidget] UpdateTileWidgets: refreshed"));
}

//==============================================================================
// 清空手牌显示
// 清除所有手牌Widget
//==============================================================================
void UMahjongGameWidget::ClearHandTiles()
{
    if (HandTilesGrid)
        HandTilesGrid->ClearChildren();

    HandTileWidgets.Empty();
}

//==============================================================================
// 清空弃牌池显示
// 清除所有弃牌Widget
//==============================================================================
void UMahjongGameWidget::ClearDiscardTiles()
{
    if (DiscardPoolGrid)
        DiscardPoolGrid->ClearChildren();

    DiscardTileWidgets.Empty();
}

//==============================================================================
// 获取牌颜色
// 根据牌ID返回对应颜色
// 万子=红色、条子=绿色、筒子=蓝色
//==============================================================================
FLinearColor UMahjongGameWidget::GetTileColor(int32 TileID) const
{
    int32 Suit = TileID / 10;
    int32 Value = TileID % 10;

    // 万子（0）- 红色
    if (Suit == 0)
        return FLinearColor(0.91f, 0.30f, 0.24f, 1.0f); // #E74C3C
    // 条子（1）- 绿色
    else if (Suit == 1)
        return FLinearColor(0.15f, 0.68f, 0.38f, 1.0f); // #27AE60
    // 筒子（2）- 蓝色
    else if (Suit == 2)
        return FLinearColor(0.20f, 0.60f, 0.86f, 1.0f); // #3498DB
    // 花牌（3）- 紫色
    else if (Suit == 3)
        return FLinearColor(0.61f, 0.35f, 0.71f, 1.0f); // #9B59B6

    // 默认白色
    return FLinearColor::White;
}