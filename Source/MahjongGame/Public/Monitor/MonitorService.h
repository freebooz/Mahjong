#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Monitor/MonitorConfig.h"
#include "MonitorService.generated.h"

class IMonitorBackend;

/**
 * 监控服务主类
 * 游戏服务端通过此类向Web监控应用推送事件
 * 此类是完全解耦的，对主逻辑零影响
 */
UCLASS()
class MAHJONGGAME_API UMonitorService : public UObject
{
    GENERATED_BODY()

public:
    UMonitorService();

    // 初始化监控服务
    UFUNCTION(BlueprintCallable, Category="Monitor")
    void Initialize(const FMonitorConfig& Config);

    // 关闭监控服务
    UFUNCTION(BlueprintCallable, Category="Monitor")
    void Shutdown();

    // 检查是否启用
    UFUNCTION(BlueprintPure, Category="Monitor")
    bool IsEnabled() const;

    // 检查是否已连接
    UFUNCTION(BlueprintPure, Category="Monitor")
    bool IsConnected() const;

    //============================================================================
    // 事件推送接口
    //============================================================================

    // 玩家登录事件
    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void ReportPlayerLogin(const FString& PlayerID, const FString& Nickname,
                           int32 PlayerIndex, bool bIsGuest, const FString& SessionToken);

    // 玩家退出事件
    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void ReportPlayerLogout(const FString& PlayerID, const FString& Nickname);

    // 玩家加入房间事件
    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void ReportPlayerJoinRoom(const FString& PlayerID, const FString& Nickname,
                              const FString& RoomID);

    // 玩家离开房间事件
    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void ReportPlayerLeaveRoom(const FString& PlayerID, const FString& Nickname,
                               const FString& RoomID);

    // 房间创建事件
    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void ReportRoomCreated(const FString& RoomID, const FString& RoomName,
                           int32 MaxPlayers, const TArray<FString>& PlayerIDs);

    // 房间关闭事件
    UFUNCTION(BlueprintCallable, Category="Monitor|Events")
    void ReportRoomClosed(const FString& RoomID);

    //============================================================================
    // 实时状态推送接口
    //============================================================================

    // 更新大厅玩家列表
    UFUNCTION(BlueprintCallable, Category="Monitor|Status")
    void UpdateHallPlayerList(const TArray<FMonitorPlayerInfo>& Players);

    // 更新房间玩家列表
    UFUNCTION(BlueprintCallable, Category="Monitor|Status")
    void UpdateRoomPlayerList(const FString& RoomID, const TArray<FMonitorPlayerInfo>& Players);

    // 更新所有房间信息
    UFUNCTION(BlueprintCallable, Category="Monitor|Status")
    void UpdateAllRoomsInfo(const TMap<FString, FMonitorRoomInfo>& Rooms);

    // 批量更新（推荐，减少网络开销）
    UFUNCTION(BlueprintCallable, Category="Monitor|Status")
    void UpdateFullStatus(const TArray<FMonitorPlayerInfo>& HallPlayers,
                          const TMap<FString, FMonitorRoomInfo>& Rooms);

    //============================================================================
    // 辅助接口
    //============================================================================

    // 获取最后错误
    UFUNCTION(BlueprintPure, Category="Monitor")
    FString GetLastError() const;

    // 获取当前配置
    UFUNCTION(BlueprintPure, Category="Monitor")
    FMonitorConfig GetConfig() const;

protected:
    // 内部方法：获取或创建后端
    IMonitorBackend* GetOrCreateBackend();

    // 内部方法：释放后端
    void ReleaseBackend();

private:
    // 使用空后端（零开销）
    void UseNullBackend();

    // 使用WebSocket后端
    void UseWebSocketBackend();

    // 使用Redis后端
    void UseRedisBackend();

    UPROPERTY()
    FMonitorConfig Config;

    // 后端指针（可以是空后端或WebSocket后端）
    IMonitorBackend* Backend;

    // 空后端单例
    static FNullMonitorBackend NullBackendInstance;

    // 是否已初始化
    bool bInitialized;
};
