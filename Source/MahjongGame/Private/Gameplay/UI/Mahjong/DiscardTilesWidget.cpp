#include "Gameplay/UI/Mahjong/DiscardTilesWidget.h"
#include "Components/WrapBox.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

UDiscardTilesWidget::UDiscardTilesWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    LastDiscardIndex = -1;
}

void UDiscardTilesWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UDiscardTilesWidget::AddDiscardTile(const FMahjongTile& Tile, int32 PlayerID)
{
    DiscardTiles.Add(Tile);
    DiscardPlayerIDs.Add(PlayerID);
    LastDiscardIndex = DiscardTiles.Num() - 1;
}

FMahjongTile UDiscardTilesWidget::GetLastDiscardTile() const
{
    if (!DiscardTiles.IsEmpty())
        return DiscardTiles.Last();
    return FMahjongTile();
}

int32 UDiscardTilesWidget::GetLastDiscardPlayerID() const
{
    if (!DiscardPlayerIDs.IsEmpty())
        return DiscardPlayerIDs.Last();
    return -1;
}

int32 UDiscardTilesWidget::GetDiscardCount() const
{
    return DiscardTiles.Num();
}

TArray<FMahjongTile> UDiscardTilesWidget::GetPlayerDiscards(int32 PlayerID) const
{
    TArray<FMahjongTile> PlayerTiles;
    for (int32 i = 0; i < DiscardTiles.Num(); ++i)
    {
        if (DiscardPlayerIDs[i] == PlayerID)
            PlayerTiles.Add(DiscardTiles[i]);
    }
    return PlayerTiles;
}

void UDiscardTilesWidget::ClearAllDiscards()
{
    DiscardTiles.Empty();
    DiscardPlayerIDs.Empty();
    LastDiscardIndex = -1;
}

TArray<FMahjongTile> UDiscardTilesWidget::GetRecentDiscards(int32 Count) const
{
    TArray<FMahjongTile> Recent;
    int32 Start = FMath::Max(0, DiscardTiles.Num() - Count);
    for (int32 i = Start; i < DiscardTiles.Num(); ++i)
        Recent.Add(DiscardTiles[i]);
    return Recent;
}

void UDiscardTilesWidget::OnDiscardTileClicked(int32 Index)
{
}
