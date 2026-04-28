//==============================================================================
// 麻将游戏 - 回合组件
// UTurnComponent.h
//
// 本文件定义回合管理组件
// 负责管理游戏回合流程
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTurnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnComponentChanged, int32, NewTurnPlayer);

//==============================================================================
// 回合组件
// 管理当前回合玩家和回合切换
//==============================================================================
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAHJONGGAME_API UTurnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTurnComponent();

    //============================================================================
    // 开始回合
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Turn")
    void StartTurn(int32 PlayerIndex);

    //============================================================================
    // 结束回合
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Turn")
    void EndTurn();

    //============================================================================
    // 推进到下一个玩家
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Turn")
    void AdvanceTurn();

    //============================================================================
    // 获取当前回合玩家
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Turn")
    int32 GetCurrentTurnPlayer() const { return CurrentTurnPlayer; }

    //============================================================================
    // 获取玩家数量
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Turn")
    int32 GetPlayerCount() const { return PlayerCount; }

    //============================================================================
    // 设置玩家数量
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Turn")
    void SetPlayerCount(int32 Count) { PlayerCount = Count; }

    //============================================================================
    // 广播：回合改变
    //============================================================================
    UPROPERTY(BlueprintAssignable, Category="Mahjong|Turn")
    FOnTurnComponentChanged OnTurnChanged;

protected:
    //============================================================================
    // 当前回合玩家索引
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Turn")
    int32 CurrentTurnPlayer;

    //============================================================================
    // 玩家数量
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Turn")
    int32 PlayerCount;

    //============================================================================
    // 回合号
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Turn")
    int32 TurnNumber;
};
