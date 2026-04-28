//==============================================================================
// 麻将游戏 - 玩家控制器类
// MahjongPlayerController.h
//
// 本文件定义玩家控制器
// 负责处理玩家输入、管理UI、发送网络消息等
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/Types/MahjongEnums.h"
#include "MahjongPlayerController.generated.h"

//==============================================================================
// 玩家控制器类
// 负责管理玩家与游戏的交互
//==============================================================================
UCLASS()
class MAHJONGGAME_API AMahjongPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMahjongPlayerController();

    // Event handlers - called by MahjongGameWidget
    void OnDrawTileClicked();
    void OnDiscardTileClicked(int32 TileIndex);
    void OnPengClicked();
    void OnGangClicked(EDouType GangType);
    void OnHuClicked();
    void OnGuoClicked();
    void OnBaoTingClicked();
    void OnTileSelected(int32 TileIndex);

protected:
};
