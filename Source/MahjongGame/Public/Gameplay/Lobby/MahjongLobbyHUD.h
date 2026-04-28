#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Gameplay/Lobby/MahjongLobbyPlayerController.h"
#include "MahjongLobbyHUD.generated.h"

class UMahjongLobbyWidget;

/**
 * 大厅HUD类，负责管理大厅界面的显示和交互逻辑。
 * 在独立服务器模式下，此类区分了服务器端和客户端的职责，客户端负责UI显示，服务器负责同步数据。
 */
UCLASS()
class MAHJONGGAME_API AMahjongLobbyHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMahjongLobbyHUD();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void ShowLobbyMainUI();

    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void RefreshRoomList();

    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void ShowCreateRoomPopup();

    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void ClientJoinRoom(int32 RoomID);

    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void RequestCreateRoom(const FString& RoomName, int32 MaxPlayers);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Lobby HUD|UI")
    TSubclassOf<UMahjongLobbyWidget> LobbyWidgetClass;

    UPROPERTY(Transient)
    UMahjongLobbyWidget* LobbyWidgetInstance;

    //// 服务器RPC函数
    //UFUNCTION(Server, Reliable, WithValidation)
    //void ServerCreateRoom(const FString& RoomName, int32 MaxPlayers);

    //UFUNCTION(Server, Reliable, WithValidation)
    //void ServerJoinRoom(int32 RoomID);

    //UFUNCTION(NetMulticast, Reliable)
    //void MulticastUpdateRoomList(const TArray<FString>& RoomNames, const TArray<int32>& RoomIDs);

    //UFUNCTION(Client, Reliable)
    //void ClientOnRoomJoined(int32 RoomID);

private:
    void InitializeLobbyUI();
    bool IsClient() const { return GetNetMode() != NM_DedicatedServer; }
    bool IsServer() const { return GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer; }

//public:
//    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};