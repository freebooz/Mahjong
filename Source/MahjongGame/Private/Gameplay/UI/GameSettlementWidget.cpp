#include "Gameplay/UI/GameSettlementWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

//==============================================================================
// 游戏结算界面实现
//==============================================================================
UGameSettlementWidget::UGameSettlementWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

//==============================================================================
// 界面构建完成
//==============================================================================
void UGameSettlementWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ShareButton)
        ShareButton->OnClicked.AddDynamic(this, &UGameSettlementWidget::OnShareClicked);

    if (PlayAgainButton)
        PlayAgainButton->OnClicked.AddDynamic(this, &UGameSettlementWidget::OnPlayAgainClicked);

    if (ReturnToLobbyButton)
        ReturnToLobbyButton->OnClicked.AddDynamic(this, &UGameSettlementWidget::OnReturnToLobbyClicked);

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[GameSettlement] 游戏结算界面已构建"));
}

//==============================================================================
// 显示结算界面
//==============================================================================
void UGameSettlementWidget::ShowSettlement(const TArray<FPlayerSettlement>& Players)
{
    CurrentSettlement = Players;

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Visible);

    FString WinnerName;
    int32 MaxScore = -999999;
    int32 WinnerHuType = 0;
    int32 WinnerFanCount = 0;

    for (const FPlayerSettlement& Player : Players)
    {
        if (Player.bIsWinner)
        {
            WinnerName = Player.PlayerName;
            WinnerHuType = Player.HuType;
            WinnerFanCount = Player.FanCount;
        }
    }

    if (!WinnerName.IsEmpty())
    {
        SetWinnerName(WinnerName);
    }

    if (HuTypeText)
    {
        HuTypeText->SetText(FText::FromString(GetHuTypeName(WinnerHuType)));
    }

    if (FanCountText)
    {
        FanCountText->SetText(FText::FromString(FString::Printf(TEXT("%d 番"), WinnerFanCount)));
    }

    UE_LOG(LogTemp, Log, TEXT("[GameSettlement] 显示结算界面"));
}

//==============================================================================
// 隐藏结算界面
//==============================================================================
void UGameSettlementWidget::HideSettlement()
{
    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[GameSettlement] 隐藏结算界面"));
}

//==============================================================================
// 设置赢家名字
//==============================================================================
void UGameSettlementWidget::SetWinnerName(const FString& WinnerName)
{
    if (WinnerNameText)
        WinnerNameText->SetText(FText::FromString(WinnerName));
}

//==============================================================================
// 获取胡牌类型名称
//==============================================================================
FString UGameSettlementWidget::GetHuTypeName(int32 HuType)
{
    switch (HuType)
    {
        case 1: return TEXT("平胡");
        case 2: return TEXT("对子胡");
        case 3: return TEXT("清一色");
        case 4: return TEXT("七对子");
        case 5: return TEXT("海底捞月");
        case 6: return TEXT("杠上开花");
        case 7: return TEXT("抢杠胡");
        case 8: return TEXT("天胡");
        case 9: return TEXT("地胡");
        default: return TEXT("胡牌");
    }
}

//==============================================================================
// 分享按钮点击
//==============================================================================
void UGameSettlementWidget::OnShareClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameSettlement] 分享结算"));
}

//==============================================================================
// 再来一局按钮点击
//==============================================================================
void UGameSettlementWidget::OnPlayAgainClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameSettlement] 再来一局"));
    HideSettlement();
}

//==============================================================================
// 返回大厅按钮点击
//==============================================================================
void UGameSettlementWidget::OnReturnToLobbyClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[GameSettlement] 返回大厅"));
    HideSettlement();
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}
