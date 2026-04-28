//==============================================================================
// 麻将游戏 - 玩家状态基类
// AMahjongPlayerStateBase.h
//
// 本文件定义玩家状态的抽象基类
// 所有麻将游戏变体的玩家状态应继承此类
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Core/Types/FGenericHand.h"
#include "Core/Types/FGenericTile.h"
#include "Core/Interfaces/IMahjongPlayer.h"
#include "Core/Types/MahjongEnums.h"

#include "AMahjongPlayerStateBase.generated.h"

//==============================================================================
// 玩家状态基类
// 负责管理单个玩家的状态
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API AMahjongPlayerStateBase : public APlayerState
{
    GENERATED_BODY()

public:
    //============================================================================
    // 初始化玩家
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual void InitializePlayer(int32 Index, const FString& Name);

    //============================================================================
    // 重置玩家数据（新回合）
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual void ResetForNewRound();

    //============================================================================
    // 添加手牌
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual void AddHandTile(const FGenericTile& Tile);

    //============================================================================
    // 移除手牌
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual bool RemoveHandTile(int32 TileID);

    //============================================================================
    // 获取手牌
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual FGenericHand GetHand() const;

    //============================================================================
    // 获取手牌ID列表
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual TArray<int32> GetHandTileIDs() const;

    //============================================================================
    // 添加碰牌
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual void AddPengTile(const FGenericTile& Tile);

    //============================================================================
    // 添加杠牌
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual void AddGangTile(const FGenericTile& Tile, EDouType GangType);

    //============================================================================
    // 获取总手牌数
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual int32 GetTotalTileCount() const;

    //============================================================================
    // 获取听牌列表
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual TArray<int32> GetTingTileValues() const;

    //============================================================================
    // 检查是否听牌
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual bool IsTing() const;

    //============================================================================
    // 设置听牌状态
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual void SetTing(bool bTing);

    //============================================================================
    // 设置庄家状态
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual void SetZhuang(bool bIsZhuang);

    //============================================================================
    // 获取玩家索引
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|PlayerState")
    virtual int32 GetPlayerIndex() const;

    //============================================================================
    // 获取听牌列表（接口实现）
    //============================================================================
    virtual TArray<int32> GetTingTileValues_Implementation() const;

    //============================================================================
    // 获取玩家名称（接口实现）
    //============================================================================
    virtual FString GetPlayerName_Implementation() const;

    //============================================================================
    // 获取玩家索引（接口实现）
    //============================================================================
    virtual int32 GetPlayerIndex_Implementation() const;

    //============================================================================
    // 是否庄家（接口实现）
    //============================================================================
    virtual bool IsZhuang_Implementation() const;

    //============================================================================
    // 是否听牌（接口实现）
    //============================================================================
    virtual bool IsTing_Implementation() const;

    //============================================================================
    // 添加牌（接口实现）
    //============================================================================
    virtual void AddTile_Implementation(const FGenericTile& Tile);

    //============================================================================
    // 移除牌（接口实现）
    //============================================================================
    virtual bool RemoveTile_Implementation(int32 TileID);

    //============================================================================
    // 获取手牌（接口实现）
    //============================================================================
    virtual FGenericHand GetHand_Implementation() const;

    //============================================================================
    // 是否已准备（大厅用）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|PlayerState")
    bool bIsReady;

    //============================================================================
    // 听牌列表
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|PlayerState")
    TArray<int32> TingTileValues;

    //============================================================================
    // 玩家状态
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|PlayerState")
    EPlayerState PlayerState;

protected:
    //============================================================================
    // 玩家索引
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|PlayerState")
    int32 PlayerIndex;

    //============================================================================
    // 玩家手牌
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|PlayerState")
    FGenericHand Hand;

    //============================================================================
    // 是否庄家
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|PlayerState")
    bool bIsZhuang;

    //============================================================================
    // 是否听牌
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|PlayerState")
    bool bIsTing;

};
