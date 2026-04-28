//==============================================================================
// 麻将游戏 - 牌堆组件
// UTilePoolComponent.h
//
// 本文件定义牌堆管理组件
// 负责管理麻将牌堆的摸牌和剩余数量
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTilePoolComponent.generated.h"

//==============================================================================
// 委托定义
//==============================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTileDrawnEvent, int32, PlayerIndex, int32, TileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoolEmptyEvent, int32, RemainingCount);

//==============================================================================
// 牌堆组件
// 管理牌堆的摸牌操作
//==============================================================================
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAHJONGGAME_API UTilePoolComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTilePoolComponent();

    //============================================================================
    // 初始化牌堆
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|TilePool")
    void InitializePool(const TArray<int32>& TileIDs);

    //============================================================================
    // 摸牌
    // 返回：摸到的牌ID，-1表示牌堆已空
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|TilePool")
    int32 DrawTile();

    //============================================================================
    // 获取剩余牌数
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|TilePool")
    int32 GetRemainingCount() const { return TilePool.Num(); }

    //============================================================================
    // 是否为空
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|TilePool")
    bool IsEmpty() const { return TilePool.Num() == 0; }

    //============================================================================
    // 获取牌堆数据（用于调试）
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|TilePool")
    TArray<int32> GetTilePool() const { return TilePool; }

    //============================================================================
    // 广播：摸牌
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|TilePool")
    FOnTileDrawnEvent OnTileDrawn;

    //============================================================================
    // 广播：牌堆空
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|TilePool")
    FOnPoolEmptyEvent OnPoolEmpty;

protected:
    //============================================================================
    // 牌堆
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|TilePool")
    TArray<int32> TilePool;
};
