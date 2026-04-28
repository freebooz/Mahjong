//==============================================================================
// 麻将游戏 - 游戏规则基类
// UMahjongGameRules.h
//
// 本文件定义游戏规则的抽象基类
// 通过BlueprintNativeEvent允许蓝图重写实现不同游戏变体
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Core/Types/FGenericHand.h"
#include "Core/Types/FGenericTile.h"
#include "Core/Interfaces/IMahjongGameRules.h"

#include "UMahjongGameRules.generated.h"

//==============================================================================
// 游戏规则基类
// 提供默认实现，允许蓝图子类重写特定函数
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UMahjongGameRules : public UObject, public IIMahjongGameRules
{
    GENERATED_BODY()

public:
    //============================================================================
    // 获取游戏名称
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    FString GetGameName() const;

    //============================================================================
    // 获取牌堆数量
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    int32 GetTilePoolCount() const;

    //============================================================================
    // 获取玩家数量
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    int32 GetPlayerCount() const;

    //============================================================================
    // 获取初始手牌数
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    int32 GetInitialHandCount() const;

    //============================================================================
    // 检查是否可以碰牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool CanPeng(const FGenericHand& Hand, int32 TileValue) const;

    //============================================================================
    // 检查是否可以杠牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool CanGang(const FGenericHand& Hand, int32 TileValue) const;

    //============================================================================
    // 检查是否可以胡牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool CanHu(const FGenericHand& Hand, int32 TileValue) const;

    //============================================================================
    // 获取可用动作列表
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    FAvailableActions GetAvailableActions(int32 PlayerIndex, const FGenericHand& Hand, int32 LastDiscardTile) const;

    //============================================================================
    // 计算胡牌分数
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    int32 CalculateHuScore(const FGenericHand& Hand, int32 HuType) const;

    //============================================================================
    // 获取胡牌类型名称
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    FString GetHuTypeName(int32 HuType) const;

    //============================================================================
    // 获取听牌列表
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    TArray<int32> GetTingTiles(const FGenericHand& Hand) const;

    //============================================================================
    // 获取牌面纹理
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    UTexture2D* GetTileTexture(int32 TileValue) const;

    //============================================================================
    // 初始化牌堆
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    TArray<int32> InitializeTilePool() const;

    //============================================================================
    // 检查是否为鸡牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool IsChickenTile(int32 TileValue) const;

protected:
    //============================================================================
    // 默认牌堆数量
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Rules|Defaults")
    int32 DefaultTilePoolCount;

    //============================================================================
    // 默认玩家数量
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Rules|Defaults")
    int32 DefaultPlayerCount;

    //============================================================================
    // 默认初始手牌数
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Rules|Defaults")
    int32 DefaultInitialHandCount;
};
