#include "Gameplay/Monitor/RedisMonitorBackend.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

URedisMonitorBackend::URedisMonitorBackend()
{
    bIsConnected = false;
    bIsConnecting = false;
    bShutdownRequested = false;
    ReconnectAttempts = 0;
    RedisURL = TEXT("http://localhost:6379");
    ChannelName = TEXT("mahjong:monitor");
}

URedisMonitorBackend::~URedisMonitorBackend()
{
    Shutdown();
}

void URedisMonitorBackend::Initialize(const FMonitorConfig& InConfig)
{
    Config = InConfig;
    UE_LOG(LogTemp, Log, TEXT("[RedisBackend] Initialized (URL: %s, Channel: %s)"), *RedisURL, *ChannelName);
}

void URedisMonitorBackend::Shutdown()
{
    bShutdownRequested = true;
    Disconnect();
    UE_LOG(LogTemp, Log, TEXT("[RedisBackend] Shutdown"));
}

void URedisMonitorBackend::Connect()
{
    // Stub: No connection for client build
    bIsConnecting = false;
    bIsConnected = false;
}

void URedisMonitorBackend::Disconnect()
{
    bIsConnected = false;
    bIsConnecting = false;
}

bool URedisMonitorBackend::SendRedisCommand(const FString& Command)
{
    // Stub: Just log and return success
    UE_LOG(LogTemp, Verbose, TEXT("[RedisBackend] SendCommand: %s"), *Command);
    return true;
}

void URedisMonitorBackend::OnResponseReceived(void* Request, void* Response, bool bSuccess)
{
    // Stub: No response handling for client build
    UE_LOG(LogTemp, Verbose, TEXT("[RedisBackend] OnResponse: %d"), bSuccess);
}

void URedisMonitorBackend::ScheduleReconnect()
{
    // Stub: No reconnection for client build
}

void URedisMonitorBackend::CancelReconnect()
{
    // Stub
}

void URedisMonitorBackend::OnReconnectTimer()
{
    // Stub
    UE_LOG(LogTemp, Log, TEXT("[RedisBackend] Reconnect timer (stub)"));
}

void URedisMonitorBackend::SendMessage(const FMonitorMessage& Message)
{
    if (bShutdownRequested)
    {
        return;
    }

    // Stub: Just log the message for client build
    UE_LOG(LogTemp, Verbose, TEXT("[RedisBackend] SendMessage: %s"), *Message.EventID);
}

void URedisMonitorBackend::UpdatePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers,
                                           const TMap<FString, FMonitorRoomInfo>& Rooms)
{
    if (bShutdownRequested)
    {
        return;
    }

    // Stub: Just log for client build
    UE_LOG(LogTemp, Verbose, TEXT("[RedisBackend] UpdatePlayerList: %d hall players, %d rooms"),
           HallPlayers.Num(), Rooms.Num());
}

void URedisMonitorBackend::SetRedisURL(const FString& URL)
{
    RedisURL = URL;
    UE_LOG(LogTemp, Log, TEXT("[RedisBackend] RedisURL set to: %s"), *URL);
}

void URedisMonitorBackend::SetChannelName(const FString& Channel)
{
    ChannelName = Channel;
    UE_LOG(LogTemp, Log, TEXT("[RedisBackend] Channel set to: %s"), *Channel);
}

FString URedisMonitorBackend::SerializeMessage(const FMonitorMessage& Message)
{
    // Stub: Return empty JSON
    return TEXT("{}");
}

FString URedisMonitorBackend::SerializePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers,
                                                  const TMap<FString, FMonitorRoomInfo>& Rooms)
{
    // Stub: Return empty JSON
    return TEXT("{\"players\": [], \"rooms\": []}");
}

void URedisMonitorBackend::ProcessQueuedMessages()
{
    // Stub: Nothing to process for client build
}