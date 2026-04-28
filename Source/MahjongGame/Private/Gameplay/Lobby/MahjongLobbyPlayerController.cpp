#include "Gameplay/Lobby/MahjongLobbyPlayerController.h"
#include "Gameplay/Lobby/MahjongLobbyGameState.h"
#include "Core/PlayerState/AMahjongPlayerStateBase.h"
AMahjongLobbyPlayerController::AMahjongLobbyPlayerController() {}
void AMahjongLobbyPlayerController::BeginPlay() { Super::BeginPlay(); UE_LOG(LogTemp, Log, TEXT("[CLIENT] LobbyPlayerController BeginPlay")); }
void AMahjongLobbyPlayerController::Server_SetPlayerReady_Implementation(bool bReady)
{
    if (AMahjongLobbyGameState* GS = Cast<AMahjongLobbyGameState>(GetWorld()->GetGameState()))
        GS->SetPlayerReady(this, bReady);
}
bool AMahjongLobbyPlayerController::Server_SetPlayerReady_Validate(bool bReady) { return true; }
void AMahjongLobbyPlayerController::Server_RequestStartGame_Implementation()
{
    if (AMahjongLobbyGameState* GS = Cast<AMahjongLobbyGameState>(GetWorld()->GetGameState()))
        GS->TryStartGame(this);
}
bool AMahjongLobbyPlayerController::Server_RequestStartGame_Validate() { return true; }
void AMahjongLobbyPlayerController::Client_ShowNotification_Implementation(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[CLIENT] Lobby Notification: %s"), *Message);
}
void AMahjongLobbyPlayerController::Client_CloseLobby_Implementation() { UE_LOG(LogTemp, Log, TEXT("[CLIENT] Closing Lobby")); }
void AMahjongLobbyPlayerController::ClientRequestRefreshRoomList_Implementation() { UE_LOG(LogTemp, Log, TEXT("[CLIENT] Requesting room list refresh")); }
void AMahjongLobbyPlayerController::ClientRequestJoinRoom_Implementation(const FString& RoomID) { UE_LOG(LogTemp, Log, TEXT("[CLIENT] Requesting to join room: %s"), *RoomID); }
