#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MahjongTileActor.generated.h"
class UStaticMeshComponent;
UCLASS()
class MAHJONGGAME_API AMahjongTileActor : public AActor
{
    GENERATED_BODY()
public:
    AMahjongTileActor();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetTileData(int32 InTileID, const FString& InSuit, int32 InValue);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetTileMesh(UStaticMesh* Mesh);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void HighlightTile(bool bHighlight);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void AnimateTilePlacement(float Duration);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void AnimateTileFlip();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetTileName(const FString& Name) { TileName = Name; }
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Tile")
    int32 TileID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Tile")
    FString TileSuit;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Tile")
    int32 TileValue;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Tile")
    FString TileName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Tile")
    bool bIsSelected;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Tile")
    bool bIsHighlighted;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Tile")
    bool bIsFaceUp;
protected:
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    UStaticMeshComponent* TileMeshComponent;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    UMaterialInstance* NormalMaterial;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    UMaterialInstance* HighlightedMaterial;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    UMaterialInstance* SelectedMaterial;
    UPROPERTY(EditAnywhere, Category="Mahjong|Tile")
    UMaterialInstance* BackMaterial;
private:
    void UpdateVisuals();
};