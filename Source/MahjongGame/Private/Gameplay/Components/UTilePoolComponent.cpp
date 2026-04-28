//==============================================================================
// 麻将游戏 - 牌堆组件实现
// UTilePoolComponent.cpp
//==============================================================================

#include "Gameplay/Components/UTilePoolComponent.h"

UTilePoolComponent::UTilePoolComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTilePoolComponent::InitializePool(const TArray<int32>& TileIDs)
{
    TilePool = TileIDs;
    // 打乱牌堆顺序
    for (int32 i = TilePool.Num() - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        if (i != j)
        {
            TilePool.Swap(i, j);
        }
    }
}

int32 UTilePoolComponent::DrawTile()
{
    if (TilePool.Num() == 0)
    {
        OnPoolEmpty.Broadcast(0);
        return -1;
    }

    int32 TileID = TilePool.Pop();
    OnTileDrawn.Broadcast(0, TileID);
    return TileID;
}
