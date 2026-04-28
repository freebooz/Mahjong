#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/CheckBox.h"
#include "GameRulesWidget.generated.h"

//==============================================================================
// 游戏规则界面
// 显示麻将游戏规则、番型说明、得分计算等
// 支持多标签页分类显示
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UGameRulesWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 规则章节结构
    USTRUCT(BlueprintType)
    struct FRuleSection
    {
        GENERATED_BODY()

        UPROPERTY()
        FString SectionTitle;

        UPROPERTY()
        FString SectionContent;

        UPROPERTY()
        bool bIsExpanded;
    };

    // 构造函数
    UGameRulesWidget(const FObjectInitializer& ObjectInitializer);

    // 界面构建完成
    virtual void NativeConstruct() override;

    // 显示规则界面
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void ShowRules();

    // 隐藏规则界面
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void HideRules();

    // 添加规则章节
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void AddRuleSection(const FString& Title, const FString& Content);

    // 清空所有规则
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void ClearAllRules();

    // 关闭按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnCloseClicked();

    // 基础规则标签点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnBasicRulesClicked();

    // 番型规则标签点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnFanRulesClicked();

    // 特殊规则标签点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnSpecialRulesClicked();

    // 得分计算标签点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Rules")
    void OnScoreRulesClicked();

protected:
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

    // 规则滚动区域
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UScrollBox* RulesScrollBox;

    // 规则内容容器
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* RulesContentBox;

    // 背景遮罩
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* BackgroundOverlay;

    // 当前选中的标签页
    UPROPERTY()
    int32 CurrentTabIndex;

    // 所有规则章节
    UPROPERTY()
    TArray<FRuleSection> AllRuleSections;
};
