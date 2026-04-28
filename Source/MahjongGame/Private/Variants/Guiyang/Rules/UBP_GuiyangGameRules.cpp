//==============================================================================
// 麻将游戏 - 贵阳麻将规则Blueprint基类实现
// UBP_GuiyangGameRules.cpp
//==============================================================================

#include "Variants/Guiyang/Rules/UBP_GuiyangGameRules.h"

FString UBP_GuiyangGameRules::GetGameName_Implementation() const
{
    return TEXT("贵阳捉鸡麻将");
}

bool UBP_GuiyangGameRules::IsChickenTile_Implementation(int32 TileValue) const
{
    // 幺鸡（1条）
    if (TileValue == YaoJiValue)
        return true;

    // 翻牌鸡
    if (TileValue == FanPaiJiValue)
        return true;

    // 金鸡
    if (JinJiValues.Contains(TileValue))
        return true;

    return false;
}

int32 UBP_GuiyangGameRules::CalculateChickenScore(bool bHasYaoJi, bool bIsChongFengJiParam, bool bIsZeRenJiParam) const
{
    int32 Score = 0;
    if (bHasYaoJi) Score += 1;      // 持有幺鸡 +1番
    if (bIsChongFengJiParam) Score += 1;  // 冲锋鸡 +1番
    if (bIsZeRenJiParam) Score += 1;      // 责任鸡 +1番
    // 翻牌鸡：根据翻牌值计算，鸡牌分叠加
    if (FanPaiJiValue > 0) Score += 1;
    // 金鸡：每只金鸡 +1番
    Score += JinJiValues.Num();
    return Score;
}

int32 UBP_GuiyangGameRules::CalculateDouScore(int32 MenDouCount, int32 ZhuanWanDouCount, int32 DianDouCount) const
{
    return MenDouCount * 1 + ZhuanWanDouCount * 2 + DianDouCount * 3;
}

bool UBP_GuiyangGameRules::IsDaDuiZi(const FGenericHand& Hand) const
{
    // 大对子：全部是对子（碰牌也算）
    int32 PengCount = Hand.PengTiles.Num();
    int32 AnGangCount = Hand.AnGangTiles.Num();
    int32 MingGangCount = Hand.MingGangTiles.Num();
    int32 DuiZiCount = PengCount + AnGangCount + MingGangCount;

    // 手牌中对子数量
    int32 DuiZiInHand = 0;
    TMap<int32, int32> TileCount;
    for (const FGenericTile& Tile : Hand.Tiles)
    {
        int32 Key = Tile.Value;
        TileCount.FindOrAdd(Key)++;
    }
    for (const auto& Pair : TileCount)
    {
        if (Pair.Value >= 2)
            DuiZiInHand++;
    }

    // 大对子：4个对子 + 2个刻子（碰牌）
    return DuiZiInHand >= 4 && PengCount >= 2;
}

bool UBP_GuiyangGameRules::IsQiDui(const FGenericHand& Hand) const
{
    // 七对：7个对子
    TMap<int32, int32> TileCount;
    for (const FGenericTile& Tile : Hand.Tiles)
    {
        int32 Key = Tile.Value;
        TileCount.FindOrAdd(Key)++;
    }

    int32 DuiCount = 0;
    for (const auto& Pair : TileCount)
    {
        if (Pair.Value >= 2)
            DuiCount++;
    }

    return DuiCount >= 7;
}

bool UBP_GuiyangGameRules::IsLongQiDui(const FGenericHand& Hand) const
{
    // 龙七对：7个对子 + 一对将（实际是8对）
    TMap<int32, int32> TileCount;
    for (const FGenericTile& Tile : Hand.Tiles)
    {
        int32 Key = Tile.Value;
        TileCount.FindOrAdd(Key)++;
    }

    int32 DuiCount = 0;
    for (const auto& Pair : TileCount)
    {
        if (Pair.Value >= 2)
            DuiCount++;
    }

    return DuiCount >= 8;
}

bool UBP_GuiyangGameRules::IsYiSe(const FGenericHand& Hand) const
{
    // 一色：全部是同一种花色
    if (Hand.Tiles.Num() == 0)
        return false;

    ETileSuit FirstSuit = Hand.Tiles[0].Suit;
    for (const FGenericTile& Tile : Hand.Tiles)
    {
        if (Tile.Suit != FirstSuit)
            return false;
    }
    return true;
}

void UBP_GuiyangGameRules::SetRoomRulesConfig(const FRoomGameRulesConfig& Config)
{
    RulesConfig = Config;
    UE_LOG(LogTemp, Log, TEXT("[GuiyangGameRules] Room rules config updated: %s"), *Config.GetRulesDescription());
}
