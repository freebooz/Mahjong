#pragma once
#include "CoreMinimal.h"
#include "Gameplay/Lobby/MahjongLobbyGameState.h"
#include "MahjongLobbyPlayerController.generated.h"
UCLASS()
class MAHJONGGAME_API AMahjongLobbyPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    AMahjongLobbyPlayerController();
    UFUNCTION(Server, Reliable) void Server_SetPlayerReady(bool bReady);
    UFUNCTION(Server, Reliable) void Server_RequestStartGame();
    UFUNCTION(Client, Reliable) void Client_ShowNotification(const FString& Message);
    UFUNCTION(Client, Reliable) void Client_CloseLobby();
    UFUNCTION(Client, Reliable) void ClientRequestRefreshRoomList();
    UFUNCTION(Client, Reliable) void ClientRequestJoinRoom(const FString& RoomID);
    UFUNCTION(Server, Reliable) void ClientRequestCreateRoom(const FString& RoomName, int32 MaxPlayers);
protected:
    virtual void BeginPlay() override;
private:
    bool Server_SetPlayerReady_Validate(bool bReady);
    void Server_SetPlayerReady_Implementation(bool bReady);
    bool Server_RequestStartGame_Validate();
    void Server_RequestStartGame_Implementation();
};
