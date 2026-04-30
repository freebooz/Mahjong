#include "Gameplay/UI/Mahjong/ActionPromptWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Core/UI/Common/UISoundManager.h"

UActionPromptWidget::UActionPromptWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UActionPromptWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ChowButton)
        ChowButton->OnClicked.AddDynamic(this, &UActionPromptWidget::OnChowClicked);

    if (PongButton)
        PongButton->OnClicked.AddDynamic(this, &UActionPromptWidget::OnPongClicked);

    if (KongButton)
        KongButton->OnClicked.AddDynamic(this, &UActionPromptWidget::OnKongClicked);

    if (HuButton)
        HuButton->OnClicked.AddDynamic(this, &UActionPromptWidget::OnHuClicked);

    if (DrawButton)
        DrawButton->OnClicked.AddDynamic(this, &UActionPromptWidget::OnDrawClicked);

    if (DiscardButton)
        DiscardButton->OnClicked.AddDynamic(this, &UActionPromptWidget::OnDiscardClicked);

    if (SkipButton)
        SkipButton->OnClicked.AddDynamic(this, &UActionPromptWidget::OnSkipClicked);

    HideActions();
}

void UActionPromptWidget::ShowActions(const FAvailableGameActions& Actions, float CountdownTime)
{
    CurrentActions = Actions;

    if (ChowButton)
        ChowButton->SetVisibility(Actions.bCanChow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (PongButton)
        PongButton->SetVisibility(Actions.bCanPong ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (KongButton)
        KongButton->SetVisibility(Actions.bCanKong ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (HuButton)
        HuButton->SetVisibility(Actions.bCanHu ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (DrawButton)
        DrawButton->SetVisibility(Actions.bCanDraw ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (DiscardButton)
        DiscardButton->SetVisibility(Actions.bCanDiscard ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    Show();
}

void UActionPromptWidget::HideActions()
{
    Hide();
}

void UActionPromptWidget::SetActionCallback(FOnActionSelected Callback)
{
    ActionCallback = Callback;
}

bool UActionPromptWidget::IsShowingActions() const
{
    return IsVisible();
}

void UActionPromptWidget::OnChowClicked()
{
    UUISoundManager::PlayMahjongTileSound(this, 2); // 吃牌音效
    OnActionCompleted(EActionType::Chow);
    ActionCallback.ExecuteIfBound(EActionType::Chow);
    HideActions();
}

void UActionPromptWidget::OnPongClicked()
{
    UUISoundManager::PlayMahjongTileSound(this, 3); // 碰牌音效
    OnActionCompleted(EActionType::Pong);
    ActionCallback.ExecuteIfBound(EActionType::Pong);
    HideActions();
}

void UActionPromptWidget::OnKongClicked()
{
    UUISoundManager::PlayMahjongTileSound(this, 4); // 杠牌音效
    OnActionCompleted(EActionType::Kong);
    ActionCallback.ExecuteIfBound(EActionType::Kong);
    HideActions();
}

void UActionPromptWidget::OnHuClicked()
{
    UUISoundManager::PlayMahjongTileSound(this, 5); // 胡牌音效
    OnActionCompleted(EActionType::Hu);
    ActionCallback.ExecuteIfBound(EActionType::Hu);
    HideActions();
}

void UActionPromptWidget::OnDrawClicked()
{
    UUISoundManager::PlayMahjongTileSound(this, 0); // 摸牌音效
    OnActionCompleted(EActionType::Draw);
    ActionCallback.ExecuteIfBound(EActionType::Draw);
    HideActions();
}

void UActionPromptWidget::OnDiscardClicked()
{
    UUISoundManager::PlayMahjongTileSound(this, 1); // 出牌音效
    OnActionCompleted(EActionType::Discard);
    ActionCallback.ExecuteIfBound(EActionType::Discard);
    HideActions();
}

void UActionPromptWidget::OnSkipClicked()
{
    UUISoundManager::PlayButtonClick(this);
    OnActionCompleted(EActionType::Skip);
    ActionCallback.ExecuteIfBound(EActionType::Skip);
    HideActions();
}
