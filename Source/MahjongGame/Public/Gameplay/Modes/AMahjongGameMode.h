//==============================================================================
// 麻将游戏 - 游戏模式基类
// AMahjongGameMode.h
//
// 本文件定义游戏模式的抽象基类
// 所有麻将游戏变体的游戏模式应继承此类
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Types/FGenericHand.h"
#include "Core/Types/MahjongEnums.h"
#include "Core/Interfaces/IMahjongGameRules.h"
#include "Gameplay/Rules/UMahjongGameRules.h"

// 前向声明
class AMahjongGameStateBase;
class AMahjongPlayerStateBase;
class AMahjongPlayerController;

#include "AMahjongGameMode.generated.h"

//==============================================================================
// 游戏模式基类
// 负责管理游戏流程初始化和玩家生成
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API AMahjongGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    //============================================================================
    // 获取游戏规则对象
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameMode")
    virtual TScriptInterface<IIMahjongGameRules> GetGameRules() const;

    //============================================================================
    // 获取游戏状态
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameMode")
    virtual AMahjongGameStateBase* GetMahjongGameState() const;

    //============================================================================
    // 开始游戏
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameMode")
    virtual void StartGame();

    //============================================================================
    // 结束游戏
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameMode")
    virtual void EndGame();

    //============================================================================
    // 获取玩家控制器类
    // 子类应重写此函数返回特定游戏变体的控制器类
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameMode")
    virtual TSubclassOf<AMahjongPlayerController> GetMahjongPlayerControllerClass() const;

    //============================================================================
    // 获取玩家状态类
    // 子类应重写此函数返回特定游戏变体的状态类
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|GameMode")
    virtual TSubclassOf<AMahjongPlayerStateBase> GetMahjongPlayerStateClass() const;

protected:
    //============================================================================
    // 游戏规则对象（通过蓝图或C++设置）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameMode")
    TScriptInterface<IIMahjongGameRules> GameRules;

    //============================================================================
    // 玩家控制器类
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameMode")
    TSubclassOf<AMahjongPlayerController> MahjongPlayerControllerClass;

    //============================================================================
    // 玩家状态类
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|GameMode")
    TSubclassOf<AMahjongPlayerStateBase> MahjongPlayerStateClass;
};
