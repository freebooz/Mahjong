//==============================================================================
// 麻将游戏 - 玩家状态基类实现
// AMahjongPlayerStateBase.cpp
//==============================================================================

#include "Core/PlayerState/AMahjongPlayerStateBase.h"

void AMahjongPlayerStateBase::InitializePlayer(int32 Index, const FString& Name)
{
    PlayerIndex = Index;
    SetPlayerName(Name);
}

void AMahjongPlayerStateBase::ResetForNewRound()
{
    Hand.Tiles.Empty();
    Hand.PengTiles.Empty();
    Hand.AnGangTiles.Empty();
    Hand.MingGangTiles.Empty();
    Hand.HandTiles.Empty();
    bIsTing = false;
    TingTileValues.Empty();
    PlayerState = EPlayerState::Ready;
}

void AMahjongPlayerStateBase::AddHandTile(const FGenericTile& Tile)
{
    Hand.Tiles.Add(Tile);
}

bool AMahjongPlayerStateBase::RemoveHandTile(int32 TileID)
{
    for (int32 i = 0; i < Hand.Tiles.Num(); ++i)
    {
        if (Hand.Tiles[i].TileID == TileID)
        {
            Hand.Tiles.RemoveAt(i);
            return true;
        }
    }
    return false;
}

FGenericHand AMahjongPlayerStateBase::GetHand() const
{
    return Hand;
}

TArray<int32> AMahjongPlayerStateBase::GetHandTileIDs() const
{
    TArray<int32> TileIDs;
    for (const FGenericTile& Tile : Hand.Tiles)
    {
        TileIDs.Add(Tile.TileID);
    }
    return TileIDs;
}

void AMahjongPlayerStateBase::AddPengTile(const FGenericTile& Tile)
{
    Hand.PengTiles.Add(Tile);
}

void AMahjongPlayerStateBase::AddGangTile(const FGenericTile& Tile, EDouType GangType)
{
    switch (GangType)
    {
        case EDouType::MenDou:
            Hand.AnGangTiles.Add(Tile);
            break;
        case EDouType::ZhuanWanDou:
        case EDouType::DianDou:
            Hand.MingGangTiles.Add(Tile);
            break;
        default:
            break;
    }
}

int32 AMahjongPlayerStateBase::GetTotalTileCount() const
{
    return Hand.GetTotalTileCount();
}

TArray<int32> AMahjongPlayerStateBase::GetTingTileValues() const
{
    return TingTileValues;
}

bool AMahjongPlayerStateBase::IsTing() const
{
    return bIsTing;
}

void AMahjongPlayerStateBase::SetTing(bool bTing)
{
    bIsTing = bTing;
}

void AMahjongPlayerStateBase::SetZhuang(bool bIsZhuangFlag)
{
    bIsZhuang = bIsZhuangFlag;
}

int32 AMahjongPlayerStateBase::GetPlayerIndex() const
{
    return PlayerIndex;
}

// _Implementation functions for IIMahjongPlayer interface
TArray<int32> AMahjongPlayerStateBase::GetTingTileValues_Implementation() const
{
    return TingTileValues;
}

FString AMahjongPlayerStateBase::GetPlayerName_Implementation() const
{
    return GetPlayerName();
}

int32 AMahjongPlayerStateBase::GetPlayerIndex_Implementation() const
{
    return PlayerIndex;
}

bool AMahjongPlayerStateBase::IsZhuang_Implementation() const
{
    return bIsZhuang;
}

bool AMahjongPlayerStateBase::IsTing_Implementation() const
{
    return bIsTing;
}

void AMahjongPlayerStateBase::AddTile_Implementation(const FGenericTile& Tile)
{
    Hand.Tiles.Add(Tile);
}

bool AMahjongPlayerStateBase::RemoveTile_Implementation(int32 TileID)
{
    for (int32 i = 0; i < Hand.Tiles.Num(); ++i)
    {
        if (Hand.Tiles[i].TileID == TileID)
        {
            Hand.Tiles.RemoveAt(i);
            return true;
        }
    }
    return false;
}

FGenericHand AMahjongPlayerStateBase::GetHand_Implementation() const
{
    return Hand;
}
