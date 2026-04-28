#include "Gameplay/Actor/MahjongTileMesh.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
AMahjongTileMesh::AMahjongTileMesh()
{
    PrimaryActorTick.bCanEverTick = false;
    TileType = 0;
    TileValue = 0;
    bIsHighlighted = false;
    bIsSelected = false;
    TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComponent"));
    RootComponent = TileMeshComponent;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (TileMeshAsset.Succeeded())
    {
        TileMeshComponent->SetStaticMesh(TileMeshAsset.Object);
        TileMeshComponent->SetWorldScale3D(FVector(0.8f, 0.2f, 1.2f));
    }
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