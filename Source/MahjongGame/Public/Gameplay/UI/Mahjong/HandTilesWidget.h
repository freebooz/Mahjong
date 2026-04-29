#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Gameplay/UI/Mahjong/MahjongTileWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "HandTilesWidget.generated.h"

//==============================================================================
// 麻将专用层 - 手牌显示组件
// 显示玩家手牌，支持摸牌、出牌、排序等操作
//==============================================================================
USTRUCT(BlueprintType)
struct FHandTilesData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FMahjongTile> Tiles;

    UPROPERTY()
    int32 TileCount;

    UPROPERTY()
    bool bIsSorted;
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UHandTilesWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UHandTilesWidget(const FObjectInitializer& ObjectInitializer);

    // 设置手牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    void SetHandTiles(const TArray<FMahjongTile>& Tiles);

    // 添加一张牌（摸牌）
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    void AddTile(const FMahjongTile& Tile);

    // 移除一张牌（出牌）
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    void RemoveTile(int32 TileID);

    // 选中的牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    void SelectTile(int32 Index);

    // 取消选中
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    void DeselectAll();

    // 获取选中的牌索引
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    int32 GetSelectedIndex() const;

    // 获取选中的牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    FMahjongTile GetSelectedTile() const;

    // 排序手牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    void SortTiles();

    // 获取手牌数量
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    int32 GetTileCount() const;

    // 获取所有手牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    TArray<FMahjongTile> GetAllTiles() const;

    // 出牌动画完成回调
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Hand")
    void OnDiscardAnimationComplete();

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 牌点击处理
    UFUNCTION(BlueprintCallable, Category="Mahjong|Hand")
    void OnTileClicked(int32 Index);

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 手牌容器
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHorizontalBox* HandTilesContainer;

    // 手牌数量文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TileCountText;

    // 排序按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* SortButton;

    // 当前手牌数据
    UPROPERTY()
    TArray<FMahjongTile> CurrentTiles;

    // 选中的牌索引
    UPROPERTY()
    int32 SelectedIndex;

    // 是否已排序
    UPROPERTY()
    bool bIsSorted;
};
