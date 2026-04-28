//==============================================================================
// 麻将游戏 - 玩家控制器实现
// MahjongPlayerController.cpp
//==============================================================================

#include "Gameplay/Game/MahjongPlayerController.h"

AMahjongPlayerController::AMahjongPlayerController()
{
}

void AMahjongPlayerController::OnDrawTileClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnDrawTileClicked"));
}

void AMahjongPlayerController::OnDiscardTileClicked(int32 TileIndex)
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnDiscardTileClicked: %d"), TileIndex);
}

void AMahjongPlayerController::OnPengClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnPengClicked"));
}

void AMahjongPlayerController::OnGangClicked(EDouType GangType)
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnGangClicked: %d"), (int32)GangType);
}

void AMahjongPlayerController::OnHuClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnHuClicked"));
}

void AMahjongPlayerController::OnGuoClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnGuoClicked"));
}

void AMahjongPlayerController::OnBaoTingClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnBaoTingClicked"));
}

void AMahjongPlayerController::OnTileSelected(int32 TileIndex)
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerController] OnTileSelected: %d"), TileIndex);
}
