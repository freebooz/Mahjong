//==============================================================================
// 麻将游戏 - 房间游戏规则配置
// RoomGameRulesConfig.h
//
// 本文件定义房间创建时可配置的游戏规则
// 房间创建者可以在创建房间时设置各项规则
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "RoomGameRulesConfig.generated.h"

//==============================================================================
// 房间游戏规则配置
// 包含所有可自定义的游戏规则设置
//==============================================================================
USTRUCT(BlueprintType)
struct MAHJONGGAME_API FRoomGameRulesConfig
{
    GENERATED_BODY()

public:
    //============================================================================
    // 构造函数
    //============================================================================
    FRoomGameRulesConfig()
    {
        // 贵阳捉鸡麻将默认规则
        bEnableHeiBa = true;           // 默认启用黑八
        bEnableChongFengJi = true;    // 默认启用冲锋鸡
        bEnableZeRenJi = true;        // 默认启用责任鸡
        bEnableJinJi = true;          // 默认启用金鸡
        bEnableFanPaiJi = true;      // 默认启用翻牌鸡

        // 分数配置
        BaseScore = 1;               // 基础分数
        MaxDou = 10;                 // 最大豆数

        // 游戏人数
        PlayerCount = 4;

        // 房间密码（为空表示无密码）
        RoomPassword = TEXT("");
    }

    //============================================================================
    // 规则名称
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    FString RuleName;

    //============================================================================
    // 黑八开关
    // 启用后：玩家摸到黑八（发财）可额外获得番数
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    bool bEnableHeiBa;

    //============================================================================
    // 冲锋鸡开关
    // 启用后：玩家打出的第一张幺鸡为冲锋鸡，可额外获得番数
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    bool bEnableChongFengJi;

    //============================================================================
    // 责任鸡开关
    // 启用后：玩家打出的第二张幺鸡为责任鸡，需额外付出番数
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    bool bEnableZeRenJi;

    //============================================================================
    // 金鸡开关
    // 启用后：根据翻牌确定的鸡牌为金鸡，可额外获得番数
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    bool bEnableJinJi;

    //============================================================================
    // 翻牌鸡开关
    // 启用后：翻牌确定的鸡牌可额外获得番数
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    bool bEnableFanPaiJi;

    //============================================================================
    // 基础分数
    // 每局游戏的基础分数
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    int32 BaseScore;

    //============================================================================
    // 最大豆数
    // 单局游戏最大豆数限制
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    int32 MaxDou;

    //============================================================================
    // 玩家数量
    // 游戏人数（2-4人）
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    int32 PlayerCount;

    //============================================================================
    // 房间密码
    // 为空表示无密码，任何人都可进入
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rules")
    FString RoomPassword;

    //============================================================================
    // 获取规则描述字符串
    //============================================================================
    FString GetRulesDescription() const
    {
        FString Desc = TEXT("规则: ");
        if (bEnableHeiBa) Desc += TEXT("黑八 ");
        if (bEnableChongFengJi) Desc += TEXT("冲锋鸡 ");
        if (bEnableZeRenJi) Desc += TEXT("责任鸡 ");
        if (bEnableJinJi) Desc += TEXT("金鸡 ");
        if (bEnableFanPaiJi) Desc += TEXT("翻牌鸡 ");
        Desc += FString::Printf(TEXT("底分%d 最大豆数%d"), BaseScore, MaxDou);
        return Desc;
    }

    //============================================================================
    // 计算鸡牌总分
    // 根据启用的规则计算鸡牌的总番数
    //============================================================================
    int32 CalculateChickenBonus(bool bHasYaoJi, bool bIsChongFengJi, bool bIsZeRenJi, int32 JinJiCount, int32 FanPaiJiCount) const
    {
        int32 Bonus = 0;

        // 幺鸡
        if (bHasYaoJi) Bonus += 1;

        // 冲锋鸡
        if (bEnableChongFengJi && bIsChongFengJi) Bonus += 1;

        // 责任鸡
        if (bEnableZeRenJi && bIsZeRenJi) Bonus += 1;

        // 金鸡
        if (bEnableJinJi) Bonus += JinJiCount;

        // 翻牌鸡
        if (bEnableFanPaiJi) Bonus += FanPaiJiCount;

        return Bonus;
    }
};