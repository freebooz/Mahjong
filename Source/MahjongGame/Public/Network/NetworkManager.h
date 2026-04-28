#pragma once
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "UObject/NoExportTypes.h"
#include "NetworkManager.generated.h"

// 前向声明
class FSocket;
class FRunnableThread;
class FInternetAddr;

//==============================================================================
// 网络连接状态枚举
// 描述当前网络管理的连接状态
//==============================================================================
UENUM(BlueprintType)
enum class ENetworkStatus : uint8
{
    Disconnected,   // 未连接
    Connecting,      // 正在连接
    Connected,       // 已连接
    Reconnecting,    // 正在重连
    Error            // 连接错误
};

//==============================================================================
// 网络状态变更回调代理
// 当网络状态发生变化时触发
//==============================================================================
DECLARE_DYNAMIC_DELEGATE(FOnNetworkStatusChanged);

//==============================================================================
// 重连完成回调代理
// 参数：bSuccess - 重连是否成功
//==============================================================================
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnReconnectComplete, bool, bSuccess);

//==============================================================================
// 收到消息回调代理
// 参数：Message - 收到的消息内容
//==============================================================================
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnMessageReceived, const FString&, Message);

//==============================================================================
// 网络管理器
// 负责游戏客户端与服务器之间的网络通信
// 功能：
//   - 建立/断开服务器连接（使用 TCP Socket）
//   - 发送/接收网络消息
//   - 自动重连机制
//   - 消息队列管理
//   - 异步接收线程
//==============================================================================
UCLASS(Blueprintable)
class MAHJONGGAME_API UNetworkManager : public UObject
{
    GENERATED_BODY()

public:
    // 构造函数
    UNetworkManager();

    // 析构函数 - 清理资源
    virtual ~UNetworkManager();

    // 连接到服务器
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    bool ConnectToServer(const FString& IP, int32 Port);

    // 断开服务器连接
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void Disconnect();

    // 发送消息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    bool SendMessage(const FString& Message);

    // 接收消息（从队列）
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    FString ReceiveMessage();

    // 开始重连流程
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void BeginReconnect(const FString& SessionToken);

    // 取消重连
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void CancelReconnect();

    // 获取当前网络状态
    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    ENetworkStatus GetStatus() const { return NetworkStatus; }

    // 获取最后错误信息
    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    FString GetLastError() const { return LastError; }

    // 检查是否正在重连
    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    bool IsReconnecting() const { return bIsReconnecting; }

    // 获取当前重连尝试次数
    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    int32 GetReconnectAttempts() const { return ReconnectAttempts; }

    // 获取当前消息队列中的消息数量
    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    int32 GetQueuedMessageCount() const { return MessageQueue.Num(); }

    // 设置回调
    void SetOnStatusChanged(FOnNetworkStatusChanged Callback) { OnStatusChanged = Callback; }
    void SetOnReconnectComplete(FOnReconnectComplete Callback) { OnReconnectCompleteCallback = Callback; }
    void SetOnMessageReceived(FOnMessageReceived Callback) { OnMessageReceivedCallback = Callback; }

protected:
    // 尝试重连
    void AttemptReconnect();

    // 重连定时器触发
    void OnReconnectTimer();

    // 异步接收线程处理函数
    void ReceiveThreadLoop();

    // 清理 Socket 资源
    void CleanupSocket();

    // 处理 Socket 连接结果
    void HandleConnectionResult(bool bSuccess);

    // 属性
    UPROPERTY()
    ENetworkStatus NetworkStatus;

    UPROPERTY()
    FString LastError;

    UPROPERTY()
    FString ServerIP;

    UPROPERTY()
    int32 ServerPort;

    UPROPERTY()
    FString SessionToken;

    // 消息队列
    UPROPERTY()
    TArray<FString> MessageQueue;

    // 队列访问互斥锁
    FCriticalSection QueueMutex;

    // 发送队列互斥锁
    FCriticalSection SendMutex;

    // 重连配置
    bool bIsReconnecting;
    int32 ReconnectAttempts;

    UPROPERTY(EditAnywhere, Category="Mahjong|Network|Reconnect")
    int32 MaxReconnectAttempts;

    UPROPERTY(EditAnywhere, Category="Mahjong|Network|Reconnect")
    float ReconnectDelaySeconds;

    FTimerHandle ReconnectTimerHandle;
    UPROPERTY()
    FTimerManager* TimerManager;

    // 消息队列配置
    UPROPERTY(EditAnywhere, Category="Mahjong|Network|MessageQueue")
    int32 MaxQueueSize;

    // Socket 相关（不使用 UPROPERTY，因为 Socket 由底层系统管理）
    FSocket* ClientSocket;
    FRunnableThread* ReceiveThread;
    bool bShouldStopThread;

    // 回调代理
    FOnNetworkStatusChanged OnStatusChanged;
    FOnReconnectComplete OnReconnectCompleteCallback;
    FOnMessageReceived OnMessageReceivedCallback;
};