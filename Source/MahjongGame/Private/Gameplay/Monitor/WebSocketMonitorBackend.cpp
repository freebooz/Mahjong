#include "Gameplay/Monitor/WebSocketMonitorBackend.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UWebSocketMonitorBackend::UWebSocketMonitorBackend()
{
    bIsConnected = false;
    bIsConnecting = false;
    bShutdownRequested = false;
    ReconnectAttempts = 0;
}

UWebSocketMonitorBackend::~UWebSocketMonitorBackend()
{
    Shutdown();
}

void UWebSocketMonitorBackend::Initialize(const FMonitorConfig& InConfig)
{
    Config = InConfig;
    UE_LOG(LogTemp, Log, TEXT("[WebSocketBackend] Initialized"));
}

void UWebSocketMonitorBackend::Shutdown()
{
    bShutdownRequested = true;
    Disconnect();
    UE_LOG(LogTemp, Log, TEXT("[WebSocketBackend] Shutdown"));
}

void UWebSocketMonitorBackend::Connect()
{
    // Stub: No connection for client build
    bIsConnecting = false;
    bIsConnected = false;
}

void UWebSocketMonitorBackend::Disconnect()
{
    bIsConnected = false;
    bIsConnecting = false;
}

void UWebSocketMonitorBackend::SendWebSocketFrame(const FString& Data)
{
    // Stub: Just log for client build
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] SendFrame: %d chars"), Data.Len());
}

void UWebSocketMonitorBackend::SendHttpUpgradeRequest()
{
    // Stub: No HTTP upgrade for client build
}

void UWebSocketMonitorBackend::OnHttpResponseReceived(void* Request, void* Response, bool bSuccess)
{
    // Stub: No response handling for client build
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] OnResponse: %d"), bSuccess);
}

void UWebSocketMonitorBackend::OnWebSocketConnected()
{
    // Stub
    UE_LOG(LogTemp, Log, TEXT("[WebSocketBackend] Connected (stub)"));
}

void UWebSocketMonitorBackend::OnWebSocketError(const FString& Error)
{
    // Stub
    UE_LOG(LogTemp, Warning, TEXT("[WebSocketBackend] Error: %s"), *Error);
}

void UWebSocketMonitorBackend::ProcessQueuedMessages()
{
    // Stub: Nothing to process for client build
}

void UWebSocketMonitorBackend::StartHeartbeat()
{
    // Stub: No heartbeat for client build
}

void UWebSocketMonitorBackend::StopHeartbeat()
{
    // Stub
}

void UWebSocketMonitorBackend::OnHeartbeatTimer()
{
    // Stub
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] Heartbeat (stub)"));
}

void UWebSocketMonitorBackend::ScheduleReconnect()
{
    // Stub: No reconnection for client build
}

void UWebSocketMonitorBackend::CancelReconnect()
{
    // Stub
}

void UWebSocketMonitorBackend::OnReconnectTimer()
{
    // Stub
    UE_LOG(LogTemp, Log, TEXT("[WebSocketBackend] Reconnect timer (stub)"));
}

void UWebSocketMonitorBackend::SendMessage(const FMonitorMessage& Message)
{
    if (bShutdownRequested)
    {
        return;
    }

    // Stub: Just log the message for client build
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] SendMessage: %s"), *Message.EventID);
}

void UWebSocketMonitorBackend::UpdatePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers,
                                               const TMap<FString, FMonitorRoomInfo>& Rooms)
{
    if (bShutdownRequested)
    {
        return;
    }

    // Stub: Just log for client build
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] UpdatePlayerList: %d hall players, %d rooms"),
           HallPlayers.Num(), Rooms.Num());
}

void UWebSocketMonitorBackend::OnPlayerLogin(const FMonitorPlayerInfo& Player)
{
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] PlayerLogin: %s"), *Player.Nickname);
}

void UWebSocketMonitorBackend::OnPlayerLogout(const FMonitorPlayerInfo& Player)
{
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] PlayerLogout: %s"), *Player.Nickname);
}

void UWebSocketMonitorBackend::OnPlayerJoinRoom(const FMonitorPlayerInfo& Player, const FString& RoomID)
{
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] PlayerJoinRoom: %s -> %s"), *Player.Nickname, *RoomID);
}

void UWebSocketMonitorBackend::OnPlayerLeaveRoom(const FMonitorPlayerInfo& Player, const FString& RoomID)
{
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] PlayerLeaveRoom: %s <- %s"), *Player.Nickname, *RoomID);
}

void UWebSocketMonitorBackend::OnRoomCreated(const FMonitorRoomInfo& Room)
{
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] RoomCreated: %s"), *Room.RoomID);
}

void UWebSocketMonitorBackend::OnRoomClosed(const FString& RoomID)
{
    UE_LOG(LogTemp, Verbose, TEXT("[WebSocketBackend] RoomClosed: %s"), *RoomID);
}

FString UWebSocketMonitorBackend::SerializeMessage(const FMonitorMessage& Message)
{
    // Stub: Return empty JSON
    return TEXT("{}");
}

FString UWebSocketMonitorBackend::SerializePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers,
                                                      const TMap<FString, FMonitorRoomInfo>& Rooms)
{
    // Stub: Return empty JSON
    return TEXT("{\"players\": [], \"rooms\": []}");
}