#include "Variants/UI/Regional/SichuanSettlementWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

USichuanSettlementWidget::USichuanSettlementWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USichuanSettlementWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void USichuanSettlementWidget::ShowSichuanSettlement(
    const TArray<FPlayerSettlement>& Players,
    const FSichuanSettlementData& SichuanData
)
{
    SichuanSettlementData = SichuanData;

    ShowSettlement(Players);

    if (WindRainTotalText)
        WindRainTotalText->SetText(FText::FromString(FString::Printf(TEXT("刮风下雨: +%d"), SichuanData.WindRainBonus)));

    if (BloodBattleText)
        BloodBattleText->SetText(FText::FromString(FString::Printf(TEXT("血战场倍数: x%d"), SichuanData.BloodBattleMultiplier)));
}

int32 USichuanSettlementWidget::GetWindRainBonus() const
{
    return SichuanSettlementData.WindRainBonus;
}
