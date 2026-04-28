#include "Gameplay/Actor/MahjongTileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "UObject/ConstructorHelpers.h"
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
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TileMeshAsset(TEXT("/Game/Meshes/TileMesh"));
    if (TileMeshAsset.Succeeded())
        TileMeshComponent->SetStaticMesh(TileMeshAsset.Object);
    static ConstructorHelpers::FObjectFinder<UMaterialInstance> NormalMat(TEXT("/Game/Materials/TileNormalMat"));
    if (NormalMat.Succeeded())
        NormalMaterial = NormalMat.Object;
    static ConstructorHelpers::FObjectFinder<UMaterialInstance> HighlightMat(TEXT("/Game/Materials/TileHighlightMat"));
    if (HighlightMat.Succeeded())
        HighlightedMaterial = HighlightMat.Object;
    static ConstructorHelpers::FObjectFinder<UMaterialInstance> SelectedMat(TEXT("/Game/Materials/TileSelectedMat"));
    if (SelectedMat.Succeeded())
        SelectedMaterial = SelectedMat.Object;
    static ConstructorHelpers::FObjectFinder<UMaterialInstance> BackMat(TEXT("/Game/Materials/TileBackMat"));
    if (BackMat.Succeeded())
        BackMaterial = BackMat.Object;
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