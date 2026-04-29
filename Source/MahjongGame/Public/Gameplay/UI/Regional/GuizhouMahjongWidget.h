#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Mahjong/MahjongGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "GuizhouMahjongWidget.generated.h"

//==============================================================================
// 区域扩展层 - 贵阳麻将游戏界面
// 在通用麻将游戏界面上添加贵阳麻将特有功能
// 特有功能：捉鸡、贵阳夏麻、特殊番型
//==============================================================================

// 贵阳麻将特有数据
USTRUCT(BlueprintType)
struct FGuizhouMahjongData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ChickenTileValue;

    UPROPERTY()
    bool bChickenCaptured;

    UPROPERTY()
    int32 XiaMaValue;

    UPROPERTY()
    int32 XiaMaCount;
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UGuizhouMahjongWidget : public UMahjongGameWidget
{
    GENERATED_BODY()

public:

    // 构造函数
    UGuizhouMahjongWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化贵阳麻将
    UFUNCTION(BlueprintCallable, Category="Guizhou|Game")
    void InitializeGuizhouMahjong();

    // 设置捉鸡牌值
    UFUNCTION(BlueprintCallable, Category="Guizhou|Game")
    void SetChickenTile(int32 TileValue);

    // 获取捉鸡牌值
    UFUNCTION(BlueprintCallable, Category="Guizhou|Game")
    int32 GetChickenTileValue() const;

    // 捉鸡按钮点击
    UFUNCTION(BlueprintCallable, Category="Guizhou|Game")
    void OnCatchChickenClicked();

    // 不捉鸡按钮点击
    UFUNCTION(BlueprintCallable, Category="Guizhou|Game")
    void OnNoCatchChickenClicked();

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 捉鸡提示面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* ChickenPromptPanel;

    // 捉鸡提示文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ChickenPromptText;

    // 捉鸡按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CatchChickenButton;

    // 不捉按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* NoCatchChickenButton;

    // 夏麻显示文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* XiaMaText;

    // 贵阳麻将特有数据
    UPROPERTY()
    FGuizhouMahjongData GuizhouData;
};
