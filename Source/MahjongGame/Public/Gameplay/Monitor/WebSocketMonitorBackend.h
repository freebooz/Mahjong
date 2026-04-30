#pragma once
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "Gameplay/Monitor/MonitorConfig.h"
#include "WebSocketMonitorBackend.generated.h"

/**
 * WebSocket监控后端实现
 * 使用UE的HTTP模块实现WebSocket通信（非阻塞）
 * 当连接不可用时自动降级，不影响主逻辑
 */
UCLASS()
class MAHJONGGAME_API UWebSocketMonitorBackend : public UObject, public IMonitorBackend
{
    GENERATED_BODY()

public:
    UWebSocketMonitorBackend();
    virtual ~UWebSocketMonitorBackend();

    // IMonitorBackend interface
    virtual void Initialize(const FMonitorConfig& Config) override;
    virtual void Shutdown() override;
    virtual bool IsEnabled() const override { return Config.bEnableMonitoring; }
    virtual bool IsConnected() const override { return bIsConnected; }
    virtual void SendMessage(const FMonitorMessage& Message) override;
    virtual void UpdatePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers, const TMap<FString, FMonitorRoomInfo>& Rooms) override;

    // Public methods for external triggering
    void OnPlayerLogin(const FMonitorPlayerInfo& Player);
    void OnPlayerLogout(const FMonitorPlayerInfo& Player);
    void OnPlayerJoinRoom(const FMonitorPlayerInfo& Player, const FString& RoomID);
    void OnPlayerLeaveRoom(const FMonitorPlayerInfo& Player, const FString& RoomID);
    void OnRoomCreated(const FMonitorRoomInfo& Room);
    void OnRoomClosed(const FString& RoomID);

protected:
    // Internal WebSocket handling
    void Connect();
    void Disconnect();
    void SendWebSocketFrame(const FString& Data);
    void SendHttpUpgradeRequest();
    void OnHttpResponseReceived(void* Request, void* Response, bool bSuccess);
    void OnWebSocketConnected();
    void OnWebSocketError(const FString& Error);
    void ProcessQueuedMessages();

    // Heartbeat handling
    void StartHeartbeat();
    void StopHeartbeat();
    void OnHeartbeatTimer();

    // Reconnection handling
    void ScheduleReconnect();
    void CancelReconnect();
    void OnReconnectTimer();

    // JSON serialization helpers
    FString SerializeMessage(const FMonitorMessage& Message);
    FString SerializePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers,
                               const TMap<FString, FMonitorRoomInfo>& Rooms);

private:
    UPROPERTY()
    FMonitorConfig Config;

    bool bIsConnected;
    bool bIsConnecting;
    bool bShutdownRequested;
    int32 ReconnectAttempts;
    FTimerHandle HeartbeatTimerHandle;
    FTimerHandle ReconnectTimerHandle;
    TArray<FString> MessageQueue;
    FCriticalSection QueueMutex;
    FString LastError;
};
