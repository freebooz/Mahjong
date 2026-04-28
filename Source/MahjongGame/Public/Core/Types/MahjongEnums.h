//==============================================================================
// 麻将游戏 - 麻将枚举定义
// MahjongEnums.h
//
// 本文件定义所有麻将游戏相关的枚举和结构体
// 属于核心层，所有游戏变体共享
//==============================================================================

#pragma once

#include "CoreMinimal.h"
#include "MahjongEnums.generated.h"

//==============================================================================
// 游戏阶段枚举
//==============================================================================
UENUM(BlueprintType)
enum class EGamePhase : uint8
{
    Idle        UMETA(DisplayName = "空闲"),
    Waiting     UMETA(DisplayName = "等待中"),
    Betting     UMETA(DisplayName = "下注中"),
    Dealing     UMETA(DisplayName = "发牌中"),
    Playing     UMETA(DisplayName = "游戏中"),
    DrawTile    UMETA(DisplayName = "摸牌"),
    DiscardTile UMETA(DisplayName = "出牌"),
    Scoring     UMETA(DisplayName = "结算"),
    GameOver    UMETA(DisplayName = "游戏结束")
};

//==============================================================================
// 玩家状态枚举
//==============================================================================
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle        UMETA(DisplayName = "空闲"),
    Ready       UMETA(DisplayName = "已准备"),
    Playing     UMETA(DisplayName = "游戏中"),
    Ting        UMETA(DisplayName = "已听牌"),
    Hu          UMETA(DisplayName = "已胡牌"),
    Liao        UMETA(DisplayName = "了")
};

//==============================================================================
// 麻将动作枚举
//==============================================================================
UENUM(BlueprintType)
enum class EMahjongAction : uint8
{
    None        UMETA(DisplayName = "无"),
    Draw        UMETA(DisplayName = "摸牌"),
    Discard     UMETA(DisplayName = "出牌"),
    Peng        UMETA(DisplayName = "碰"),
    Gang        UMETA(DisplayName = "杠"),
    Hu          UMETA(DisplayName = "胡"),
    Guo         UMETA(DisplayName = "过"),
    Ting        UMETA(DisplayName = "听"),
    BaoTing     UMETA(DisplayName = "报听"),
    QiangGang   UMETA(DisplayName = "抢杠")
};

//==============================================================================
// 豆类型枚举（贵阳麻将特有）
//==============================================================================
UENUM(BlueprintType)
enum class EDouType : uint8
{
    None        UMETA(DisplayName = "无"),
    MenDou      UMETA(DisplayName = "闷豆"),
    ZhuanWanDou UMETA(DisplayName = "转弯豆"),
    DianDou     UMETA(DisplayName = "点豆")
};

//==============================================================================
// 胡牌类型结构
//==============================================================================
USTRUCT(BlueprintType)
struct FHuType
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HuTypeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HuTypeName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BaseScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Multiple;
};

//==============================================================================
// 回合结果结构
//==============================================================================
USTRUCT(BlueprintType)
struct FRoundResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PlayerIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Score;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHu;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HuType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
};
