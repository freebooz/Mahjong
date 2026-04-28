#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "GameSettlementWidget.generated.h"

//==============================================================================
// 游戏结算界面
// 显示游戏结束后的结算信息
// 包含：排名、得分、胡牌类型、番数计算等
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UGameSettlementWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 玩家结算数据
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

    // 构造函数
    UGameSettlementWidget(const FObjectInitializer& ObjectInitializer);

    // 界面构建完成
    virtual void NativeConstruct() override;

    // 显示结算界面
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void ShowSettlement(const TArray<FPlayerSettlement>& Players);

    // 隐藏结算界面
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void HideSettlement();

    // 设置赢家名字
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void SetWinnerName(const FString& WinnerName);

    // 获取本局胡牌类型名称
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    FString GetHuTypeName(int32 HuType);

    // 分享按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void OnShareClicked();

    // 再来一局按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void OnPlayAgainClicked();

    // 返回大厅按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settlement")
    void OnReturnToLobbyClicked();

protected:
    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 标题文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TitleText;

    // 赢家名称
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* WinnerNameText;

    // 胡牌类型
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* HuTypeText;

    // 番数文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* FanCountText;

    // 玩家结算列表容器
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

    // 背景图片
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* BackgroundImage;

    // 当前结算数据
    UPROPERTY()
    TArray<FPlayerSettlement> CurrentSettlement;
};
