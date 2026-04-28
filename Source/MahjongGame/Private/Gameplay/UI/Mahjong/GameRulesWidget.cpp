#include "Gameplay/UI/Mahjong/GameRulesWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"

UGameRulesWidget::UGameRulesWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CurrentTabIndex = 0;
}

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

    HideRules();
}

void UGameRulesWidget::ShowRules()
{
    Show();
}

void UGameRulesWidget::HideRules()
{
    Hide();
}

void UGameRulesWidget::AddRuleSection(const FString& Title, const FString& Content)
{
    FRuleSection Section;
    Section.Title = Title;
    Section.Content = Content;
    AllRules.Add(Section);
}

void UGameRulesWidget::ClearRules()
{
    AllRules.Empty();
    CurrentTabIndex = 0;
}

void UGameRulesWidget::OnCloseClicked()
{
    HideRules();
}

void UGameRulesWidget::OnBasicRulesClicked()
{
    CurrentTabIndex = 0;
}

void UGameRulesWidget::OnFanRulesClicked()
{
    CurrentTabIndex = 1;
}

void UGameRulesWidget::OnSpecialRulesClicked()
{
    CurrentTabIndex = 2;
}

void UGameRulesWidget::OnScoreRulesClicked()
{
    CurrentTabIndex = 3;
}
