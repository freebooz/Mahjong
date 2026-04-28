#include "Monitor/MonitorService.h"
#include "Monitor/WebSocketMonitorBackend.h"
#include "Monitor/RedisMonitorBackend.h"

// Static instance for null backend
FNullMonitorBackend UMonitorService::NullBackendInstance;

UMonitorService::UMonitorService()
{
    Backend = nullptr;
    bInitialized = false;
}

void UMonitorService::Initialize(const FMonitorConfig& InConfig)
{
    Config = InConfig;
    bInitialized = true;

    if (!Config.bEnableMonitoring)
    {
        UE_LOG(LogTemp, Log, TEXT("[MonitorService] Monitoring disabled - using NULL backend (zero overhead)"));
        UseNullBackend();
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[MonitorService] Initializing with monitoring enabled"));

    // Select backend based on connection type
    switch (Config.ConnectionType)
    {
    case EMonitorConnectionType::WebSocket:
        UseWebSocketBackend();
        break;
    case EMonitorConnectionType::RedisPubSub:
    case EMonitorConnectionType::RedisStreams:
        UseRedisBackend();
        break;
    case EMonitorConnectionType::HTTP_POST:
        UseWebSocketBackend(); // Use WebSocket as fallback
        break;
    case EMonitorConnectionType::None:
    default:
        UseNullBackend();
        break;
    }
}

void UMonitorService::Shutdown()
{
    if (Backend)
    {
        Backend->Shutdown();
        ReleaseBackend();
    }
    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("[MonitorService] Shutdown complete"));
}

bool UMonitorService::IsEnabled() const
{
    return Config.bEnableMonitoring;
}

bool UMonitorService::IsConnected() const
{
    if (Backend)
    {
        return Backend->IsConnected();
    }
    return false;
}

FMonitorConfig UMonitorService::GetConfig() const
{
    return Config;
}

IMonitorBackend* UMonitorService::GetOrCreateBackend()
{
    if (!Backend)
    {
        if (Config.bEnableMonitoring)
        {
            switch (Config.ConnectionType)
            {
            case EMonitorConnectionType::RedisPubSub:
            case EMonitorConnectionType::RedisStreams:
                UseRedisBackend();
                break;
            case EMonitorConnectionType::WebSocket:
            case EMonitorConnectionType::HTTP_POST:
            default:
                UseWebSocketBackend();
                break;
            }
        }
        else
        {
            UseNullBackend();
        }
    }
    return Backend;
}

void UMonitorService::ReleaseBackend()
{
    if (Backend && Backend != &NullBackendInstance)
    {
        delete Backend;
    }
    Backend = nullptr;
}

void UMonitorService::UseNullBackend()
{
    ReleaseBackend();
    Backend = &NullBackendInstance;
}

void UMonitorService::UseWebSocketBackend()
{
    ReleaseBackend();

    UWebSocketMonitorBackend* WSBackend = NewObject<UWebSocketMonitorBackend>();
    WSBackend->AddToRoot();
    WSBackend->Initialize(Config);
    Backend = WSBackend;
}

void UMonitorService::UseRedisBackend()
{
    ReleaseBackend();

    URedisMonitorBackend* RedisBackend = NewObject<URedisMonitorBackend>();
    RedisBackend->AddToRoot();
    RedisBackend->Initialize(Config);
    Backend = RedisBackend;
}

FString UMonitorService::GetLastError() const
{
    if (Backend && Backend->IsEnabled())
    {
        return TEXT("No error");
    }
    return TEXT("Monitor disabled");
}

void UMonitorService::ReportPlayerLogin(const FString& PlayerID, const FString& Nickname,
                                        int32 PlayerIndex, bool bIsGuest, const FString& SessionToken)
{
    if (!IsEnabled())
    {
        return;
    }

    FMonitorPlayerInfo Player;
    Player.PlayerID = PlayerID;
    Player.Nickname = Nickname;
    Player.PlayerIndex = PlayerIndex;
    Player.bIsGuest = bIsGuest;
    Player.SessionToken = SessionToken;
    Player.LoginTime = FDateTime::UtcNow();

    FMonitorMessage Message;
    Message.EventType = EMonitorEventType::PlayerLogin;
    Message.EventID = FGuid::NewGuid().ToString();
    Message.Timestamp = FDateTime::UtcNow();
    Message.Player = Player;

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->SendMessage(Message);
    }
}

