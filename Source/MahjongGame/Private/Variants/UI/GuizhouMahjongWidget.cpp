#include "Variants/UI/Regional/GuizhouMahjongWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

UGuizhouMahjongWidget::UGuizhouMahjongWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    GuizhouData.ChickenTileValue = -1;
    GuizhouData.bChickenCaptured = false;
    GuizhouData.XiaMaValue = -1;
    GuizhouData.XiaMaCount = 0;
}

void UGuizhouMahjongWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CatchChickenButton)
        CatchChickenButton->OnClicked.AddDynamic(this, &UGuizhouMahjongWidget::OnCatchChickenClicked);

    if (NoCatchChickenButton)
        NoCatchChickenButton->OnClicked.AddDynamic(this, &UGuizhouMahjongWidget::OnNoCatchChickenClicked);

    if (ChickenPromptPanel)
        ChickenPromptPanel->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[GuizhouMahjongWidget] 贵阳麻将界面已构建"));
}

void UGuizhouMahjongWidget::InitializeGuizhouMahjong()
{
    InitializeGame();
}

void UGuizhouMahjongWidget::SetChickenTile(int32 TileValue)
{
    GuizhouData.ChickenTileValue = TileValue;

    if (ChickenPromptPanel)
        ChickenPromptPanel->SetVisibility(ESlateVisibility::Visible);

    if (ChickenPromptText)
        ChickenPromptText->SetText(FText::FromString(FString::Printf(TEXT("捉鸡！鸡牌值: %d"), TileValue)));
}

int32 UGuizhouMahjongWidget::GetChickenTileValue() const
{
    return GuizhouData.ChickenTileValue;
}

void UGuizhouMahjongWidget::OnCatchChickenClicked()
{
    GuizhouData.bChickenCaptured = true;
    if (ChickenPromptPanel)
        ChickenPromptPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UGuizhouMahjongWidget::OnNoCatchChickenClicked()
{
    GuizhouData.bChickenCaptured = false;
    if (ChickenPromptPanel)
        ChickenPromptPanel->SetVisibility(ESlateVisibility::Collapsed);
}
