#include "Gameplay/UI/Mahjong/MahjongTileWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"

UMahjongTileWidget::UMahjongTileWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bIsSelected = false;
    bIsHighlighted = false;
}

void UMahjongTileWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (TileButton)
        TileButton->OnClicked.AddDynamic(this, &UMahjongTileWidget::HandleClick);

    SetSelected(false);
    SetHighlighted(false);
    SetEnabled(true);
}

void UMahjongTileWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UMahjongTileWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
}

void UMahjongTileWidget::SetTileData(const FMahjongTile& Tile)
{
    TileData = Tile;
}

FMahjongTile UMahjongTileWidget::GetTileData() const
{
    return TileData;
}

void UMahjongTileWidget::SetSelected(bool bSelected)
{
    bIsSelected = bSelected;
    if (SelectedOverlay)
        SelectedOverlay->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UMahjongTileWidget::SetHighlighted(bool bHighlighted)
{
    bIsHighlighted = bHighlighted;
}

void UMahjongTileWidget::SetEnabled(bool bEnabled)
{
    if (DisabledOverlay)
        DisabledOverlay->SetVisibility(bEnabled ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

bool UMahjongTileWidget::IsSelected() const
{
    return bIsSelected;
}

void UMahjongTileWidget::HandleClick()
{
    OnTileClicked(TileData);
}
