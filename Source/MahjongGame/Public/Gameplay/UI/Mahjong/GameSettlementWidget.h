#pragma once
#include "CoreMinimal.h"
#include "Gameplay/UI/Common/BaseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "GameSettlementWidget.generated.h"

//==============================================================================
// 麻将专用层 - 游戏结算界面
// 显示游戏结算信息，包括排名、得分、胡牌类型等
//==============================================================================
USTRUCT(BlueprintType)
struct FPlayerSettlement
{
    GENERATED_BODY()

    UPROPERTY()
    FString PlayerName;

    UPROPERTY()
    int32 PlayerRank;

    UPROPERTY()
    int32 TotalScore;

    UPROPERTY()
    int32 HuType;

    UPROPERTY()
    int32 FanCount;

    UPROPERTY()
    bool bIsWinner;

    UPROPERTY()
    bool bIsSelfHu;
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UGameSettlementWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UGameSettlementWidget(const FObjectInitializer& ObjectInitializer);

    // 显示结算
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void ShowSettlement(const TArray<FPlayerSettlement>& Players);

    // 隐藏结算
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void HideSettlement();

    // 获取胡牌类型名称
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    FString GetHuTypeName(int32 HuType);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 分享按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void OnShareClicked();

    // 再来一局按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void OnPlayAgainClicked();

    // 返回大厅按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void OnReturnToLobbyClicked();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 标题
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TitleText;

    // 赢家名称
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* WinnerNameText;

    // 胡牌类型
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* HuTypeText;

    // 番数
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* FanCountText;

    // 玩家列表
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* PlayerListBox;

    // 分享按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ShareButton;

    // 再来一局按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* PlayAgainButton;

    // 返回大厅按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ReturnToLobbyButton;
};
