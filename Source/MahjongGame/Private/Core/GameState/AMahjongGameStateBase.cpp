//==============================================================================
// 麻将游戏 - 游戏状态基类实现
// AMahjongGameStateBase.cpp
//==============================================================================

#include "Core/GameState/AMahjongGameStateBase.h"
#include "Core/PlayerState/AMahjongPlayerStateBase.h"
#include "Gameplay/Components/UTilePoolComponent.h"
#include "Gameplay/Components/UTurnComponent.h"

void AMahjongGameStateBase::SetGamePhase(EGamePhase NewPhase)
{
    if (GamePhase != NewPhase)
    {
        GamePhase = NewPhase;
        OnGamePhaseChanged.Broadcast(NewPhase);
    }
}

EGamePhase AMahjongGameStateBase::GetGamePhase() const
{
    return GamePhase;
}

int32 AMahjongGameStateBase::GetCurrentTurnPlayer() const
{
    return CurrentTurnPlayer;
}

int32 AMahjongGameStateBase::GetTilePoolCount() const
{
    if (TilePoolComponent)
    {
        return TilePoolComponent->GetRemainingCount();
    }
    return 0;
}

int32 AMahjongGameStateBase::DrawTile(int32 PlayerIndex)
{
    if (!TilePoolComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AMahjongGameStateBase] TilePoolComponent is null"));
        return -1;
    }

    int32 TileID = TilePoolComponent->DrawTile();
    if (TileID >= 0)
    {
        OnTileDrawn.Broadcast(PlayerIndex, TileID);
    }
    return TileID;
}

void AMahjongGameStateBase::DiscardTile(int32 PlayerIndex, int32 TileIndex)
{
    AMahjongPlayerStateBase* PlayerState = GetMahjongPlayerState(PlayerIndex);
    if (!PlayerState)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AMahjongGameStateBase] PlayerState is null for index %d"), PlayerIndex);
        return;
    }

    // 从玩家手牌获取牌
    // 这里简化处理，实际需要根据TileIndex获取具体牌ID
    TArray<int32> HandTiles = PlayerState->GetHandTileIDs();
    if (TileIndex >= 0 && TileIndex < HandTiles.Num())
    {
        int32 TileID = HandTiles[TileIndex];
        DiscardPool.Add(TileID);
        PlayerState->RemoveHandTile(TileID);
        OnTileDiscarded.Broadcast(PlayerIndex, TileID, DiscardPool.Num() - 1);
    }
}

FGameContext AMahjongGameStateBase::GetGameContext() const
{
    FGameContext Context;
    Context.Phase = GamePhase;
    Context.CurrentTurnPlayer = CurrentTurnPlayer;
    Context.RemainingTileCount = GetTilePoolCount();
    Context.DiscardPoolIDs = DiscardPool;
    Context.FanPaiJiValue = FanPaiJiValue;
    return Context;
}

AMahjongPlayerStateBase* AMahjongGameStateBase::GetMahjongPlayerState(int32 PlayerIndex) const
{
    if (PlayerArray.IsValidIndex(PlayerIndex))
    {
        return Cast<AMahjongPlayerStateBase>(PlayerArray[PlayerIndex]);
    }
    return nullptr;
}

TScriptInterface<IIMahjongGameRules> AMahjongGameStateBase::GetGameRules() const
{
    return GameRules;
}

TArray<int32> AMahjongGameStateBase::GetDiscardPool() const
{
    return DiscardPool;
}

bool AMahjongGameStateBase::IsGameOver() const
{
    return GamePhase == EGamePhase::GameOver;
}

int32 AMahjongGameStateBase::GetPlayerCount() const
{
    return PlayerArray.Num();
}

TArray<int32> AMahjongGameStateBase::GetHandTileIDs(int32 PlayerIndex) const
{
    AMahjongPlayerStateBase* PlayerState = GetMahjongPlayerState(PlayerIndex);
    if (PlayerState)
    {
        return PlayerState->GetHandTileIDs();
    }
    return TArray<int32>();
}
