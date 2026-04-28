#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Mahjong/MahjongGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "SichuanMahjongWidget.generated.h"

//==============================================================================
// 区域扩展层 - 四川麻将游戏界面
// 在通用麻将游戏界面上添加四川麻将特有功能
// 特有功能：血战到底、叫牌、刮风下雨
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API USichuanMahjongWidget : public UMahjongGameWidget
{
    GENERATED_BODY()

public:
    // 四川麻将特有数据
    USTRUCT(BlueprintType)
    struct FSichuanMahjongData
    {
        GENERATED_BODY()

        UPROPERTY()
        int32 CalledTileValue;

        UPROPERTY()
        bool bIsBaoJiao;

        UPROPERTY()
        int32 WindRainBonus;
    };

    // 构造函数
    USichuanMahjongWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化四川麻将
    UFUNCTION(BlueprintCallable, Category="Sichuan|Game")
    void InitializeSichuanMahjong();

    // 设置叫牌
    UFUNCTION(BlueprintCallable, Category="Sichuan|Game")
    void SetCalledTile(int32 TileValue);

    // 获取叫牌值
    UFUNCTION(BlueprintCallable, Category="Sichuan|Game")
    int32 GetCalledTileValue() const;

    // 刮风下雨奖励更新
    UFUNCTION(BlueprintCallable, Category="Sichuan|Game")
    void UpdateWindRainBonus(int32 Bonus);

    // 报叫按钮点击
    UFUNCTION(BlueprintCallable, Category="Sichuan|Game")
    void OnBaoJiaoClicked();

    // 不报按钮点击
    UFUNCTION(BlueprintCallable, Category="Sichuan|Game")
    void OnNoBaoJiaoClicked();

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 报叫提示面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* BaoJiaoPromptPanel;

    // 叫牌提示文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* CalledTilePromptText;

    // 报叫按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* BaoJiaoButton;

    // 不报按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NoBaoJiaoButton;

    // 刮风下雨显示
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* WindRainBonusText;

    // 四川麻将特有数据
    UPROPERTY()
    FSichuanMahjongData SichuanData;
};
