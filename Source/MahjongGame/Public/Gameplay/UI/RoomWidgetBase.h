#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "RoomWidgetBase.generated.h"

//==============================================================================
// 房间界面基类
// 提供房间内游戏界面的基本功能
// 包含房间信息显示、玩家列表、动作按钮等
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API URoomWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    URoomWidgetBase(const FObjectInitializer& ObjectInitializer);

    // 界面构建完成回调
    virtual void NativeConstruct() override;

    // 离开房间按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnLeaveClicked();

    // 开始游戏按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnStartGameClicked();

    // 摸牌按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnDrawTileClicked();

    // 出牌按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnDiscardTileClicked();

    // 吃牌按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnChowClicked();

    // 碰牌按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnPongClicked();

    // 杠牌按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnKongClicked();

    // 胡牌按钮点击
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnHuClicked();

    // 更新房间信息显示
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void UpdateRoomInfo(int32 RoomID, int32 CurrentPlayers, int32 MaxPlayers);

    // 更新回合信息显示
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void UpdateTurnInfo(int32 CurrentPlayer, bool bIsMyTurn);

    // 显示/隐藏动作按钮
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void ShowActionButtons(bool bShowDraw, bool bShowDiscard, bool bShowChow, bool bShowPong, bool bShowKong, bool bShowHu);

    // 设置游戏状态文本
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void SetGameStatus(const FString& Status);

protected:
    // 按钮控件（通过 Blueprint 绑定）
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* LeaveButton;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* StartGameButton;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* DrawTileButton;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* DiscardButton;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ChowButton;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* PongButton;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* KongButton;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* HuButton;

    // 文本控件
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* RoomIDText;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PlayerCountText;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusText;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TurnInfoText;

    // 图片控件
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* TableImage;

    // 容器控件
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHorizontalBox* PlayerHandBox;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* PlayerPositionBox;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;
};