#include "Variants/UI/Regional/GuizhouSettlementWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

UGuizhouSettlementWidget::UGuizhouSettlementWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGuizhouSettlementWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UGuizhouSettlementWidget::ShowGuizhouSettlement(
    const TArray<FPlayerSettlement>& Players,
    const FGuizhouSettlementData& GuizhouData
)
{
    GuizhouSettlementData = GuizhouData;

    ShowSettlement(Players);

    if (TotalChickenBonusText)
        TotalChickenBonusText->SetText(FText::FromString(FString::Printf(TEXT("捉鸡: +%d"), GuizhouData.TotalChickenCount)));

    if (TotalXiaMaBonusText)
        TotalXiaMaBonusText->SetText(FText::FromString(FString::Printf(TEXT("夏麻: +%d"), GuizhouData.XiaMaBonus)));
}

int32 UGuizhouSettlementWidget::GetTotalChickenBonus() const
{
    return GuizhouSettlementData.TotalChickenCount;
}
