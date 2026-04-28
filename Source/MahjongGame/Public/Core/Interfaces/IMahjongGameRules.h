//==============================================================================
// 麻将游戏 - 游戏规则接口
// IMahjongGameRules.h
//
// 本文件定义游戏规则的抽象接口
// 所有麻将游戏变体必须实现此接口
// 通过BlueprintNativeEvent在蓝图层实现多态
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Core/Types/FGenericHand.h"
#include "Core/Types/FGenericTile.h"
#include "Core/Types/MahjongEnums.h"
#include "UObject/Interface.h"

// 前向声明
class UTexture2D;

#include "IMahjongGameRules.generated.h"

//==============================================================================
// 可用动作结构
//==============================================================================
USTRUCT(BlueprintType)
struct FAvailableActions
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanPeng;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanGang;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanHu;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanGuo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanTing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanZiMo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanDianPao;
};

//==============================================================================
// 胡牌结果结构
//==============================================================================
USTRUCT(BlueprintType)
struct FHuResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHu;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HuType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HuTypeName;
};

//==============================================================================
// 游戏规则接口
// 通过此接口定义不同游戏变体的规则差异
//==============================================================================
UINTERFACE(MinimalAPI, BlueprintType)
class UIMahjongGameRules : public UInterface
{
    GENERATED_BODY()
};

class MAHJONGGAME_API IIMahjongGameRules
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
    // 返回初始牌堆有多少张牌
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
    // 参数：Hand - 手牌信息，TileValue - 要碰的牌值
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool CanPeng(const FGenericHand& Hand, int32 TileValue) const;

    //============================================================================
    // 检查是否可以杠牌
    // 参数：Hand - 手牌信息，TileValue - 要杠的牌值
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool CanGang(const FGenericHand& Hand, int32 TileValue) const;

    //============================================================================
    // 检查是否可以胡牌
    // 参数：Hand - 手牌信息，TileValue - 要胡的牌值
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool CanHu(const FGenericHand& Hand, int32 TileValue) const;

    //============================================================================
    // 获取可用动作列表
    // 参数：PlayerIndex - 玩家索引，LastDiscardTile - 最后打出的牌
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    FAvailableActions GetAvailableActions(int32 PlayerIndex, const FGenericHand& Hand, int32 LastDiscardTile) const;

    //============================================================================
    // 计算胡牌分数
    // 参数：Hand - 手牌信息，HuType - 胡牌类型
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    int32 CalculateHuScore(const FGenericHand& Hand, int32 HuType) const;

    //============================================================================
    // 检查胡牌类型
    // 返回胡牌类型名称
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    FString GetHuTypeName(int32 HuType) const;

    //============================================================================
    // 获取听牌列表
    // 参数：Hand - 手牌信息
    // 返回可以胡的牌值列表
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    TArray<int32> GetTingTiles(const FGenericHand& Hand) const;

    //============================================================================
    // 获取牌面纹理
    // 参数：TileValue - 牌值
    // 返回对应牌的纹理（蓝图可重写返回自定义纹理）
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    UTexture2D* GetTileTexture(int32 TileValue) const;

    //============================================================================
    // 初始化牌堆
    // 生成所有牌的ID列表
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    TArray<int32> InitializeTilePool() const;

    //============================================================================
    // 检查是否为鸡牌
    // 参数：TileValue - 牌值
    //============================================================================
    UFUNCTION(BlueprintNativeEvent, Category="Mahjong|Rules")
    bool IsChickenTile(int32 TileValue) const;
};
