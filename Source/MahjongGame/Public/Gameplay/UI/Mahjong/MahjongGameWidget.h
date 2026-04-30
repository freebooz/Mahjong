#pragma once
#include "CoreMinimal.h"
#include "Core/UI/Common/BaseWidget.h"
#include "Gameplay/UI/Mahjong/HandTilesWidget.h"
#include "Gameplay/UI/Mahjong/DiscardTilesWidget.h"
#include "Gameplay/UI/Mahjong/PlayerInfoWidget.h"
#include "Gameplay/UI/Mahjong/ActionPromptWidget.h"
#include "Gameplay/UI/Mahjong/CountdownTimerWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "MahjongGameWidget.generated.h"

//==============================================================================
// 麻将专用层 - 麻将游戏主界面
// 整合所有麻将游戏组件，提供完整的游戏界面
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UMahjongGameWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UMahjongGameWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化游戏界面
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void InitializeGame();

    // 更新手牌显示
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void UpdateHandTiles(const TArray<FMahjongTile>& Tiles);

    // 更新弃牌区
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void UpdateDiscardTiles();

    // 更新玩家信息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void UpdatePlayerInfo(int32 PlayerID, const FPlayerInfoData& Info);

    // 显示动作提示
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void ShowActionPrompt(const FAvailableGameActions& Actions, float Countdown);

    // 隐藏动作提示
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void HideActionPrompt();

    // 显示回合倒计时
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void ShowTurnCountdown(float Duration);

    // 设置当前回合玩家
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void SetCurrentTurnPlayer(int32 PlayerID);

    // 获取当前回合玩家
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    int32 GetCurrentTurnPlayer() const;

    // 是否是自己的回合
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    bool IsMyTurn() const;

    // 获取自己位置
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    int32 GetSelfPlayerID() const;

    // 设置牌桌背景纹理
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void SetTableBackground(UTexture2D* NewTexture);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 离开按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void OnLeaveClicked();

    // 菜单按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void OnMenuClicked();

    // 规则按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game")
    void OnRulesClicked();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 牌桌背景纹理
    UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
    UImage* TableBackgroundImage;

    // 玩家1信息（自己，下）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UPlayerInfoWidget* Player1Info;

    // 玩家2信息（右边）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UPlayerInfoWidget* Player2Info;

    // 玩家3信息（对面）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UPlayerInfoWidget* Player3Info;

    // 玩家4信息（左边）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UPlayerInfoWidget* Player4Info;

    // 自己的手牌
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHandTilesWidget* SelfHandTiles;

    // 弃牌区
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UDiscardTilesWidget* DiscardTiles;

    // 动作提示
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UActionPromptWidget* ActionPrompt;

    // 倒计时
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCountdownTimerWidget* TurnCountdown;

    // 房间ID文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* RoomIDText;

    // 局数文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* RoundText;

    // 离开按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* LeaveButton;

    // 菜单按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* MenuButton;

    // 规则按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* RulesButton;

    // 当前回合玩家ID
    UPROPERTY()
    int32 CurrentTurnPlayerID;

    // 自己玩家ID
    UPROPERTY()
    int32 SelfPlayerID;
};
