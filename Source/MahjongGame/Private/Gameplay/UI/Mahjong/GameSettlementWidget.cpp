#include "Gameplay/UI/Mahjong/GameSettlementWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Core/UI/Common/UISoundManager.h"

UGameSettlementWidget::UGameSettlementWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameSettlementWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ShareButton)
        ShareButton->OnClicked.AddDynamic(this, &UGameSettlementWidget::OnShareClicked);

    if (PlayAgainButton)
        PlayAgainButton->OnClicked.AddDynamic(this, &UGameSettlementWidget::OnPlayAgainClicked);

    if (ReturnToLobbyButton)
        ReturnToLobbyButton->OnClicked.AddDynamic(this, &UGameSettlementWidget::OnReturnToLobbyClicked);

    HideSettlement();
}

void UGameSettlementWidget::ShowSettlement(const TArray<FPlayerSettlement>& Players)
{
    FString WinnerName;
    int32 WinnerHuType = 0;
    int32 WinnerFanCount = 0;

    for (const FPlayerSettlement& Player : Players)
    {
        if (Player.bIsWinner)
        {
            WinnerName = Player.PlayerName;
            WinnerHuType = Player.HuType;
            WinnerFanCount = Player.FanCount;
            break;
        }
    }

    if (WinnerNameText)
        WinnerNameText->SetText(FText::FromString(WinnerName));

    if (HuTypeText)
        HuTypeText->SetText(FText::FromString(GetHuTypeName(WinnerHuType)));

    if (FanCountText)
        FanCountText->SetText(FText::FromString(FString::Printf(TEXT("%d 番"), WinnerFanCount)));

    UUISoundManager::PlaySuccessSound(this); // 结算界面弹出成功音效
    Show();
}

void UGameSettlementWidget::HideSettlement()
{
    Hide();
}

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

void UGameSettlementWidget::OnShareClicked()
{
    UUISoundManager::PlayButtonClick(this);
    UE_LOG(LogTemp, Log, TEXT("[Settlement] 分享"));
}

void UGameSettlementWidget::OnPlayAgainClicked()
{
    UUISoundManager::PlayConfirmClick(this);
    UE_LOG(LogTemp, Log, TEXT("[Settlement] 再来一局"));
    HideSettlement();
}

void UGameSettlementWidget::OnReturnToLobbyClicked()
{
    UUISoundManager::PlayCancelClick(this);
    UE_LOG(LogTemp, Log, TEXT("[Settlement] 返回大厅"));
    HideSettlement();
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}
