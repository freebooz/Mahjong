#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "MahjongTileWidget.generated.h"

//==============================================================================
// 麻将专用层 - 单个麻将牌组件
// 显示麻将牌，支持选中、禁用、高亮等状态
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UMahjongTileWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 牌类型
    UENUM(BlueprintType)
    enum class ETileType : uint8
    {
        Wan,      // 万
        Tiao,     // 条
        Tong,     // 筒
        Feng,     // 风
        Jian,     // 箭
        Hua       // 花
    };

    // 牌数据结构
    USTRUCT(BlueprintType)
    struct FMahjongTile
    {
        GENERATED_BODY()

        UPROPERTY()
        int32 TileID;

        UPROPERTY()
        int32 TileValue;

        UPROPERTY()
        ETileType TileType;

        UPROPERTY()
        FString TileTexturePath;
    };

    // 构造函数
    UMahjongTileWidget(const FObjectInitializer& ObjectInitializer);

    // 设置牌数据
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetTileData(const FMahjongTile& Tile);

    // 获取牌数据
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    FMahjongTile GetTileData() const;

    // 设置选中状态
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetSelected(bool bSelected);

    // 设置高亮状态
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetHighlighted(bool bHighlighted);

    // 设置可用状态
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void SetEnabled(bool bEnabled);

    // 是否被选中
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    bool IsSelected() const;

    // 牌点击事件
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Tile")
    void OnTileClicked(const FMahjongTile& Tile);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 鼠标悬停事件
    virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 鼠标离开事件
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    // 点击处理
    UFUNCTION(BlueprintCallable, Category="Mahjong|Tile")
    void HandleClick();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* TileButton;

    // 牌面图片
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* TileImage;

    // 选中高亮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* SelectedOverlay;

    // 禁用遮罩
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* DisabledOverlay;

    // 当前牌数据
    UPROPERTY()
    FMahjongTile TileData;

    // 是否选中
    UPROPERTY()
    bool bIsSelected;

    // 是否高亮
    UPROPERTY()
    bool bIsHighlighted;
};
