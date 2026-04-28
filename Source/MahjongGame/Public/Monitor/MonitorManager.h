#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Monitor/MonitorConfig.h"
#include "MonitorManager.generated.h"

class UMonitorService;

/**
 * 全局监控管理器
 * 提供游戏全局唯一的监控服务访问点
 * 建议在GameInstance或GameMode中初始化
 */
UCLASS()
class MAHJONGGAME_API UMonitorManager : public UObject
{
    GENERATED_BODY()

public:
    UMonitorManager();

    // 获取单例实例
    UFUNCTION(BlueprintCallable, Category="Monitor", meta=(WorldContext="WorldContextObject"))
    static UMonitorManager* GetInstance(UObject* WorldContextObject);

    // 初始化监控（应在游戏开始时调用一次）
    UFUNCTION(BlueprintCallable, Category="Monitor")
    void Initialize(bool bEnable, const FString& ServerURL = TEXT("redis://localhost:6379"));

    // 关闭监控
    UFUNCTION(BlueprintCallable, Category="Monitor")
    void Shutdown();

    // 检查是否启用
    UFUNCTION(BlueprintPure, Category="Monitor")
    bool IsEnabled() const;

    // 检查连接状态
    UFUNCTION(BlueprintPure, Category="Monitor")
    bool IsConnected() const;

    //============================================================================
    // 事件推送（调用这些方法即可触发推送，无需关心连接状态）
    //============================================================================

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnPlayerConnected(const FString& PlayerID, const FString& Nickname, bool bIsGuest);

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnPlayerDisconnected(const FString& PlayerID, const FString& Nickname);

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnPlayerJoinedRoom(const FString& PlayerID, const FString& Nickname, const FString& RoomID);

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnPlayerLeftRoom(const FString& PlayerID, const FString& Nickname, const FString& RoomID);

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnRoomCreated(const FString& RoomID, const FString& RoomName, int32 MaxPlayers);

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnRoomClosed(const FString& RoomID);

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnGameStarted(const FString& RoomID, const TArray<FString>& PlayerIDs);

    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void OnGameEnded(const FString& RoomID);

    //============================================================================
    // 状态同步（定期调用以更新玩家列表）
    //============================================================================

    UFUNCTION(BlueprintCallable, Category="Monitor|Status")
    void SyncHallPlayers(const TArray<FMonitorPlayerInfo>& Players);

    UFUNCTION(BlueprintCallable, Category="Monitor|Status")
    void SyncRoomPlayers(const FString& RoomID, const TArray<FMonitorPlayerInfo>& Players);

    UFUNCTION(BlueprintCallable, Category="Monitor|Status")
    void SyncAllRooms(const TMap<FString, FMonitorRoomInfo>& Rooms);

protected:
    // 内部初始化
    void EnsureInitialized();

private:
    // 内部获取单例指针
    static UMonitorManager* GetInternalSingleton();

    UPROPERTY()
    UMonitorService* MonitorService;

    bool bInitialized;
    bool bEnableFlag;
    FString MonitorServerURL;

    // 静态单例实例
    static UMonitorManager* SingletonInstance;

    // 单例互斥锁 - 保证线程安全
    static FCriticalSection SingletonMutex;
};
