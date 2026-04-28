#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MahjongConstants.generated.h"

// ===========================
// 游戏状态与操作枚举（移到全局作用域）
// ===========================

/** 游戏状态枚举 */
UENUM(BlueprintType)
enum class EMahjongGameState : uint8
{
    WaitingForPlayers  UMETA(DisplayName = "等待玩家加入"),
    GameInProgress     UMETA(DisplayName = "游戏进行中"),
    GameOver           UMETA(DisplayName = "游戏结束")
};

/** 玩家操作类型枚举 */
UENUM(BlueprintType)
enum class EMahjongPlayerActionType : uint8
{
    Pass              UMETA(DisplayName = "过"),
    Draw              UMETA(DisplayName = "摸牌"),
    Discard           UMETA(DisplayName = "打牌"),
    Chow              UMETA(DisplayName = "吃"),
    Pong              UMETA(DisplayName = "碰"),
    Kong              UMETA(DisplayName = "杠"),
    Win               UMETA(DisplayName = "胡牌")
};

/**
 * @class UMahjongConstants
 * @brief 麻将游戏中使用的核心常量和枚举定义
 * 该类包含游戏规则常量、状态枚举和音频默认值，供服务器和客户端共享
 */
UCLASS(BlueprintType)
class MAHJONGGAME_API UMahjongConstants : public UObject
{
    GENERATED_BODY()

public:
    UMahjongConstants(); // 构造函数声明

    // ===========================
    // 游戏规则相关常量
    // ===========================

    /** 默认玩家数量 */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|游戏规则")
    int32 DefaultPlayerCount = 4;

    /** 每局标准麻将牌数量 */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|游戏规则")
    int32 TotalTilesCount = 144;

    /** 每位玩家初始手牌数量 */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|游戏规则")
    int32 InitialHandSize = 13;

    /** 完整手牌数量（包括自摸牌） */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|游戏规则")
    int32 CompleteHandSize = 14;

    /** 游戏超时时间（秒） */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|游戏规则")
    float DefaultTurnTimeout = 15.0f;

    // ===========================
    // 网络相关常量
    // ===========================

    /** 服务器默认端口号 */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|网络")
    int32 DefaultServerPort = 7777;

    /** 网络超时时间（秒） */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|网络")
    float ConnectionTimeout = 30.0f;

    // ===========================
    // 音频相关常量
    // ===========================

    /** 游戏音效音量默认值 */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|音频")
    float DefaultSfxVolume = 0.8f;

    /** 游戏背景音乐音量默认值 */
    UPROPERTY(BlueprintReadOnly, Category = "麻将|音频")
    float DefaultMusicVolume = 0.5f;
};