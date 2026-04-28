//==============================================================================
// 麻将游戏 - 贵阳麻将规则Blueprint基类
// UBP_GuiyangGameRules.h
//
// 本文件定义贵阳麻将规则的Blueprint基类
// 继承自UMahjongGameRules，实现贵阳捉鸡麻将特有规则
// 通过Blueprint重写实现多态
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Gameplay/Rules/UMahjongGameRules.h"
#include "Gameplay/Room/RoomGameRulesConfig.h"
#include "UBP_GuiyangGameRules.generated.h"

//==============================================================================
// 贵阳麻将规则Blueprint基类
// 实现鸡牌、豆计算等贵阳特有规则
//==============================================================================
UCLASS(Blueprintable)
class MAHJONGGAME_API UBP_GuiyangGameRules : public UMahjongGameRules
{
    GENERATED_BODY()

public:
    //============================================================================
    // 获取游戏名称
    //============================================================================
    FString GetGameName_Implementation() const override;

    //============================================================================
    // 检查是否为鸡牌
    // 贵阳麻将：幺鸡(1条)、翻牌鸡、金鸡
    //============================================================================
    bool IsChickenTile_Implementation(int32 TileValue) const override;

    //============================================================================
    // 计算鸡牌分数
    // 包含幺鸡、冲锋鸡、责任鸡、翻牌鸡、金鸡的番数计算
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    int32 CalculateChickenScore(bool bHasYaoJi, bool bIsChongFengJi, bool bIsZeRenJi) const;

    //============================================================================
    // 计算豆的分数
    // 闷豆1番、转弯豆2番、点豆3番
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    int32 CalculateDouScore(int32 MenDouCount, int32 ZhuanWanDouCount, int32 DianDouCount) const;

    //============================================================================
    // 检查是否为大对子
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    bool IsDaDuiZi(const FGenericHand& Hand) const;

    //============================================================================
    // 检查是否为七对
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    bool IsQiDui(const FGenericHand& Hand) const;

    //============================================================================
    // 检查是否为龙七对
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    bool IsLongQiDui(const FGenericHand& Hand) const;

    //============================================================================
    // 检查是否为一色牌
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    bool IsYiSe(const FGenericHand& Hand) const;

    //============================================================================
    // 设置翻牌鸡值
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    void SetFanPaiJiValue(int32 Value) { FanPaiJiValue = Value; }

    //============================================================================
    // 获取翻牌鸡值
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Guiyang|Rules")
    int32 GetFanPaiJiValue() const { return FanPaiJiValue; }

    //============================================================================
    // 设置冲锋鸡标志（玩家打出的第一张幺鸡）
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    void SetChongFengJi(bool bValue) { bIsChongFengJi = bValue; }

    //============================================================================
    // 获取冲锋鸡标志
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Guiyang|Rules")
    bool IsChongFengJi() const { return bIsChongFengJi; }

    //============================================================================
    // 设置责任鸡标志（玩家打出的第二张幺鸡）
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    void SetZeRenJi(bool bValue) { bIsZeRenJi = bValue; }

    //============================================================================
    // 获取责任鸡标志
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Guiyang|Rules")
    bool IsZeRenJi() const { return bIsZeRenJi; }

    //============================================================================
    // 添加金鸡
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    void AddJinJi(int32 TileValue) { if (!JinJiValues.Contains(TileValue)) JinJiValues.Add(TileValue); }

    //============================================================================
    // 清空金鸡列表
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    void ClearJinJi() { JinJiValues.Empty(); }

    //============================================================================
    // 设置房间规则配置
    // 根据房间设置启用/禁用各项规则
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Guiyang|Rules")
    void SetRoomRulesConfig(const FRoomGameRulesConfig& Config);

    //============================================================================
    // 获取房间规则配置
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Guiyang|Rules")
    FRoomGameRulesConfig GetRoomRulesConfig() const { return RulesConfig; }

protected:
    //============================================================================
    // 翻牌鸡值
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guiyang|Rules")
    int32 FanPaiJiValue;

    //============================================================================
    // 幺鸡牌值（默认1条 = 11）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guiyang|Rules")
    int32 YaoJiValue;

    //============================================================================
    // 金鸡牌值列表
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guiyang|Rules")
    TArray<int32> JinJiValues;

    //============================================================================
    // 冲锋鸡标志（玩家打出的第一张幺鸡）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guiyang|Rules")
    bool bIsChongFengJi;

    //============================================================================
    // 责任鸡标志（玩家打出的第二张幺鸡）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guiyang|Rules")
    bool bIsZeRenJi;

    //============================================================================
    // 房间规则配置
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guiyang|Rules")
    FRoomGameRulesConfig RulesConfig;
};