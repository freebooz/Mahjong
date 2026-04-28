#include "Gameplay/Actor/MahjongTableActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AMahjongTableActor::AMahjongTableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// 网络优化：控制更新频率
	SetNetUpdateFrequency(10.0f);
	NetPriority = 10.0f;

	// 移动端优化：默认启用
	bEnableMobileSimplification = true;
	CurrentLODLevel = 0;
}

void AMahjongTableActor::BeginPlay()
{
	Super::BeginPlay();

	// 只有服务器初始化牌桌
	if (HasAuthority())
	{
		SetupMahjongTable();
	}
}

void AMahjongTableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 清理牌墙Actor - 避免内存泄漏
	for (auto& TileWeakPtr : TileActors)
	{
		if (TileWeakPtr.IsValid())
		{
			AActor* Tile = TileWeakPtr.Get();
			if (Tile)
			{
				Tile->Destroy();
			}
		}
	}
	TileActors.Empty();

	Super::EndPlay(EndPlayReason);
}

void AMahjongTableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 移动端LOD更新
	if (bEnableMobileSimplification)
	{
		UpdateLODLevel();
	}
}

void AMahjongTableActor::SetupMahjongTable()
{
	// 创建四面牌墙
	for (int32 WallIndex = 0; WallIndex < 4; WallIndex++)
	{
		CreateWall(WallIndex);
	}
}

void AMahjongTableActor::CreateWall(int32 WallIndex)
{
	for (int32 TileIndex = 0; TileIndex < TilesPerWallSide; TileIndex++)
	{
		// 底层牌
		FVector BottomLocation = CalculateTilePosition(WallIndex, TileIndex, false);
		AActor* BottomTile = GetWorld()->SpawnActor<AActor>(BottomLocation, FRotator::ZeroRotator);
		if (BottomTile)
		{
			BottomTile->SetReplicates(true);
			TileActors.Add(BottomTile);
		}

		// 上层牌
		FVector TopLocation = CalculateTilePosition(WallIndex, TileIndex, true);
		AActor* TopTile = GetWorld()->SpawnActor<AActor>(TopLocation, FRotator::ZeroRotator);
		if (TopTile)
		{
			TopTile->SetReplicates(true);
			TileActors.Add(TopTile);
		}
	}
}

void AMahjongTableActor::UpdateLODLevel()
{
	// 获取相机位置
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC || !PC->PlayerCameraManager)
	{
		return;
	}

	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	float Distance = FVector::Dist(CameraLocation, GetActorLocation());

	// 计算LOD级别
	int32 NewLODLevel = 2; // 默认最远
	if (Distance < LODDistanceHigh)
	{
		NewLODLevel = 0; // 高细节
	}
	else if (Distance < LODDistanceMedium)
	{
		NewLODLevel = 1; // 中等细节
	}

	// 如果LOD级别变化，应用优化
	if (NewLODLevel != CurrentLODLevel)
	{
		CurrentLODLevel = NewLODLevel;
		ApplyMobileOptimizations();
	}
}

void AMahjongTableActor::ApplyMobileOptimizations()
{
	// 根据LOD级别调整渲染细节
	switch (CurrentLODLevel)
	{
	case 0: // 高细节 - 近距离
		for (auto& TileWeakPtr : TileActors)
		{
			if (TileWeakPtr.IsValid())
			{
				AActor* Tile = TileWeakPtr.Get();
				Tile->SetActorHiddenInGame(false);
			}
		}
		break;

	case 1: // 中等细节
		for (auto& TileWeakPtr : TileActors)
		{
			if (TileWeakPtr.IsValid())
			{
				AActor* Tile = TileWeakPtr.Get();
				Tile->SetActorHiddenInGame(false);
			}
		}
		break;

	case 2: // 远距离 - 简化渲染
	default:
		// 隐藏远处的牌，只保留桌子的基本形状
		for (auto& TileWeakPtr : TileActors)
		{
			if (TileWeakPtr.IsValid())
			{
				AActor* Tile = TileWeakPtr.Get();
				Tile->SetActorHiddenInGame(true);
			}
		}
		break;
	}

	UE_LOG(LogTemp, Log, TEXT("[MahjongTableActor] LOD Level: %d"), CurrentLODLevel);
}

FVector AMahjongTableActor::CalculateTilePosition(int32 WallIndex, int32 TileIndex, bool bIsTopLayer) const
{
	float WallLength = TilesPerWallSide * TileDimensions.X;
	float HalfWall = WallLength / 2.0f;
	float WallOffset = TableWidth / 2.0f + TileDimensions.Y;
	float Height = bIsTopLayer ? TileDimensions.Z : 0.0f;

	// 根据墙壁索引计算位置
	switch (WallIndex)
	{
	case 0: // 东
		return FVector(WallOffset, -HalfWall + (TileIndex * TileDimensions.X), Height);
	case 1: // 南
		return FVector(-HalfWall + (TileIndex * TileDimensions.X), -WallOffset, Height);
	case 2: // 西
		return FVector(-WallOffset, -HalfWall + (TileIndex * TileDimensions.X), Height);
	case 3: // 北
		return FVector(-HalfWall + (TileIndex * TileDimensions.X), WallOffset, Height);
	default:
		return FVector::ZeroVector;
	}
}

FVector AMahjongTableActor::GetTileLocation(int32 WallIndex, int32 TileIndex, bool bIsTopLayer) const
{
	return CalculateTilePosition(WallIndex, TileIndex, bIsTopLayer);
}

void AMahjongTableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 牌桌配置数据需要复制 - 只在首次变化时复制
	DOREPLIFETIME_CONDITION(AMahjongTableActor, TileActors, COND_SkipOwner);
}

#if WITH_EDITOR
void AMahjongTableActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 编辑器中修改属性后重新应用设置
	UE_LOG(LogTemp, Log, TEXT("[MahjongTableActor] Property changed, reapplying settings"));
}
#endif