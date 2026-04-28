#include "Monitor/MonitorManager.h"
#include "Monitor/MonitorService.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Static instance
UMonitorManager* UMonitorManager::SingletonInstance = nullptr;

UMonitorManager::UMonitorManager()
{
    MonitorService = nullptr;
    bInitialized = false;
    bEnableFlag = false;
}

UMonitorManager* UMonitorManager::GetInternalSingleton()
{
    if (!SingletonInstance)
    {
        SingletonInstance = NewObject<UMonitorManager>();
        SingletonInstance->AddToRoot();
    }
    return SingletonInstance;
}

UMonitorManager* UMonitorManager::GetInstance(UObject* WorldContextObject)
{
    return GetInternalSingleton();
}

void UMonitorManager::Initialize(bool bEnable, const FString& ServerURL)
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("[MonitorManager] Already initialized, skipping..."));
        return;
    }

    bEnableFlag = bEnable;
    MonitorServerURL = ServerURL;

    FMonitorConfig Config;
    Config.bEnableMonitoring = bEnable;
    Config.MonitorServerURL = ServerURL;
    Config.ConnectionType = EMonitorConnectionType::RedisPubSub;
    Config.RedisChannel = TEXT("mahjong:monitor");
    Config.HeartbeatInterval = 30.0f;
    Config.ReconnectInterval = 5.0f;
    Config.MaxReconnectAttempts = 0;
    Config.Timeout = 10.0f;

    MonitorService = NewObject<UMonitorService>();
    MonitorService->AddToRoot();
    MonitorService->Initialize(Config);

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("[MonitorManager] Initialized (enabled=%d, url=%s)"), bEnable, *ServerURL);
}

void UMonitorManager::Shutdown()
{
    if (MonitorService)
    {
        MonitorService->Shutdown();
        MonitorService->RemoveFromRoot();
        MonitorService = nullptr;
    }
    bInitialized = false;

    if (SingletonInstance == this)
    {
        SingletonInstance = nullptr;
    }

    UE_LOG(LogTemp, Log, TEXT("[MonitorManager] Shutdown complete"));
}

bool UMonitorManager::IsEnabled() const
{
    return bEnableFlag && MonitorService && MonitorService->IsEnabled();
}

bool UMonitorManager::IsConnected() const
{
    return MonitorService && MonitorService->IsConnected();
}

void UMonitorManager::EnsureInitialized()
{
    if (!bInitialized)
    {
        Initialize(false); // Default to disabled
    }
}

void UMonitorManager::OnPlayerConnected(const FString& PlayerID, const FString& Nickname, bool bIsGuest)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    int32 PlayerIndex = 0;
    MonitorService->ReportPlayerLogin(PlayerID, Nickname, PlayerIndex, bIsGuest, TEXT(""));
}

void UMonitorManager::OnPlayerDisconnected(const FString& PlayerID, const FString& Nickname)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->ReportPlayerLogout(PlayerID, Nickname);
}

void UMonitorManager::OnPlayerJoinedRoom(const FString& PlayerID, const FString& Nickname, const FString& RoomID)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->ReportPlayerJoinRoom(PlayerID, Nickname, RoomID);
}

void UMonitorManager::OnPlayerLeftRoom(const FString& PlayerID, const FString& Nickname, const FString& RoomID)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->ReportPlayerLeaveRoom(PlayerID, Nickname, RoomID);
}

void UMonitorManager::OnRoomCreated(const FString& RoomID, const FString& RoomName, int32 MaxPlayers)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->ReportRoomCreated(RoomID, RoomName, MaxPlayers, TArray<FString>());
}

void UMonitorManager::OnRoomClosed(const FString& RoomID)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->ReportRoomClosed(RoomID);
}

void UMonitorManager::OnGameStarted(const FString& RoomID, const TArray<FString>& PlayerIDs)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    // Report as room created with players
    MonitorService->ReportRoomCreated(RoomID, TEXT("GameRoom"), 4, PlayerIDs);
}

void UMonitorManager::OnGameEnded(const FString& RoomID)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->ReportRoomClosed(RoomID);
}

void UMonitorManager::SyncHallPlayers(const TArray<FMonitorPlayerInfo>& Players)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->UpdateHallPlayerList(Players);
}

void UMonitorManager::SyncRoomPlayers(const FString& RoomID, const TArray<FMonitorPlayerInfo>& Players)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->UpdateRoomPlayerList(RoomID, Players);
}

void UMonitorManager::SyncAllRooms(const TMap<FString, FMonitorRoomInfo>& Rooms)
{
    EnsureInitialized();
    if (!IsEnabled())
    {
        return;
    }

    MonitorService->UpdateAllRoomsInfo(Rooms);
}
