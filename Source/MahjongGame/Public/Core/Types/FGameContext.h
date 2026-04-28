//==============================================================================
// 麻将游戏 - 游戏上下文结构
// FGameContext.h
//
// 本文件定义游戏上下文结构
// 用于在游戏状态和UI之间传递完整的游戏信息
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Core/Types/FGenericHand.h"
#include "Core/Types/MahjongEnums.h"
#include "FGameContext.generated.h"

// 前向声明
class AMahjongPlayerStateBase;

//==============================================================================
// 游戏上下文结构
// 包含当前游戏的所有状态信息
//==============================================================================
USTRUCT(BlueprintType)
struct FGameContext
{
    GENERATED_BODY()

    // 当前游戏阶段
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGamePhase Phase;

    // 当前出牌玩家索引
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentTurnPlayer;

    // 牌堆剩余数量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RemainingTileCount;

    // 剩余牌ID列表（用于网络同步）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> RemainingTileIDs;

    // 弃牌池ID列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> DiscardPoolIDs;

    // 翻牌鸡牌值（贵阳麻将特有）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FanPaiJiValue;

    // 当前回合是否有人可以胡（用于显示提示）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanHuInCurrentTurn;

    // 抢杠胡标记
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bQiangGangAlert;

    //============================================================================
    // 默认构造函数
    //============================================================================
    FGameContext()
        : Phase(EGamePhase::Idle)
        , CurrentTurnPlayer(-1)
        , RemainingTileCount(0)
        , FanPaiJiValue(-1)
        , bCanHuInCurrentTurn(false)
        , bQiangGangAlert(false)
    {}

    //============================================================================
    // 是否游戏已开始
    //============================================================================
    bool IsGameStarted() const
    {
        return Phase != EGamePhase::Idle && Phase != EGamePhase::Waiting;
    }
};
