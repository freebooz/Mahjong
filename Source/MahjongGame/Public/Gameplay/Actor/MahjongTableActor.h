// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MahjongTableActor.generated.h"

UCLASS()
class MAHJONGGAME_API AMahjongTableActor : public AActor
{
	GENERATED_BODY()

public:
	AMahjongTableActor();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	// 初始化麻将桌布局
	UFUNCTION(BlueprintCallable, Category = "Mahjong")
	void SetupMahjongTable();

	// 获取特定麻将牌的三维位置
	UFUNCTION(BlueprintCallable, Category = "Mahjong")
	FVector GetTileLocation(int32 WallIndex, int32 TileIndex, bool bIsTopLayer) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// 桌子宽度
	UPROPERTY(EditAnywhere, Category = "Mahjong Setup")
	float TableWidth = 200.0f;

	// 麻将牌尺寸 (宽，长，高)
	UPROPERTY(EditAnywhere, Category = "Mahjong Setup")
	FVector TileDimensions = FVector(5.0f, 3.0f, 2.0f);

	// 每面墙的牌数
	UPROPERTY(EditAnywhere, Category = "Mahjong Setup")
	int32 TilesPerWallSide = 18;

	// 存储麻将牌实例 - 使用TWeakObjectPtr防止悬挂指针
	UPROPERTY(Replicated)
	TArray<TWeakObjectPtr<AActor>> TileActors;

	// ==================== 移动端优化 ====================
	// LOD距离阈值
	UPROPERTY(EditAnywhere, Category = "Mobile Optimization|LOD")
	float LODDistanceHigh = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Mobile Optimization|LOD")
	float LODDistanceMedium = 2000.0f;

	// 移动端是否启用简化渲染
	UPROPERTY(EditAnywhere, Category = "Mobile Optimization")
	bool bEnableMobileSimplification = true;

	// 当前LOD级别 (0=近, 1=中, 2=远) - 默认初始化为0
	UPROPERTY()
	int32 CurrentLODLevel = 0;

	// 创建单面墙
	void CreateWall(int32 WallIndex);

	// 计算麻将牌位置
	FVector CalculateTilePosition(int32 WallIndex, int32 TileIndex, bool bIsTopLayer) const;

	// 更新LOD级别
	void UpdateLODLevel();

	// 移动端优化：根据距离调整显示
	void ApplyMobileOptimizations();

	// 网络复制支持
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
