#pragma once
#include "CoreMinimal.h"
#include "Core/UI/Common/BaseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "GameRulesWidget.generated.h"

//==============================================================================
// 麻将专用层 - 游戏规则界面
// 显示麻将游戏规则，支持多标签页分类
//==============================================================================
USTRUCT(BlueprintType)
struct FRuleSection
{
    GENERATED_BODY()

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString Content;
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UGameRulesWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UGameRulesWidget(const FObjectInitializer& ObjectInitializer);

    // 显示规则
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void ShowRules();

    // 隐藏规则
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void HideRules();

    // 添加规则章节
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void AddRuleSection(const FString& Title, const FString& Content);

    // 清空规则
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void ClearRules();

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 关闭按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnCloseClicked();

    // 基础规则标签
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnBasicRulesClicked();

    // 番型规则标签
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnFanRulesClicked();

    // 特殊规则标签
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnSpecialRulesClicked();

    // 得分规则标签
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnScoreRulesClicked();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 标题
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TitleText;

    // 关闭按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CloseButton;

    // 基础规则按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* BasicRulesButton;

    // 番型规则按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* FanRulesButton;

    // 特殊规则按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* SpecialRulesButton;

    // 得分规则按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ScoreRulesButton;

    // 规则滚动区
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UScrollBox* RulesScrollBox;

    // 规则内容容器
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* RulesContentBox;

    // 当前标签索引
    UPROPERTY()
    int32 CurrentTabIndex;

    // 所有规则
    UPROPERTY()
    TArray<FRuleSection> AllRules;
};
