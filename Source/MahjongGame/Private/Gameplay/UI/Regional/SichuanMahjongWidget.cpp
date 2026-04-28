#include "Gameplay/UI/Regional/SichuanMahjongWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

USichuanMahjongWidget::USichuanMahjongWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SichuanData.CalledTileValue = -1;
    SichuanData.bIsBaoJiao = false;
    SichuanData.WindRainBonus = 0;
}

void USichuanMahjongWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BaoJiaoButton)
        BaoJiaoButton->OnClicked.AddDynamic(this, &USichuanMahjongWidget::OnBaoJiaoClicked);

    if (NoBaoJiaoButton)
        NoBaoJiaoButton->OnClicked.AddDynamic(this, &USichuanMahjongWidget::OnNoBaoJiaoClicked);

    if (BaoJiaoPromptPanel)
        BaoJiaoPromptPanel->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[SichuanMahjongWidget] 四川麻将界面已构建"));
}

void USichuanMahjongWidget::InitializeSichuanMahjong()
{
    InitializeGame();
}

void USichuanMahjongWidget::SetCalledTile(int32 TileValue)
{
    SichuanData.CalledTileValue = TileValue;

    if (BaoJiaoPromptPanel)
        BaoJiaoPromptPanel->SetVisibility(ESlateVisibility::Visible);

    if (CalledTilePromptText)
        CalledTilePromptText->SetText(FText::FromString(FString::Printf(TEXT("报叫！叫牌: %d"), TileValue)));
}

int32 USichuanMahjongWidget::GetCalledTileValue() const
{
    return SichuanData.CalledTileValue;
}

void USichuanMahjongWidget::UpdateWindRainBonus(int32 Bonus)
{
    SichuanData.WindRainBonus = Bonus;

    if (WindRainBonusText)
        WindRainBonusText->SetText(FText::FromString(FString::Printf(TEXT("刮风下雨: +%d"), Bonus)));
}

void USichuanMahjongWidget::OnBaoJiaoClicked()
{
    SichuanData.bIsBaoJiao = true;
    if (BaoJiaoPromptPanel)
        BaoJiaoPromptPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void USichuanMahjongWidget::OnNoBaoJiaoClicked()
{
    SichuanData.bIsBaoJiao = false;
    if (BaoJiaoPromptPanel)
        BaoJiaoPromptPanel->SetVisibility(ESlateVisibility::Collapsed);
}
