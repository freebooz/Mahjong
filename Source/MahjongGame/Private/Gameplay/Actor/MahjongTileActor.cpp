#include "Gameplay/Actor/MahjongTileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"

AMahjongTileActor::AMahjongTileActor()
{
    PrimaryActorTick.bCanEverTick = true;
    TileID = 0;
    TileValue = 0;
    bIsSelected = false;
    bIsHighlighted = false;
    bIsFaceUp = true;
    TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    RootComponent = TileMeshComponent;
    // 注意：资产加载应在运行时通过 SetTileMesh 或编辑器中设置
    // 这里不进行静态加载以兼容 UE5.7
}
void AMahjongTileActor::BeginPlay()
{
    Super::BeginPlay();
    UpdateVisuals();
}
void AMahjongTileActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void AMahjongTileActor::SetTileData(int32 InTileID, const FString& InSuit, int32 InValue)
{
    TileID = InTileID;
    TileSuit = InSuit;
    TileValue = InValue;
    UpdateVisuals();
    UE_LOG(LogTemp, Log, TEXT("[Tile] SetTileData: ID=%d, Suit=%s, Value=%d"), TileID, *TileSuit, TileValue);
}
void AMahjongTileActor::SetTileMesh(UStaticMesh* Mesh)
{
    if (Mesh && TileMeshComponent)
        TileMeshComponent->SetStaticMesh(Mesh);
}
void AMahjongTileActor::HighlightTile(bool bHighlight)
{
    bIsHighlighted = bHighlight;
    UpdateVisuals();
}
void AMahjongTileActor::AnimateTilePlacement(float Duration)
{
    if (Duration <= 0) Duration = 0.3f;
    UE_LOG(LogTemp, Log, TEXT("[Tile] Animating tile placement over %f seconds"), Duration);
}
void AMahjongTileActor::AnimateTileFlip()
{
    bIsFaceUp = !bIsFaceUp;
    UpdateVisuals();
    UE_LOG(LogTemp, Log, TEXT("[Tile] Tile flipped, face up: %s"), bIsFaceUp ? TEXT("true") : TEXT("false"));
}
void AMahjongTileActor::UpdateVisuals()
{
    if (!TileMeshComponent) return;
    UMaterialInstance* TargetMat = nullptr;
    if (bIsSelected && SelectedMaterial)
        TargetMat = SelectedMaterial;
    else if (bIsHighlighted && HighlightedMaterial)
        TargetMat = HighlightedMaterial;
    else if (!bIsFaceUp && BackMaterial)
        TargetMat = BackMaterial;
    else if (NormalMaterial)
        TargetMat = NormalMaterial;
    if (TargetMat)
        TileMeshComponent->SetMaterial(0, TargetMat);
}