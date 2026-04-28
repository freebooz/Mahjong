//==============================================================================
// 麻将游戏 - 游戏模式基类实现
// AMahjongGameMode.cpp
//==============================================================================

#include "Gameplay/Modes/AMahjongGameMode.h"
#include "Core/GameState/AMahjongGameStateBase.h"
#include "Core/PlayerState/AMahjongPlayerStateBase.h"
#include "Gameplay/Game/MahjongPlayerController.h"
#include "Gameplay/Components/UTilePoolComponent.h"

TScriptInterface<IIMahjongGameRules> AMahjongGameMode::GetGameRules() const
{
    return GameRules;
}

AMahjongGameStateBase* AMahjongGameMode::GetMahjongGameState() const
{
    return Cast<AMahjongGameStateBase>(GameState);
}

void AMahjongGameMode::StartGame()
{
    UE_LOG(LogTemp, Log, TEXT("[AMahjongGameMode] StartGame"));

    AMahjongGameStateBase* MahjongGS = GetMahjongGameState();
    if (MahjongGS && GameRules.GetInterface())
    {
        // 初始化牌堆
        TArray<int32> InitialTiles = GameRules.GetInterface()->Execute_InitializeTilePool(GameRules.GetObject());
        if (MahjongGS->TilePoolComponent)
        {
            MahjongGS->TilePoolComponent->InitializePool(InitialTiles);
        }

        // 设置游戏阶段
        MahjongGS->SetGamePhase(EGamePhase::Playing);
    }
}

void AMahjongGameMode::EndGame()
{
    UE_LOG(LogTemp, Log, TEXT("[AMahjongGameMode] EndGame"));

    AMahjongGameStateBase* MahjongGS = GetMahjongGameState();
    if (MahjongGS)
    {
        MahjongGS->SetGamePhase(EGamePhase::GameOver);
    }
}

TSubclassOf<AMahjongPlayerController> AMahjongGameMode::GetMahjongPlayerControllerClass() const
{
    return MahjongPlayerControllerClass;
}

TSubclassOf<AMahjongPlayerStateBase> AMahjongGameMode::GetMahjongPlayerStateClass() const
{
    return MahjongPlayerStateClass;
}
