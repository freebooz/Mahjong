//==============================================================================
// 麻将游戏 - 通用手牌结构
// FGenericHand.h
//
// 本文件定义通用的麻将手牌结构
// 包含普通手牌、碰牌、杠牌等
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Core/Types/FGenericTile.h"
#include "FGenericHand.generated.h"

//==============================================================================
// 通用麻将手牌结构
// 包含玩家所有相关牌组
//==============================================================================
USTRUCT(BlueprintType)
struct FGenericHand
{
    GENERATED_BODY()

    // 普通手牌
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGenericTile> Tiles;

    // 碰牌（三张相同的牌）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGenericTile> PengTiles;

    // 暗杠（四张相同的牌，自己摸到）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGenericTile> AnGangTiles;

    // 明杠（四张相同的牌，碰后摸到或点杠）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGenericTile> MingGangTiles;

    // 备用牌（用于多牌或少牌的容错）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGenericTile> HandTiles;

    //============================================================================
    // 获取总牌数
    // 计算公式：普通牌 + 碰牌×3 + 暗杠×4 + 明杠×4
    //============================================================================
    int32 GetTotalTileCount() const
    {
        return Tiles.Num()
            + PengTiles.Num() * 3
            + AnGangTiles.Num() * 4
            + MingGangTiles.Num() * 4
            + HandTiles.Num();
    }

    //============================================================================
    // 是否为空
    //============================================================================
    bool IsEmpty() const
    {
        return Tiles.Num() == 0
            && PengTiles.Num() == 0
            && AnGangTiles.Num() == 0
            && MingGangTiles.Num() == 0
            && HandTiles.Num() == 0;
    }

    //============================================================================
    // 获取所有牌（用于显示）
    //============================================================================
    TArray<FGenericTile> GetAllTiles() const
    {
        TArray<FGenericTile> AllTiles;
        AllTiles.Append(Tiles);
        AllTiles.Append(PengTiles);
        AllTiles.Append(AnGangTiles);
        AllTiles.Append(MingGangTiles);
        AllTiles.Append(HandTiles);
        return AllTiles;
    }

    //============================================================================
    // 获取听牌列表
    //============================================================================
    TArray<int32> GetTingTileValues() const
    {
        TArray<int32> TingValues;
        // 子类实现
        return TingValues;
    }
};
