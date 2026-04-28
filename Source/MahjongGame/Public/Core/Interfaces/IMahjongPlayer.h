//==============================================================================
// 麻将游戏 - 玩家接口
// IMahjongPlayer.h
//
// 本文件定义玩家相关的抽象接口
// 用于解耦玩家状态和游戏逻辑
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Core/Types/FGenericHand.h"
#include "Core/Types/MahjongEnums.h"
#include "UObject/Interface.h"

// 前向声明
class AMahjongPlayerStateBase;

#include "IMahjongPlayer.generated.h"

//==============================================================================
// 玩家接口
// 定义玩家状态和操作的抽象
//==============================================================================
UINTERFACE(MinimalAPI, BlueprintType)
class UIMahjongPlayer : public UInterface
{
    GENERATED_BODY()
};

class MAHJONGGAME_API IIMahjongPlayer
{
    GENERATED_BODY()

public:
    //============================================================================
    // 获取玩家手牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    FGenericHand GetHand() const;

    //============================================================================
    // 添加牌到手牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    void AddTile(const FGenericTile& Tile);

    //============================================================================
    // 从手牌移除牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    bool RemoveTile(int32 TileID);

    //============================================================================
    // 获取玩家名称
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    FString GetPlayerName() const;

    //============================================================================
    // 获取玩家索引
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    int32 GetPlayerIndex() const;

    //============================================================================
    // 检查是否庄家
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    bool IsZhuang() const;

    //============================================================================
    // 检查是否听牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    bool IsTing() const;

    //============================================================================
    // 获取听牌列表
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Player")
    TArray<int32> GetTingTileValues() const;
};
