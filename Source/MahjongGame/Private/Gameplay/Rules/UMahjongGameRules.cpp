//==============================================================================
// 麻将游戏 - 游戏规则基类实现
// UMahjongGameRules.cpp
//==============================================================================

#include "Gameplay/Rules/UMahjongGameRules.h"

FString UMahjongGameRules::GetGameName_Implementation() const
{
    return TEXT("Generic Mahjong");
}

int32 UMahjongGameRules::GetTilePoolCount_Implementation() const
{
    return DefaultTilePoolCount > 0 ? DefaultTilePoolCount : 144;
}

int32 UMahjongGameRules::GetPlayerCount_Implementation() const
{
    return DefaultPlayerCount > 0 ? DefaultPlayerCount : 4;
}

int32 UMahjongGameRules::GetInitialHandCount_Implementation() const
{
    return DefaultInitialHandCount > 0 ? DefaultInitialHandCount : 13;
}

bool UMahjongGameRules::CanPeng_Implementation(const FGenericHand& Hand, int32 TileValue) const
{
    // 使用 TMap 缓存计数，避免每次遍历整个手牌
    static TMap<int32, int32> TileCountCache;
    TileCountCache.Empty();

    for (const FGenericTile& Tile : Hand.Tiles)
    {
        TileCountCache.FindOrAdd(Tile.Value)++;
    }

    const int32* Count = TileCountCache.Find(TileValue);
    return Count != nullptr && *Count >= 2;
}

bool UMahjongGameRules::CanGang_Implementation(const FGenericHand& Hand, int32 TileValue) const
{
    // 使用 TMap 缓存计数，避免每次遍历整个手牌
    static TMap<int32, int32> TileCountCache;
    TileCountCache.Empty();

    for (const FGenericTile& Tile : Hand.Tiles)
    {
        TileCountCache.FindOrAdd(Tile.Value)++;
    }

    const int32* Count = TileCountCache.Find(TileValue);
    return Count != nullptr && *Count >= 3;
}

bool UMahjongGameRules::CanHu_Implementation(const FGenericHand& Hand, int32 TileValue) const
{
    // 默认实现：检查是否听牌
    TArray<int32> TingTiles = GetTingTiles_Implementation(Hand);
    return TingTiles.Contains(TileValue);
}

FAvailableActions UMahjongGameRules::GetAvailableActions_Implementation(int32 PlayerIndex, const FGenericHand& Hand, int32 LastDiscardTile) const
{
    FAvailableActions Actions;
    Actions.bCanPeng = CanPeng_Implementation(Hand, LastDiscardTile);
    Actions.bCanGang = CanGang_Implementation(Hand, LastDiscardTile);
    Actions.bCanHu = CanHu_Implementation(Hand, LastDiscardTile);
    Actions.bCanGuo = true;
    Actions.bCanTing = false;
    Actions.bCanZiMo = false;
    Actions.bCanDianPao = false;
    return Actions;
}

int32 UMahjongGameRules::CalculateHuScore_Implementation(const FGenericHand& Hand, int32 HuType) const
{
    return 1;
}

FString UMahjongGameRules::GetHuTypeName_Implementation(int32 HuType) const
{
    // 使用静态数组避免每次switch，编译期确定
    static const TCHAR* HuTypeNames[] = {
        TEXT("平胡"),      // 0
        TEXT("大对子"),    // 1
        TEXT("七对"),      // 2
        TEXT("龙七对"),    // 3
        TEXT("清一色"),    // 4
        TEXT("混一色"),    // 5
        TEXT("碰碰胡"),    // 6
        TEXT("全球人")     // 7
    };

    if (HuType >= 0 && HuType < 8)
    {
        return FString(HuTypeNames[HuType]);
    }
    return TEXT("胡牌");
}

TArray<int32> UMahjongGameRules::GetTingTiles_Implementation(const FGenericHand& Hand) const
{
    TArray<int32> TingTiles;
    // 子类应重写此函数
    return TingTiles;
}

UTexture2D* UMahjongGameRules::GetTileTexture_Implementation(int32 TileValue) const
{
    return nullptr;
}

TArray<int32> UMahjongGameRules::InitializeTilePool_Implementation() const
{
    TArray<int32> Pool;
    // 生成108张标准麻将牌
    for (int32 Suit = 0; Suit < 3; ++Suit)
    {
        for (int32 Value = 1; Value <= 9; ++Value)
        {
            for (int32 i = 0; i < 4; ++i)
            {
                Pool.Add(Suit * 10 + Value);
            }
        }
    }
    return Pool;
}

bool UMahjongGameRules::IsChickenTile_Implementation(int32 TileValue) const
{
    // 默认：1条（幺鸡）是鸡牌
    return TileValue == 11; // 条子花色 * 10 + 1
}
