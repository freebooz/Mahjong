#include "Gameplay/UI/Mahjong/HandTilesWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UHandTilesWidget::UHandTilesWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SelectedIndex = -1;
    bIsSorted = false;
}

void UHandTilesWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SortButton)
        SortButton->OnClicked.AddDynamic(this, &UHandTilesWidget::SortTiles);
}

void UHandTilesWidget::SetHandTiles(const TArray<FMahjongTile>& Tiles)
{
    CurrentTiles = Tiles;
    bIsSorted = true;
}

void UHandTilesWidget::AddTile(const FMahjongTile& Tile)
{
    CurrentTiles.Add(Tile);
    bIsSorted = false;
}

void UHandTilesWidget::RemoveTile(int32 TileID)
{
    CurrentTiles.RemoveAll([TileID](const FMahjongTile& Tile) {
        return Tile.TileID == TileID;
    });
}

void UHandTilesWidget::SelectTile(int32 Index)
{
    if (Index >= 0 && Index < CurrentTiles.Num())
    {
        SelectedIndex = Index;
    }
}

void UHandTilesWidget::DeselectAll()
{
    SelectedIndex = -1;
}

int32 UHandTilesWidget::GetSelectedIndex() const
{
    return SelectedIndex;
}

FMahjongTile UHandTilesWidget::GetSelectedTile() const
{
    if (SelectedIndex >= 0 && SelectedIndex < CurrentTiles.Num())
        return CurrentTiles[SelectedIndex];

    return FMahjongTile();
}

void UHandTilesWidget::SortTiles()
{
    CurrentTiles.Sort([](const FMahjongTile& A, const FMahjongTile& B) {
        if (A.TileType != B.TileType)
            return A.TileType < B.TileType;
        return A.TileValue < B.TileValue;
    });
    bIsSorted = true;
}

int32 UHandTilesWidget::GetTileCount() const
{
    return CurrentTiles.Num();
}

TArray<FMahjongTile> UHandTilesWidget::GetAllTiles() const
{
    return CurrentTiles;
}

void UHandTilesWidget::OnTileClicked(int32 Index)
{
    SelectTile(Index);
}
