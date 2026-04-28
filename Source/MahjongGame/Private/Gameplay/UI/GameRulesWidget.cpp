#include "Gameplay/UI/GameRulesWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"

//==============================================================================
// 游戏规则界面实现
//==============================================================================
UGameRulesWidget::UGameRulesWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CurrentTabIndex = 0;
}

//==============================================================================
// 界面构建完成
//==============================================================================
void UGameRulesWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseButton)
        CloseButton->OnClicked.AddDynamic(this, &UGameRulesWidget::OnCloseClicked);

    if (BasicRulesButton)
        BasicRulesButton->OnClicked.AddDynamic(this, &UGameRulesWidget::OnBasicRulesClicked);

    if (FanRulesButton)
        FanRulesButton->OnClicked.AddDynamic(this, &UGameRulesWidget::OnFanRulesClicked);

    if (SpecialRulesButton)
        SpecialRulesButton->OnClicked.AddDynamic(this, &UGameRulesWidget::OnSpecialRulesClicked);

    if (ScoreRulesButton)
        ScoreRulesButton->OnClicked.AddDynamic(this, &UGameRulesWidget::OnScoreRulesClicked);

    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[GameRules] 游戏规则界面已构建"));
}

//==============================================================================
// 显示规则界面
//==============================================================================
void UGameRulesWidget::ShowRules()
{
    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Visible);

    UE_LOG(LogTemp, Log, TEXT("[GameRules] 显示规则界面"));
}

//==============================================================================
// 隐藏规则界面
//==============================================================================
void UGameRulesWidget::HideRules()
{
    if (RootCanvas)
        RootCanvas->SetVisibility(ESlateVisibility::Collapsed);

    UE_LOG(LogTemp, Log, TEXT("[GameRules] 隐藏规则界面"));
}

//==============================================================================
// 添加规则章节
//==============================================================================
void UGameRulesWidget::AddRuleSection(const FString& Title, const FString& Content)
{
    FRuleSection Section;
    Section.SectionTitle = Title;
    Section.SectionContent = Content;
    Section.bIsExpanded = true;

    AllRuleSections.Add(Section);
}

//==============================================================================
// 清空所有规则
//==============================================================================
void UGameRulesWidget::ClearAllRules()
{
    AllRuleSections.Empty();
    CurrentTabIndex = 0;
}

//==============================================================================
// 关闭按钮点击
//==============================================================================
void UGameRulesWidget::OnCloseClicked()
{
    HideRules();
}

//==============================================================================
// 基础规则标签点击
//==============================================================================
void UGameRulesWidget::OnBasicRulesClicked()
{
    CurrentTabIndex = 0;
    UE_LOG(LogTemp, Log, TEXT("[GameRules] 切换到基础规则"));
}

//==============================================================================
// 番型规则标签点击
//==============================================================================
void UGameRulesWidget::OnFanRulesClicked()
{
    CurrentTabIndex = 1;
    UE_LOG(LogTemp, Log, TEXT("[GameRules] 切换到番型规则"));
}

//==============================================================================
// 特殊规则标签点击
//==============================================================================
void UGameRulesWidget::OnSpecialRulesClicked()
{
    CurrentTabIndex = 2;
    UE_LOG(LogTemp, Log, TEXT("[GameRules] 切换到特殊规则"));
}

//==============================================================================
// 得分规则标签点击
//==============================================================================
void UGameRulesWidget::OnScoreRulesClicked()
{
    CurrentTabIndex = 3;
    UE_LOG(LogTemp, Log, TEXT("[GameRules] 切换到得分规则"));
}
