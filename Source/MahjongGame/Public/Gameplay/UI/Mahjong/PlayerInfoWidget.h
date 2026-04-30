#pragma once
#include "CoreMinimal.h"
#include "Core/UI/Common/BaseWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "PlayerInfoWidget.generated.h"

//==============================================================================
// 麻将专用层 - 玩家信息组件
// 显示玩家头像、昵称、座位、分数、金币等信息
// 支持四个方向的玩家位置显示
//==============================================================================
UENUM(BlueprintType)
enum class EPlayerPosition : uint8
{
    Self,    // 自己（下）
    Right,   // 右边（左）
    Top,     // 对面（上）
    Left     // 左边（右）
};

USTRUCT(BlueprintType)
struct FPlayerInfoData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 PlayerID;

    UPROPERTY()
    FString Nickname;

    UPROPERTY()
    int32 PlayerLevel;

    UPROPERTY()
    FString AvatarURL;

    UPROPERTY()
    int32 Score;

    UPROPERTY()
    int32 TotalScore;

    UPROPERTY()
    bool bIsReady;

    UPROPERTY()
    bool bIsOnline;

    UPROPERTY()
    bool bIsHost;
};

UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UPlayerInfoWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer);

    // 设置玩家信息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void SetPlayerInfo(const FPlayerInfoData& Info);

    // 获取玩家信息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    FPlayerInfoData GetPlayerInfo() const;

    // 设置玩家位置
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void SetPlayerPosition(EPlayerPosition Position);

    // 设置回合指示器
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void SetTurnIndicator(bool bIsMyTurn);

    // 设置准备状态
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void SetReadyStatus(bool bIsReady);

    // 设置在线状态
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void SetOnlineStatus(bool bIsOnline);

    // 更新分数
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void UpdateScore(int32 NewScore);

    // 增加分数
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void AddScore(int32 Delta);

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 玩家信息面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* PlayerPanel;

    // 头像
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* AvatarImage;

    // 昵称
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* NicknameText;

    // 等级
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* LevelText;

    // 分数
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ScoreText;

    // 回合指示器
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* TurnIndicator;

    // 准备标记
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ReadyText;

    // 离线标记
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* OfflineOverlay;

    // 庄家标记
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* BankerIcon;

    // 当前玩家信息
    UPROPERTY()
    FPlayerInfoData PlayerData;

    // 当前位置
    UPROPERTY()
    EPlayerPosition CurrentPosition;
};
