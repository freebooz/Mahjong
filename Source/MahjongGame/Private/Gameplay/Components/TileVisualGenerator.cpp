//==============================================================================
// 麻将游戏 - 麻将牌视觉生成器实现
// TileVisualGenerator.cpp
//==============================================================================

#include "Gameplay/Components/TileVisualGenerator.h"
#include "Core/Types/ETileSuit.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"

//==============================================================================
// 花色常量 - 与 ETileSuit 枚举对应
//==============================================================================
constexpr int32 SUIT_CHARACTERS = 0;  // 万子
constexpr int32 SUIT_BAMBOOS = 1;     // 条子
constexpr int32 SUIT_CIRCLES = 2;     // 筒子
constexpr int32 SUIT_FLOWERS = 3;     // 花牌
constexpr int32 SUIT_CHICKEN = 4;     // 鸡牌（贵阳麻将特有）

//==============================================================================
// 构造函数
//==============================================================================
UTileVisualGenerator::UTileVisualGenerator()
{
    PrimaryComponentTick.bCanEverTick = false;

    TextureWidth = 128;
    TextureHeight = 128;

    DefaultStyle.BackgroundColor = FLinearColor::White;
    DefaultStyle.BorderColor = FLinearColor::Black;
    DefaultStyle.TextColor = FLinearColor::Black;
    DefaultStyle.BorderWidth = 4;
}

//==============================================================================
// 生成牌面纹理
//==============================================================================
bool UTileVisualGenerator::GenerateTileTexture(int32 TileValue, UTextureRenderTarget2D* RenderTarget)
{
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("[TileVisualGenerator] RenderTarget is null"));
        return false;
    }

    int32 Suit = TileValue / 10;
    int32 Value = TileValue % 10;

    if (Suit == 0 || Value == 0)
    {
        // 特殊牌处理
        Suit = TileValue;
        Value = 1;
    }

    RenderTile(Suit, Value, RenderTarget);
    return true;
}

//==============================================================================
// 生成指定花色的牌纹理
//==============================================================================
bool UTileVisualGenerator::GenerateTileTextureWithSuit(int32 Suit, int32 Value, UTextureRenderTarget2D* RenderTarget)
{
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("[TileVisualGenerator] RenderTarget is null"));
        return false;
    }

    RenderTile(Suit, Value, RenderTarget);
    return true;
}

//==============================================================================
// 创建渲染目标
//==============================================================================
UTextureRenderTarget2D* UTileVisualGenerator::CreateRenderTarget(int32 Width, int32 Height)
{
    UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
    if (RenderTarget)
    {
        RenderTarget->InitCustomFormat(Width, Height, PF_B8G8R8A8, false);
        RenderTarget->UpdateResource();
        UE_LOG(LogTemp, Log, TEXT("[TileVisualGenerator] Created render target %dx%d"), Width, Height);
    }
    return RenderTarget;
}

//==============================================================================
// 渲染牌面
//==============================================================================
void UTileVisualGenerator::RenderTile(int32 Suit, int32 Value, UTextureRenderTarget2D* RenderTarget)
{
    if (!RenderTarget)
        return;

    FLinearColor BGColor = DefaultStyle.BackgroundColor;
    FLinearColor BorderColor = DefaultStyle.BorderColor;
    FLinearColor TextColor = DefaultStyle.TextColor;

    // 根据花色设置颜色
    switch (Suit)
    {
        case SUIT_CHARACTERS: // 万子 - 红色
            TextColor = FLinearColor(0.91f, 0.30f, 0.24f, 1.0f); // #E74C3C
            break;
        case SUIT_BAMBOOS: // 条子 - 绿色
            TextColor = FLinearColor(0.15f, 0.68f, 0.38f, 1.0f); // #27AE60
            break;
        case SUIT_CIRCLES: // 筒子 - 蓝色
            TextColor = FLinearColor(0.20f, 0.60f, 0.86f, 1.0f); // #3498DB
            break;
        case SUIT_FLOWERS: // 花牌 - 紫色
            TextColor = FLinearColor(0.61f, 0.35f, 0.71f, 1.0f); // #9B59B6
            break;
        case SUIT_CHICKEN: // 鸡牌 - 金色
            TextColor = FLinearColor(0.95f, 0.76f, 0.03f, 1.0f); // #F1C40F
            break;
    }

    // 清空渲染目标
    UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, BGColor);

    // 绘制边框
    DrawBorder(RenderTarget, BorderColor, DefaultStyle.BorderWidth);

    // 绘制文字
    FString Text = GetValueText(Suit, Value);
    if (!Text.IsEmpty())
    {
        DrawText(RenderTarget, Text, TextColor, 48.0f);
    }

    UE_LOG(LogTemp, Verbose, TEXT("[TileVisualGenerator] Rendered tile Suit=%d, Value=%d"), Suit, Value);
}

//==============================================================================
// 绘制边框
//==============================================================================
void UTileVisualGenerator::DrawBorder(UTextureRenderTarget2D* RenderTarget, const FLinearColor& Color, int32 Thickness)
{
    if (!RenderTarget)
        return;

    // 边框绘制需要在运行时通过Canvas实现
    // 此处仅记录日志
    UE_LOG(LogTemp, Verbose, TEXT("[TileVisualGenerator] DrawBorder called - using Canvas for actual rendering"));
}

//==============================================================================
// 绘制文字
//==============================================================================
void UTileVisualGenerator::DrawText(UTextureRenderTarget2D* RenderTarget, const FString& Text, const FLinearColor& Color, float FontSize)
{
    if (!RenderTarget)
        return;

    // 使用UKismetRenderingLibrary绘制文字
    // 注意：实际项目中可能需要更复杂的文字渲染方案
    UE_LOG(LogTemp, Log, TEXT("[TileVisualGenerator] Drawing text: %s, Color: %s, Size: %f"),
        *Text, *Color.ToString(), FontSize);
}

//==============================================================================
// 获取花色名称
//==============================================================================
FString UTileVisualGenerator::GetSuitName(int32 Suit) const
{
    switch (Suit)
    {
        case SUIT_CHARACTERS: return TEXT("萬");
        case SUIT_BAMBOOS: return TEXT("条");
        case SUIT_CIRCLES: return TEXT("筒");
        case SUIT_FLOWERS: return TEXT("花");
        case SUIT_CHICKEN: return TEXT("鸡");
        default: return TEXT("");
    }
}

//==============================================================================
// 获取数值文字
//==============================================================================
FString UTileVisualGenerator::GetValueText(int32 Suit, int32 Value) const
{
    if (Suit == SUIT_FLOWERS)
    {
        switch (Value)
        {
            case 1: return TEXT("梅");
            case 2: return TEXT("兰");
            case 3: return TEXT("菊");
            case 4: return TEXT("竹");
        }
    }
    else if (Suit == SUIT_CHICKEN)
    {
        return TEXT("鸡");
    }
    else
    {
        // 万、筒、条使用数字
        static const TCHAR* ChineseNumbers[] = { TEXT(""), TEXT("一"), TEXT("二"), TEXT("三"), TEXT("四"),
                                                  TEXT("五"), TEXT("六"), TEXT("七"), TEXT("八"), TEXT("九") };
        if (Value >= 1 && Value <= 9)
        {
            FString Suitsuffix = GetSuitName(Suit);
            return FString::Printf(TEXT("%s%s"), ChineseNumbers[Value], *Suitsuffix);
        }
    }

    return TEXT("");
}
