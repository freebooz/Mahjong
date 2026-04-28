//==============================================================================
// MahjongGame - UI GameMode Base Class
// UIGameModeBase.h
//
// 提供UI GameMode的公共功能
// 消除LoginGameMode和RoomGameMode之间的代码重复
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UIGameModeBase.generated.h"

class UUserWidget;

//==============================================================================
// UI GameMode基类
// 提供通用的UI显示/隐藏功能
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API AUIGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    //============================================================================
    // 显示UI组件
    // 参数：
    //   - WidgetClass - Widget类
    //   - WidgetInstance - 输出参数，Widget实例
    //   - LogPrefix - 日志前缀
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|UI")
    void ShowUIWidget(TSubclassOf<UUserWidget> WidgetClass, UUserWidget*& WidgetInstance, const FString& LogPrefix);

    //============================================================================
    // 隐藏UI组件
    // 参数：
    //   - WidgetInstance - 要隐藏的Widget实例
    //   - LogPrefix - 日志前缀
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|UI")
    void HideUIWidget(UUserWidget* WidgetInstance, const FString& LogPrefix);

protected:
    //============================================================================
    // 切换到指定关卡
    // 参数：
    //   - LevelName - 关卡名称
    //   - bIsListen - 是否为listen服务器
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|UI")
    void TransitionToLevel(const FString& LevelName, bool bIsListen = true);
};
