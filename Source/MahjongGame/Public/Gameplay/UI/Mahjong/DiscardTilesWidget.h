#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Gameplay/UI/Mahjong/MahjongTileWidget.h"
#include "Components/WrapBox.h"
#include "Components/CanvasPanel.h"
#include "DiscardTilesWidget.generated.h"

//==============================================================================
// 麻将专用层 - 弃牌区显示组件
// 显示所有已出的牌，支持最后一张高亮
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UDiscardTilesWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UDiscardTilesWidget(const FObjectInitializer& ObjectInitializer);

    // 添加弃牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    void AddDiscardTile(const FMahjongTile& Tile, int32 PlayerID);

    // 获取最近一张弃牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    FMahjongTile GetLastDiscardTile() const;

    // 获取最后弃牌玩家ID
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    int32 GetLastDiscardPlayerID() const;

    // 获取弃牌数量
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    int32 GetDiscardCount() const;

    // 获取某个玩家的弃牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    TArray<FMahjongTile> GetPlayerDiscards(int32 PlayerID) const;

    // 清除所有弃牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    void ClearAllDiscards();

    // 获取最近N张弃牌
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    TArray<FMahjongTile> GetRecentDiscards(int32 Count) const;

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 弃牌点击处理
    UFUNCTION(BlueprintCallable, Category="Mahjong|Discard")
    void OnDiscardTileClicked(int32 Index);

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 弃牌容器
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UWrapBox* DiscardTilesContainer;

    // 弃牌数量文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* DiscardCountText;

    // 当前弃牌列表
    UPROPERTY()
    TArray<FMahjongTile> DiscardTiles;

    // 对应玩家ID列表
    UPROPERTY()
    TArray<int32> DiscardPlayerIDs;

    // 最后一张索引
    UPROPERTY()
    int32 LastDiscardIndex;
};