void UMonitorService::ReportPlayerLogout(const FString& PlayerID, const FString& Nickname)
{
    if (!IsEnabled())
    {
        return;
    }

    FMonitorPlayerInfo Player;
    Player.PlayerID = PlayerID;
    Player.Nickname = Nickname;

    FMonitorMessage Message;
    Message.EventType = EMonitorEventType::PlayerLogout;
    Message.EventID = FGuid::NewGuid().ToString();
    Message.Timestamp = FDateTime::UtcNow();
    Message.Player = Player;

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->SendMessage(Message);
    }
}

void UMonitorService::ReportPlayerJoinRoom(const FString& PlayerID, const FString& Nickname,
                                           const FString& RoomID)
{
    if (!IsEnabled())
    {
        return;
    }

    FMonitorPlayerInfo Player;
    Player.PlayerID = PlayerID;
    Player.Nickname = Nickname;

    FMonitorMessage Message;
    Message.EventType = EMonitorEventType::PlayerJoinRoom;
    Message.EventID = FGuid::NewGuid().ToString();
    Message.Timestamp = FDateTime::UtcNow();
    Message.Player = Player;
    Message.RoomID = RoomID;

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->SendMessage(Message);
    }
}

void UMonitorService::ReportPlayerLeaveRoom(const FString& PlayerID, const FString& Nickname,
                                            const FString& RoomID)
{
    if (!IsEnabled())
    {
        return;
    }

    FMonitorPlayerInfo Player;
    Player.PlayerID = PlayerID;
    Player.Nickname = Nickname;

    FMonitorMessage Message;
    Message.EventType = EMonitorEventType::PlayerLeaveRoom;
    Message.EventID = FGuid::NewGuid().ToString();
    Message.Timestamp = FDateTime::UtcNow();
    Message.Player = Player;
    Message.RoomID = RoomID;

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->SendMessage(Message);
    }
}

void UMonitorService::ReportRoomCreated(const FString& RoomID, const FString& RoomName,
                                        int32 MaxPlayers, const TArray<FString>& PlayerIDs)
{
    if (!IsEnabled())
    {
        return;
    }

    FMonitorRoomInfo Room;
    Room.RoomID = RoomID;
    Room.RoomName = RoomName;
    Room.MaxPlayers = MaxPlayers;
    Room.CurrentPlayers = PlayerIDs.Num();

    FMonitorMessage Message;
    Message.EventType = EMonitorEventType::RoomCreated;
    Message.EventID = FGuid::NewGuid().ToString();
    Message.Timestamp = FDateTime::UtcNow();
    Message.Room = Room;

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->SendMessage(Message);
    }
}

void UMonitorService::ReportRoomClosed(const FString& RoomID)
{
    if (!IsEnabled())
    {
        return;
    }

    FMonitorMessage Message;
    Message.EventType = EMonitorEventType::RoomClosed;
    Message.EventID = FGuid::NewGuid().ToString();
    Message.Timestamp = FDateTime::UtcNow();
    Message.RoomID = RoomID;

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->SendMessage(Message);
    }
}

void UMonitorService::UpdateHallPlayerList(const TArray<FMonitorPlayerInfo>& Players)
{
    if (!IsEnabled())
    {
        return;
    }

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->UpdatePlayerList(Players, TMap<FString, FMonitorRoomInfo>());
    }
}

void UMonitorService::UpdateRoomPlayerList(const FString& RoomID, const TArray<FMonitorPlayerInfo>& Players)
{
    if (!IsEnabled())
    {
        return;
    }

    // Build room map with single room
    TMap<FString, FMonitorRoomInfo> Rooms;
    FMonitorRoomInfo Room;
    Room.RoomID = RoomID;
    Room.CurrentPlayers = Players.Num();
    Room.Players = Players;
    Rooms.Add(RoomID, Room);

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->UpdatePlayerList(TArray<FMonitorPlayerInfo>(), Rooms);
    }
}

void UMonitorService::UpdateAllRoomsInfo(const TMap<FString, FMonitorRoomInfo>& Rooms)
{
    if (!IsEnabled())
    {
        return;
    }

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->UpdatePlayerList(TArray<FMonitorPlayerInfo>(), Rooms);
    }
}

void UMonitorService::UpdateFullStatus(const TArray<FMonitorPlayerInfo>& HallPlayers,
                                        const TMap<FString, FMonitorRoomInfo>& Rooms)
{
    if (!IsEnabled())
    {
        return;
    }

    Backend = GetOrCreateBackend();
    if (Backend && Backend->IsEnabled())
    {
        Backend->UpdatePlayerList(HallPlayers, Rooms);
    }
}
