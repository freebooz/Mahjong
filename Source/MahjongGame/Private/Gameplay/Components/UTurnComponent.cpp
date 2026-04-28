//==============================================================================
// 麻将游戏 - 回合组件实现
// UTurnComponent.cpp
//==============================================================================

#include "Gameplay/Components/UTurnComponent.h"

UTurnComponent::UTurnComponent()
    : CurrentTurnPlayer(0)
    , PlayerCount(4)
    , TurnNumber(0)
{}

void UTurnComponent::StartTurn(int32 PlayerIndex)
{
    CurrentTurnPlayer = PlayerIndex;
    TurnNumber++;
    OnTurnChanged.Broadcast(CurrentTurnPlayer);
}

void UTurnComponent::EndTurn()
{
    AdvanceTurn();
}

void UTurnComponent::AdvanceTurn()
{
    CurrentTurnPlayer = (CurrentTurnPlayer + 1) % PlayerCount;
    TurnNumber++;
    OnTurnChanged.Broadcast(CurrentTurnPlayer);
}
