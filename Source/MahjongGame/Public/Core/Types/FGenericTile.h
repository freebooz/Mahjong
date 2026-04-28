//==============================================================================
// 麻将游戏 - 通用牌结构
// FGenericTile.h
//
// 本文件定义通用的麻将牌结构
// 是所有游戏变体的基础数据结构
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Core/Types/ETileSuit.h"
#include "FGenericTile.generated.h"

//==============================================================================
// 通用麻将牌结构
// 用于在游戏逻辑和UI之间传递牌信息
//==============================================================================
USTRUCT(BlueprintType)
struct FGenericTile
{
    GENERATED_BODY()

    // 牌ID（唯一标识，用于网络同步和状态管理）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TileID;

    // 花色
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETileSuit Suit;

    // 牌值（1-9）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value;

    // 是否为癞子牌（可替代任意牌）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRedJoker;

    // 是否为鸡牌（用于贵阳麻将鸡牌计算）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsChicken;

    // 是否已打出（用于弃牌池）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsDiscarded;

    //============================================================================
    // 默认构造函数
    //============================================================================
    FGenericTile()
        : TileID(-1)
        , Suit(ETileSuit::None)
        , Value(0)
        , bIsRedJoker(false)
        , bIsChicken(false)
        , bIsDiscarded(false)
    {}

    //============================================================================
    // 参数构造函数
    //============================================================================
    FGenericTile(int32 InTileID, ETileSuit InSuit, int32 InValue)
        : TileID(InTileID)
        , Suit(InSuit)
        , Value(InValue)
        , bIsRedJoker(false)
        , bIsChicken(false)
        , bIsDiscarded(false)
    {}

    //============================================================================
    // 是否为有效牌
    //============================================================================
    bool IsValid() const
    {
        return TileID >= 0 && Suit != ETileSuit::None && Value >= 1 && Value <= 9;
    }

    //============================================================================
    // 获取牌组ID（同一点数的牌归为一组）
    //============================================================================
    int32 GetTileGroup() const
    {
        return static_cast<int32>(Suit) * 10 + Value;
    }
};
