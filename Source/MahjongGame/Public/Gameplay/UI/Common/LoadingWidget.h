#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "LoadingWidget.generated.h"

//==============================================================================
// 通用层 - 加载等待组件
// 用于显示加载状态，如网络请求、场景切换等
// 支持自定义加载文本和旋转动画
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API ULoadingWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    ULoadingWidget(const FObjectInitializer& ObjectInitializer);

    // 显示加载中
    UFUNCTION(BlueprintCallable, Category="Common|Loading")
    void ShowLoading(const FString& Message = TEXT("加载中..."));

    // 显示进度
    UFUNCTION(BlueprintCallable, Category="Common|Loading")
    void ShowProgress(float Progress, const FString& Message = TEXT(""));

    // 隐藏加载
    UFUNCTION(BlueprintCallable, Category="Common|Loading")
    void HideLoading();

    // 设置加载文本
    UFUNCTION(BlueprintCallable, Category="Common|Loading")
    void SetLoadingText(const FString& Message);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 每帧更新
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 加载文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* LoadingText;

    // 加载图标（旋转）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* LoadingIcon;

    // 进度条
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* ProgressContainer;

    // 背景遮罩
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* BackgroundOverlay;

    // 旋转角度
    UPROPERTY()
    float RotationAngle;

    // 是否显示进度
    UPROPERTY()
    bool bShowProgress;
};
