#pragma once
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "UObject/NoExportTypes.h"
#include "Gameplay/Monitor/MonitorConfig.h"
#include "RedisMonitorBackend.generated.h"

/**
 * Redis Pub/Sub 监控后端实现
 *
 * 架构说明:
 * - 使用Redis PUBLISH命令发送消息到指定频道
 * - Web监控应用通过Redis SUBSCRIBE订阅同一频道
 * - 支持多个Web监控实例同时订阅（fan-out）
 * - 使用HTTP REST API + Lua脚本实现Redis命令（兼容各种HTTP Redis客户端）
 * - 也支持直接TCP连接（使用hiredis）
 *
 * 零影响设计:
 * - 当bEnableMonitoring=false时，使用FNullMonitorBackend，完全无开销
 * - 当Redis不可用时，消息入队后丢弃，不阻塞主逻辑
 * - 后台异步重连，不影响游戏服主循环
 */
UCLASS()
class MAHJONGGAME_API URedisMonitorBackend : public UObject, public IMonitorBackend
{
    GENERATED_BODY()

public:
    URedisMonitorBackend();
    virtual ~URedisMonitorBackend();

    // IMonitorBackend interface
    virtual void Initialize(const FMonitorConfig& Config) override;
    virtual void Shutdown() override;
    virtual bool IsEnabled() const override { return Config.bEnableMonitoring; }
    virtual bool IsConnected() const override { return bIsConnected; }
    virtual void SendMessage(const FMonitorMessage& Message) override;
    virtual void UpdatePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers,
                                  const TMap<FString, FMonitorRoomInfo>& Rooms) override;

    // Redis specific methods
    UFUNCTION(BlueprintCallable, Category="Monitor|Redis")
    void SetRedisURL(const FString& URL);

    UFUNCTION(BlueprintCallable, Category="Monitor|Redis")
    void SetChannelName(const FString& Channel);

protected:
    // Redis connection handling
    void Connect();
    void Disconnect();
    bool SendRedisCommand(const FString& Command);
    void OnResponseReceived(void* Request, void* Response, bool bSuccess);

    // Reconnection
    void ScheduleReconnect();
    void CancelReconnect();
    void OnReconnectTimer();

    // JSON serialization
    FString SerializeMessage(const FMonitorMessage& Message);
    FString SerializePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers,
                                const TMap<FString, FMonitorRoomInfo>& Rooms);

    // Queue processing
    void ProcessQueuedMessages();

private:
    UPROPERTY()
    FMonitorConfig Config;

    UPROPERTY()
    FString RedisURL;

    UPROPERTY()
    FString ChannelName;

    bool bIsConnected;
    bool bIsConnecting;
    bool bShutdownRequested;
    int32 ReconnectAttempts;
    FTimerHandle ReconnectTimerHandle;
    TArray<FString> MessageQueue;
    FCriticalSection QueueMutex;
    FString LastError;
};
