#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MahjongTableMesh.generated.h"
class UStaticMeshComponent;
class UMaterialInterface;
UCLASS()
class MAHJONGGAME_API AMahjongTableMesh : public AActor
{
    GENERATED_BODY()
public:
    AMahjongTableMesh();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Table")
    void InitializeTable();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Table")
    void SetTableMaterial(UMaterialInterface* Material);
protected:
    UPROPERTY(VisibleAnywhere, Category="Mahjong|Table")
    UStaticMeshComponent* TableTopComponent;
    UPROPERTY(VisibleAnywhere, Category="Mahjong|Table")
    UStaticMeshComponent* TableLeg1Component;
    UPROPERTY(VisibleAnywhere, Category="Mahjong|Table")
    UStaticMeshComponent* TableLeg2Component;
    UPROPERTY(VisibleAnywhere, Category="Mahjong|Table")
    UStaticMeshComponent* TableLeg3Component;
    UPROPERTY(VisibleAnywhere, Category="Mahjong|Table")
    UStaticMeshComponent* TableLeg4Component;
};