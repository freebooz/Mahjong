#include "Gameplay/Lobby/MahjongLobbyGameState.h"
#include "Gameplay/Lobby/MahjongLobbyPlayerController.h"
#include "Core/PlayerState/AMahjongPlayerStateBase.h"
#include "Gameplay/Modes/AMahjongGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Monitor/RedisMonitorBackend.h"

AMahjongLobbyGameState::AMahjongLobbyGameState()
{
    ReadyPlayerCount = 0;
    TotalPlayerCount = 0;
}

void AMahjongLobbyGameState::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[SERVER] LobbyGameState BeginPlay"));

    // Initialize monitor if needed (disabled by default)
    UMonitorManager::GetInstance(this)->Initialize(false);
}

void AMahjongLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMahjongLobbyGameState, ReadyPlayerCount);
    DOREPLIFETIME(AMahjongLobbyGameState, TotalPlayerCount);
}

bool AMahjongLobbyGameState::SetPlayerReady_Validate(AController* Controller, bool bReady)
{
    return true;
}

void AMahjongLobbyGameState::SetPlayerReady_Implementation(AController* Controller, bool bReady)
{
    if (!HasAuthority())
    {
        return;
    }

    if (AMahjongPlayerStateBase* PS = Cast<AMahjongPlayerStateBase>(Controller->PlayerState))
    {
        PS->bIsReady = bReady;

        // Update ready count
        ReadyPlayerCount = 0;
        for (APlayerState* P : PlayerArray)
        {
            if (Cast<AMahjongPlayerStateBase>(P)->bIsReady)
            {
                ReadyPlayerCount++;
            }
        }

        UE_LOG(LogTemp, Log, TEXT("[SERVER] Lobby: %s ready. %d/%d ready"),
            *PS->GetPlayerName(), ReadyPlayerCount, TotalPlayerCount);

        // Update monitor with current player list
        UpdateMonitorPlayerList();
    }
}

bool AMahjongLobbyGameState::TryStartGame_Validate(AController* Controller)
{
    return true;
}

void AMahjongLobbyGameState::TryStartGame_Implementation(AController* Controller)
{
    if (!HasAuthority())
    {
        return;
    }

    if (ReadyPlayerCount < 4)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[SERVER] Lobby: All ready - Transitioning to game"));

    // Notify monitor that game is starting
    UMonitorManager::GetInstance(this)->OnGameStarted(TEXT("MainHall"), TArray<FString>());

    UGameplayStatics::OpenLevel(this, TEXT("MahjongTable"), true, TEXT("listen"));
}

void AMahjongLobbyGameState::Server_RefreshRoomList_Implementation()
{
    UE_LOG(LogTemp, Log, TEXT("[SERVER] Refreshing room list"));
}

void AMahjongLobbyGameState::Server_JoinRoom_Implementation(int32 RoomID)
{
    UE_LOG(LogTemp, Log, TEXT("[SERVER] Joining room %d"), RoomID);
}

void AMahjongLobbyGameState::Client_ReceiveRoomList_Implementation(
    const TArray<FString>& RoomNames, const TArray<int32>& RoomIDs)
{
    UE_LOG(LogTemp, Log, TEXT("[CLIENT] Received %d rooms"), RoomNames.Num());
}

void AMahjongLobbyGameState::Server_PlayerLogin_Implementation(AController* Controller)
{
    if (!HasAuthority())
    {
        return;
    }

    NotifyPlayerLogin(Controller);
}

void AMahjongLobbyGameState::Server_PlayerLogout_Implementation(AController* Controller)
{
    if (!HasAuthority())
    {
        return;
    }

    NotifyPlayerLogout(Controller);
}

void AMahjongLobbyGameState::NotifyPlayerLogin(AController* Controller)
{
    if (!Controller || !Controller->PlayerState)
    {
        return;
    }

    FString PlayerID = Controller->PlayerState->GetPlayerName();
    FString Nickname = Controller->PlayerState->GetPlayerName();
    bool bIsGuest = false;

    // Report to monitor
    UMonitorManager::GetInstance(this)->OnPlayerConnected(PlayerID, Nickname, bIsGuest);

    // Update total player count
    TotalPlayerCount = PlayerArray.Num();

    UE_LOG(LogTemp, Log, TEXT("[SERVER] Player login event: %s"), *Nickname);
}

void AMahjongLobbyGameState::NotifyPlayerLogout(AController* Controller)
{
    if (!Controller || !Controller->PlayerState)
    {
        return;
    }

    FString PlayerID = Controller->PlayerState->GetPlayerName();
    FString Nickname = Controller->PlayerState->GetPlayerName();

    // Report to monitor
    UMonitorManager::GetInstance(this)->OnPlayerDisconnected(PlayerID, Nickname);

    // Update total player count
    TotalPlayerCount = PlayerArray.Num();

    UE_LOG(LogTemp, Log, TEXT("[SERVER] Player logout event: %s (ID: %s)"), *Nickname, *PlayerID);
}

void AMahjongLobbyGameState::UpdateMonitorPlayerList()
{
    if (!HasAuthority())
    {
        return;
    }

    // Build hall player list
    TArray<FMonitorPlayerInfo> HallPlayers;
    for (APlayerState* PS : PlayerArray)
    {
        if (AMahjongPlayerStateBase* MahjongPS = Cast<AMahjongPlayerStateBase>(PS))
        {
            FMonitorPlayerInfo PlayerInfo;
            PlayerInfo.PlayerID = FString::FromInt(PS->GetPlayerId());
            PlayerInfo.Nickname = PS->GetPlayerName();
            PlayerInfo.PlayerIndex = MahjongPS->GetPlayerIndex();
            PlayerInfo.LoginTime = FDateTime::UtcNow();
            PlayerInfo.bIsGuest = false;
            HallPlayers.Add(PlayerInfo);
        }
    }

    // Sync to monitor
    UMonitorManager::GetInstance(this)->SyncHallPlayers(HallPlayers);
}
