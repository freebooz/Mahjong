#include "Gameplay/Actor/MahjongTableMesh.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
AMahjongTableMesh::AMahjongTableMesh()
{
    PrimaryActorTick.bCanEverTick = false;
    TableTopComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableTopComponent"));
    TableTopComponent->SetupAttachment(RootComponent);
    TableLeg1Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableLeg1Component"));
    TableLeg1Component->SetupAttachment(RootComponent);
    TableLeg2Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableLeg2Component"));
    TableLeg2Component->SetupAttachment(RootComponent);
    TableLeg3Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableLeg3Component"));
    TableLeg3Component->SetupAttachment(RootComponent);
    TableLeg4Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableLeg4Component"));
    TableLeg4Component->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TableAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (TableAsset.Succeeded())
    {
        TableTopComponent->SetStaticMesh(TableAsset.Object);
        TableTopComponent->SetWorldScale3D(FVector(4.0f, 4.0f, 0.3f));
        TableTopComponent->SetWorldLocation(FVector(0, 0, 40.0f));
        float LegScale = 0.3f;
        float LegHeight = 1.5f;
        float Offset = 1.5f;
        TableLeg1Component->SetStaticMesh(TableAsset.Object);
        TableLeg1Component->SetWorldScale3D(FVector(LegScale, LegScale, LegHeight));
        TableLeg1Component->SetWorldLocation(FVector(-Offset, -Offset, 0));
        TableLeg2Component->SetStaticMesh(TableAsset.Object);
        TableLeg2Component->SetWorldScale3D(FVector(LegScale, LegScale, LegHeight));
        TableLeg2Component->SetWorldLocation(FVector(Offset, -Offset, 0));
        TableLeg3Component->SetStaticMesh(TableAsset.Object);
        TableLeg3Component->SetWorldScale3D(FVector(LegScale, LegScale, LegHeight));
        TableLeg3Component->SetWorldLocation(FVector(-Offset, Offset, 0));
        TableLeg4Component->SetStaticMesh(TableAsset.Object);
        TableLeg4Component->SetWorldScale3D(FVector(LegScale, LegScale, LegHeight));
        TableLeg4Component->SetWorldLocation(FVector(Offset, Offset, 0));
    }
}
void AMahjongTableMesh::InitializeTable()
{
    UE_LOG(LogTemp, Log, TEXT("[MahjongTableMesh] Table initialized"));
}
void AMahjongTableMesh::SetTableMaterial(UMaterialInterface* Material)
{
    if (Material)
    {
        if (TableTopComponent) TableTopComponent->SetMaterial(0, Material);
    }
}