//==============================================================================
// 麻将游戏 - 麻将牌视觉生成器
// TileVisualGenerator.h
//
// 本组件用于程序化生成麻将牌视觉
// 支持动态创建牌面纹理，无需图片资源
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TileVisualGenerator.generated.h"

class UMaterialInterface;
class UFont;

//==============================================================================
// 牌面配置
//==============================================================================
USTRUCT(BlueprintType)
struct FTileVisualStyle
{
    GENERATED_BODY()

    // 背景色
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor BackgroundColor = FLinearColor::White;

    // 边框颜色
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor BorderColor = FLinearColor::Black;

    // 文字颜色
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor TextColor = FLinearColor::Black;

    // 边框宽度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BorderWidth = 4;
};

//==============================================================================
// 麻将牌视觉生成器组件
// 负责程序化生成麻将牌的视觉表现
//==============================================================================
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAHJONGGAME_API UTileVisualGenerator : public UActorComponent
{
    GENERATED_BODY()

public:
    UTileVisualGenerator();

    //============================================================================
    // 生成牌面纹理
    // 参数：
    //   - TileValue: 牌值 (1-9, 11-19, 21-29, 31-39, 41-44, 51-53)
    //   - RenderTarget: 渲染目标纹理
    // 返回：是否成功
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Visual")
    bool GenerateTileTexture(int32 TileValue, UTextureRenderTarget2D* RenderTarget);

    //============================================================================
    // 生成指定花色的牌纹理
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Visual")
    bool GenerateTileTextureWithSuit(int32 Suit, int32 Value, UTextureRenderTarget2D* RenderTarget);

    //============================================================================
    // 创建渲染目标
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Visual")
    UTextureRenderTarget2D* CreateRenderTarget(int32 Width = 128, int32 Height = 128);

    //============================================================================
    // 获取牌面纹理尺寸
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Mahjong|Visual")
    FIntPoint GetTextureSize() const { return FIntPoint(TextureWidth, TextureHeight); }

protected:
    //============================================================================
    // 渲染牌面
    //============================================================================
    void RenderTile(int32 Suit, int32 Value, UTextureRenderTarget2D* RenderTarget);

    //============================================================================
    // 绘制边框
    //============================================================================
    void DrawBorder(UTextureRenderTarget2D* RenderTarget, const FLinearColor& Color, int32 Thickness);

    //============================================================================
    // 绘制文字
    //============================================================================
    void DrawText(UTextureRenderTarget2D* RenderTarget, const FString& Text, const FLinearColor& Color, float FontSize);

    //============================================================================
    // 获取花色名称
    //============================================================================
    FString GetSuitName(int32 Suit) const;

    //============================================================================
    // 获取数值文字
    //============================================================================
    FString GetValueText(int32 Suit, int32 Value) const;

public:
    //============================================================================
    // 纹理尺寸
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Visual|Settings")
    int32 TextureWidth = 128;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Visual|Settings")
    int32 TextureHeight = 128;

    //============================================================================
    // 默认样式
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Visual|Style")
    FTileVisualStyle DefaultStyle;

    //============================================================================
    // 字体
    //============================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Visual|Style")
    TSubclassOf<UFont> ChineseFont;
};
