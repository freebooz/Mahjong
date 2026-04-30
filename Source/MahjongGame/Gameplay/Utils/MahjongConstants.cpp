#include "Gameplay/Utils/MahjongConstants.h"

UMahjongConstants::UMahjongConstants()
{
    // 这里可以显式初始化（可选，因为已经在声明时初始化）
    DefaultPlayerCount = 4;
    TotalTilesCount = 144;
    InitialHandSize = 13;
    CompleteHandSize = 14;
    DefaultTurnTimeout = 15.0f;
    DefaultServerPort = 7777;
    ConnectionTimeout = 30.0f;
    DefaultSfxVolume = 0.8f;
    DefaultMusicVolume = 0.5f;
}