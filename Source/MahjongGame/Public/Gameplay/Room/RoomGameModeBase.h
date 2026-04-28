#pragma once
#include "Gameplay/Modes/UIGameModeBase.h"
#include "Gameplay/Room/RoomGameRulesConfig.h"
#include "RoomGameModeBase.generated.h"
class URoomWidgetBase;
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API ARoomGameModeBase : public AUIGameModeBase
{
    GENERATED_BODY()
public:
    ARoomGameModeBase();
    virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void ShowRoomUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void HideRoomUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void UpdatePlayerCount(int32 Current, int32 Max);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void StartGame();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnPlayerReady(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void LeaveRoom();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnTileDrawn(int32 PlayerIndex);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnTileDiscarded(int32 PlayerIndex, int32 TileID);

    //============================================================================
    // 获取房间规则配置
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Mahjong|Room")
    FRoomGameRulesConfig GetRoomRulesConfig() const { return RoomRulesConfig; }

    //============================================================================
    // 设置房间规则配置
    //============================================================================
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void SetRoomRulesConfig(const FRoomGameRulesConfig& Config);

    //============================================================================
    // 检查是否启用黑八
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Mahjong|Room")
    bool IsHeiBaEnabled() const { return RoomRulesConfig.bEnableHeiBa; }

    //============================================================================
    // 检查是否启用冲锋鸡
    //============================================================================
    UFUNCTION(BlueprintPure, Category="Mahjong|Room")
    bool IsChongFengJiEnabled() const { return RoomRulesConfig.bEnableChongFengJi; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Room")
    TSubclassOf<URoomWidgetBase> RoomWidgetClass;
    UPROPERTY()
    URoomWidgetBase* RoomWidgetInstance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Room")
    int32 RoomID;
    UPROPERTY()
    int32 CurrentPlayerCount;
    UPROPERTY()
    int32 MaxPlayerCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Room")
    FRoomGameRulesConfig RoomRulesConfig;
};