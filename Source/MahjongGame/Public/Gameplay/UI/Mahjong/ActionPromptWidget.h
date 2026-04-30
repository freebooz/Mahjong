#pragma once
#include "CoreMinimal.h"
#include "Core/UI/Common/BaseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "ActionPromptWidget.generated.h"

//==============================================================================
// 麻将专用层 - 动作提示组件
// 显示吃、碰、杠、胡、摸牌、出牌等动作按钮
// 支持倒计时显示
//==============================================================================
USTRUCT(BlueprintType)
struct FAvailableGameActions
{
    GENERATED_BODY()

    UPROPERTY()
    bool bCanChow;

    UPROPERTY()
    bool bCanPong;

    UPROPERTY()
    bool bCanKong;

    UPROPERTY()
    bool bCanHu;

    UPROPERTY()
    bool bCanDraw;

    UPROPERTY()
    bool bCanDiscard;

    UPROPERTY()
    bool bCanSkip;
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
    Chow,
    Pong,
    Kong,
    Hu,
    Draw,
    Discard,
    Skip
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UActionPromptWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 动作回调
    DECLARE_DELEGATE_OneParam(FOnActionSelected, EActionType)

    // 构造函数
    UActionPromptWidget(const FObjectInitializer& ObjectInitializer);

    // 显示可用动作
    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void ShowActions(const FAvailableGameActions& Actions, float CountdownTime = 0.0f);

    // 隐藏动作提示
    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void HideActions();

    // 设置动作回调
    void SetActionCallback(FOnActionSelected Callback);

    // 是否正在显示
    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    bool IsShowingActions() const;

    // 动作完成回调
    UFUNCTION(BlueprintImplementableEvent, Category="Mahjong|Action")
    void OnActionCompleted(EActionType Action);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 动作按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void OnChowClicked();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void OnPongClicked();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void OnKongClicked();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void OnHuClicked();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void OnDrawClicked();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void OnDiscardClicked();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Action")
    void OnSkipClicked();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 动作按钮容器
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHorizontalBox* ActionButtonsContainer;

    // 标题文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TitleText;

    // 吃按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ChowButton;

    // 碰按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* PongButton;

    // 杠按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* KongButton;

    // 胡按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* HuButton;

    // 摸牌按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* DrawButton;

    // 出牌按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* DiscardButton;

    // 过按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* SkipButton;

    // 倒计时文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* CountdownText;

    // 当前可用动作
    UPROPERTY()
    FAvailableGameActions CurrentActions;

    // 动作回调
    FOnActionSelected ActionCallback;
};
