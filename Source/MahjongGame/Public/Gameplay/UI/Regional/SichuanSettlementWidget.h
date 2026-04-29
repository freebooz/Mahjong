#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Mahjong/GameSettlementWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "SichuanSettlementWidget.generated.h"

//==============================================================================
// 区域扩展层 - 四川麻将结算界面
// 在通用结算界面上添加四川麻将特有结算项
// 特有：血战到底番数、刮风下雨奖励、叫牌奖励
//==============================================================================
USTRUCT(BlueprintType)
struct FSichuanSettlementData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 WindRainBonus;

    UPROPERTY()
    int32 BloodBattleMultiplier;

    UPROPERTY()
    int32 BaoJiaoBonus;
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API USichuanSettlementWidget : public UGameSettlementWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    USichuanSettlementWidget(const FObjectInitializer& ObjectInitializer);

    // 显示四川麻将结算
    UFUNCTION(BlueprintCallable, Category="Sichuan|Settlement")
    void ShowSichuanSettlement(
        const TArray<FPlayerSettlement>& Players,
        const FSichuanSettlementData& SichuanData
    );

    // 获取刮风下雨奖励
    UFUNCTION(BlueprintCallable, Category="Sichuan|Settlement")
    int32 GetWindRainBonus() const;

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 刮风下雨总奖励文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* WindRainTotalText;

    // 血战场倍数文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* BloodBattleText;

    // 四川麻将扩展数据
    UPROPERTY()
    FSichuanSettlementData SichuanSettlementData;
};
