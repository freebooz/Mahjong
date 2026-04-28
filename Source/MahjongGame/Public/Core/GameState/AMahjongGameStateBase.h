//==============================================================================
// 麻将游戏 - 游戏状态基类
// AMahjongGameStateBase.h
//
// 本文件定义游戏状态的抽象基类
// 所有麻将游戏变体的游戏状态应继承此类
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/Types/FGameContext.h"
#include "Core/Types/FGenericTile.h"
#include "Core/Types/MahjongEnums.h"
#include "Core/Interfaces/IMahjongGameRules.h"

// 前向声明
class AMahjongPlayerStateBase;
class UTilePoolComponent;
class UTurnComponent;

#include "AMahjongGameStateBase.generated.h"

//==============================================================================
// 委托定义
//==============================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnChanged, int32, NewTurnPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTileDrawn, int32, PlayerIndex, int32, TileID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTileDiscarded, int32, PlayerIndex, int32, TileID, int32, DiscardIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHuResult, int32, WinnerIndex, int32, Score, int32, HuType, const FString&, HuTypeName);

//==============================================================================
// 游戏状态基类
// 负责管理游戏全局状态
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API AMahjongGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    //============================================================================
    // 获取游戏上下文
    // 返回当前游戏的完整状态信息
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual FGameContext GetGameContext() const;

    //============================================================================
    // 获取当前回合玩家
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual int32 GetCurrentTurnPlayer() const;

    //============================================================================
    // 获取牌堆剩余数量
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual int32 GetTilePoolCount() const;

    //============================================================================
    // 摸牌
    // 参数：PlayerIndex - 玩家索引
    // 返回：摸到的牌ID
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual int32 DrawTile(int32 PlayerIndex);

    //============================================================================
    // 出牌
    // 参数：PlayerIndex - 玩家索引，TileIndex - 手牌索引
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual void DiscardTile(int32 PlayerIndex, int32 TileIndex);

    //============================================================================
    // 获取玩家状态
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual AMahjongPlayerStateBase* GetMahjongPlayerState(int32 PlayerIndex) const;

    //============================================================================
    // 获取游戏规则对象
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual TScriptInterface<IIMahjongGameRules> GetGameRules() const;

    //============================================================================
    // 获取弃牌池
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual TArray<int32> GetDiscardPool() const;

    //============================================================================
    // 设置游戏阶段
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual void SetGamePhase(EGamePhase NewPhase);

    //============================================================================
    // 获取游戏阶段
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual EGamePhase GetGamePhase() const;

    //============================================================================
    // 检查游戏是否结束
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual bool IsGameOver() const;

    //============================================================================
    // 获取玩家数量
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual int32 GetPlayerCount() const;

    //============================================================================
    // 获取手牌ID列表
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameState")
    virtual TArray<int32> GetHandTileIDs(int32 PlayerIndex) const;

    //============================================================================
    // 广播：回合改变
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|GameState")
    FOnTurnChanged OnTurnChanged;

    //============================================================================
    // 广播：摸牌
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|GameState")
    FOnTileDrawn OnTileDrawn;

    //============================================================================
    // 广播：出牌
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|GameState")
    FOnTileDiscarded OnTileDiscarded;

    //============================================================================
    // 广播：游戏阶段改变
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|GameState")
    FOnGamePhaseChanged OnGamePhaseChanged;

    //============================================================================
    // 广播：胡牌结果
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|GameState")
    FOnHuResult OnHuResult;

protected:
    //============================================================================
    // 游戏阶段
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameState")
    EGamePhase GamePhase;

    //============================================================================
    // 当前回合玩家索引
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameState")
    int32 CurrentTurnPlayer;

    //============================================================================
    // 弃牌池
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameState")
    TArray<int32> DiscardPool;

    //============================================================================
    // 翻牌鸡值（贵阳麻将特有）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameState")
    int32 FanPaiJiValue;

public:
    //============================================================================
    // 游戏规则对象（通过蓝图或C++设置）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameState")
    TScriptInterface<IIMahjongGameRules> GameRules;

    //============================================================================
    // 牌堆组件
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameState")
    UTilePoolComponent* TilePoolComponent;

    //============================================================================
    // 回合组件
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameState")
    UTurnComponent* TurnComponent;
};
