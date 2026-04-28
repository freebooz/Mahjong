#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

//==============================================================================
// 通用层 - 基础窗口组件
// 所有 UI 组件的基类，提供通用功能和接口
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UBaseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 显示窗口
    UFUNCTION(BlueprintCallable, Category="Common|Base")
    virtual void Show();

    // 隐藏窗口
    UFUNCTION(BlueprintCallable, Category="Common|Base")
    virtual void Hide();

    // 是否正在显示
    UFUNCTION(BlueprintCallable, Category="Common|Base")
    bool IsVisible() const;

    // 设置淡入淡出动画
    UFUNCTION(BlueprintCallable, Category="Common|Base")
    void PlayFadeIn(float Duration = 0.3f);

    // 设置淡出动画
    UFUNCTION(BlueprintCallable, Category="Common|Base")
    void PlayFadeOut(float Duration = 0.3f);

protected:
    // 淡入完成回调
    UFUNCTION(BlueprintImplementableEvent, Category="Common|Base")
    void OnFadeInFinished();

    // 淡出完成回调
    UFUNCTION(BlueprintImplementableEvent, Category="Common|Base")
    void OnFadeOutFinished();

    // 是否正在显示
    UPROPERTY()
    bool bIsShowing;
};
