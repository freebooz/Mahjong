#include "Gameplay/Actor/MahjongTableMesh.h"
#include "Components/StaticMeshComponent.h"

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
    // 注意：资产加载应在运行时或编辑器中设置
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