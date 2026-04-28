#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MonitorConfig.generated.h"

//==============================================================================
// 监控事件类型
//==============================================================================
UENUM(BlueprintType)
enum class EMonitorEventType : uint8
{
    PlayerLogin,
    PlayerLogout,
    PlayerJoinRoom,
    PlayerLeaveRoom,
    RoomCreated,
    RoomClosed,
    GameStarted,
    GameEnded
};

//==============================================================================
// 玩家信息结构体（推送用）
//==============================================================================
USTRUCT(BlueprintType)
struct FMonitorPlayerInfo
{
    GENERATED_BODY()
    UPROPERTY()
    FString PlayerID;
    UPROPERTY()
    FString Nickname;
    UPROPERTY()
    int32 PlayerIndex;
    UPROPERTY()
    FString SessionToken;
    UPROPERTY()
    FDateTime LoginTime;
    UPROPERTY()
    bool bIsGuest;
};

//==============================================================================
// 房间信息结构体（推送用）
//==============================================================================
USTRUCT(BlueprintType)
struct FMonitorRoomInfo
{
    GENERATED_BODY()
    UPROPERTY()
    FString RoomID;
    UPROPERTY()
    FString RoomName;
    UPROPERTY()
    int32 CurrentPlayers;
    UPROPERTY()
    int32 MaxPlayers;
    UPROPERTY()
    TArray<FMonitorPlayerInfo> Players;
};

//==============================================================================
// 监控消息结构体
//==============================================================================
USTRUCT(BlueprintType)
struct FMonitorMessage
{
    GENERATED_BODY()
    UPROPERTY()
    EMonitorEventType EventType;
    UPROPERTY()
    FString EventID;
    UPROPERTY()
    FDateTime Timestamp;
    UPROPERTY()
    FString HallID;
    UPROPERTY()
    FString RoomID;
    UPROPERTY()
    FMonitorPlayerInfo Player;
    UPROPERTY()
    TArray<FMonitorPlayerInfo> PlayersInHall;
    UPROPERTY()
    TArray<FMonitorPlayerInfo> PlayersInRoom;
    UPROPERTY()
    FMonitorRoomInfo Room;
    UPROPERTY()
    FString Ext;
};

//==============================================================================
// 监控服务接口（抽象基类）
//==============================================================================
class IMonitorBackend
{
public:
    virtual ~IMonitorBackend() = default;
    virtual void Initialize(const FMonitorConfig& Config) = 0;
    virtual void Shutdown() = 0;
    virtual bool IsEnabled() const = 0;
    virtual bool IsConnected() const = 0;
    virtual void SendMessage(const FMonitorMessage& Message) = 0;
    virtual void UpdatePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers, const TMap<FString, FMonitorRoomInfo>& Rooms) = 0;
};

//==============================================================================
// 空实现（零开销模式）
//==============================================================================
class FNullMonitorBackend : public IMonitorBackend
{
public:
    virtual void Initialize(const FMonitorConfig& Config) override {}
    virtual void Shutdown() override {}
    virtual bool IsEnabled() const override { return false; }
    virtual bool IsConnected() const override { return false; }
    virtual void SendMessage(const FMonitorMessage& Message) override {}
    virtual void UpdatePlayerList(const TArray<FMonitorPlayerInfo>& HallPlayers, const TMap<FString, FMonitorRoomInfo>& Rooms) override {}
};

//==============================================================================
// 连接类型枚举
//==============================================================================
UENUM(BlueprintType)
enum class EMonitorConnectionType : uint8
{
    None            UMETA(DisplayName = "Disabled (Zero Overhead)"),
    WebSocket       UMETA(DisplayName = "WebSocket"),
    RedisPubSub     UMETA(DisplayName = "Redis Pub/Sub"),
    RedisStreams    UMETA(DisplayName = "Redis Streams"),
    HTTP_POST       UMETA(DisplayName = "HTTP POST")
};

//==============================================================================
// 配置结构体
//==============================================================================
USTRUCT(BlueprintType)
struct FMonitorConfig
{
    GENERATED_BODY()
    // 是否启用监控推送（默认关闭）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    bool bEnableMonitoring = false;

    // 连接类型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    EMonitorConnectionType ConnectionType = EMonitorConnectionType::None;

    // 监控服务器地址
    // WebSocket: ws://localhost:8080/monitor
    // Redis Pub/Sub: redis://localhost:6379
    // HTTP POST: http://localhost:8080/monitor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    FString MonitorServerURL = TEXT("redis://localhost:6379");

    // Redis频道名称（用于Redis Pub/Sub模式）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    FString RedisChannel = TEXT("mahjong:monitor");

    // 心跳间隔（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    float HeartbeatInterval = 30.0f;

    // 重连间隔（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    float ReconnectInterval = 5.0f;

    // 最大重连次数（0=无限重试）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    int32 MaxReconnectAttempts = 0;

    // 超时时间（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    float Timeout = 10.0f;

    // 认证Token（可选）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Monitor")
    FString AuthToken;
};
