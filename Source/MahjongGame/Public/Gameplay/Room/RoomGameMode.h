#pragma once
#include "GameFramework/GameModeBase.h"
#include "RoomGameMode.generated.h"
class URoomWidget;
UCLASS()
class MAHJONGGAME_API ARoomGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ARoomGameMode();
    virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void ShowRoomUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void HideRoomUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void UpdatePlayerCount(int32 Current, int32 Max);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void StartGame();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnPlayerJoined(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnPlayerLeft(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void LeaveRoom();
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Room")
    TSubclassOf<URoomWidget> RoomWidgetClass;
    UPROPERTY()
    URoomWidget* RoomWidgetInstance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Room")
    int32 RoomID;
};