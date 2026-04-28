#pragma once
#include "GameFramework/GameModeBase.h"
#include "LobbyGameModeBase.generated.h"
class ULobbyWidgetBase;
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API ALobbyGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    ALobbyGameModeBase();
    virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void ShowLobbyUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void HideLobbyUI();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void OnPlayerJoined(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void OnPlayerLeft(const FString& PlayerName);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void CreateNewRoom();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void JoinRoom(int32 RoomID);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void LeaveLobby();
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mahjong|Lobby")
    TSubclassOf<ULobbyWidgetBase> LobbyWidgetClass;
    UPROPERTY()
    ULobbyWidgetBase* LobbyWidgetInstance;
    UPROPERTY()
    int32 ConnectedPlayerCount;
};