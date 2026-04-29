#include "Gameplay/Actor/MahjongTileMesh.h"
#include "Components/StaticMeshComponent.h"

AMahjongTileMesh::AMahjongTileMesh()
{
    PrimaryActorTick.bCanEverTick = false;
    TileType = 0;
    TileValue = 0;
    bIsHighlighted = false;
    bIsSelected = false;
    TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
    RootComponent = TileMeshComponent;
    // 注意：资产加载应在运行时或编辑器中设置
}
void AMahjongTileMesh::InitializeTile(int32 InTileType, int32 InValue)
{
    TileType = InTileType;
    TileValue = InValue;
    UE_LOG(LogTemp, Log, TEXT("[MahjongTileMesh] InitializeTile: Type=%d, Value=%d"), TileType, TileValue);
}
void AMahjongTileMesh::SetHighlighted(bool bHighlight)
{
    bIsHighlighted = bHighlight;
    UE_LOG(LogTemp, Log, TEXT("[MahjongTileMesh] SetHighlighted: %s"), bHighlight ? TEXT("true") : TEXT("false"));
}
void AMahjongTileMesh::SetSelected(bool bSelected)
{
    bIsSelected = bSelected;
    UE_LOG(LogTemp, Log, TEXT("[MahjongTileMesh] SetSelected: %s"), bSelected ? TEXT("true") : TEXT("false"));
}