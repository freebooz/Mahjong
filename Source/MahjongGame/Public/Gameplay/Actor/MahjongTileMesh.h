#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MahjongTileMesh.generated.h"
class UStaticMeshComponent;
class UMaterialInterface;
UCLASS()
class MAHJONGGAME_API AMahjongTileMesh : public AActor
{
    GENERATED_BODY()
public:
    AMahjongTileMesh();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void InitializeTile(int32 TileType, int32 Value);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetHighlighted(bool bHighlight);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetSelected(bool bSelected);
protected:
    UPROPERTY(VisibleAnywhere, Category="Mahjong|Tile")
    UStaticMeshComponent* TileMeshComponent;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    int32 TileType;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    int32 TileValue;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    bool bIsHighlighted;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    bool bIsSelected;
};