#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Mahjong/GameSettlementWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "GuizhouSettlementWidget.generated.h"

//==============================================================================
// 区域扩展层 - 贵阳麻将结算界面
// 在通用结算界面上添加贵阳麻将特有结算项
// 特有：捉鸡奖励、夏麻计算、鸡麻将结算
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UGuizhouSettlementWidget : public UGameSettlementWidget
{
    GENERATED_BODY()

public:
    // 贵阳麻将结算扩展数据
    USTRUCT(BlueprintType)
    struct FGuizhouSettlementData
    {
        GENERATED_BODY()

        UPROPERTY()
        int32 ChickenBonus;

        UPROPERTY()
        int32 XiaMaBonus;

        UPROPERTY()
        int32 TotalChickenCount;
    };

    // 构造函数
    UGuizhouSettlementWidget(const FObjectInitializer& ObjectInitializer);

    // 显示贵阳麻将结算
    UFUNCTION(BlueprintCallable, Category="Guizhou|Settlement")
    void ShowGuizhouSettlement(
        const TArray<FPlayerSettlement>& Players,
        const FGuizhouSettlementData& GuizhouData
    );

    // 获取捉鸡总奖励
    UFUNCTION(BlueprintCallable, Category="Guizhou|Settlement")
    int32 GetTotalChickenBonus() const;

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 捉鸡总奖励文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TotalChickenBonusText;

    // 夏麻总奖励文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TotalXiaMaBonusText;

    // 贵阳麻将扩展数据
    UPROPERTY()
    FGuizhouSettlementData GuizhouSettlementData;
};
