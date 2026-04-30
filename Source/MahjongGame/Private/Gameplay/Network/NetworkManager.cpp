//==============================================================================
// 麻将游戏 - 网络管理器实现
// NetworkManager.cpp
//
// 负责游戏客户端与服务器之间的 TCP Socket 网络通信
// 支持：
//   - TCP Socket 连接
//   - 异步接收线程
//   - 自动重连（指数退避）
//   - 消息队列管理
//==============================================================================

#include "Gameplay/Network/NetworkManager.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "HAL/RunnableThread.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ScopeLock.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

//==============================================================================
// 构造函数
//==============================================================================
UNetworkManager::UNetworkManager()
{
    NetworkStatus = ENetworkStatus::Disconnected;
    ServerPort = 7777;
    bIsReconnecting = false;
    ReconnectAttempts = 0;
    MaxReconnectAttempts = 5;
    ReconnectDelaySeconds = 1.0f;
    MaxQueueSize = 256;
    ClientSocket = nullptr;
    ReceiveThread = nullptr;
    bShouldStopThread = false;
}

//==============================================================================
// 析构函数
// 清理所有资源
//==============================================================================
UNetworkManager::~UNetworkManager()
{
    Disconnect();
}

//==============================================================================
// 连接到服务器
// 使用 TCP Socket 建立连接
//==============================================================================
bool UNetworkManager::ConnectToServer(const FString& IP, int32 Port)
{
    // 保存连接信息
    ServerIP = IP;
    ServerPort = Port;

    UE_LOG(LogTemp, Log, TEXT("[Network] 正在连接到服务器 %s:%d"), *ServerIP, ServerPort);

    // 清理旧的 Socket
    CleanupSocket();

    // 标记为正在连接
    NetworkStatus = ENetworkStatus::Connecting;

    // 创建 TCP Socket
    ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("MahjongClient"), false);

    if (!ClientSocket)
    {
        LastError = TEXT("无法创建 Socket");
        UE_LOG(LogTemp, Error, TEXT("[Network] %s"), *LastError);
        NetworkStatus = ENetworkStatus::Error;
        return false;
    }

    // 解析服务器地址
    TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    // 解析 IP 地址字符串
    FIPv4Address IPv4Addr;
    if (FIPv4Address::Parse(IP, IPv4Addr))
    {
        RemoteAddress->SetIp(static_cast<uint32>(IPv4Addr.Value));
    }
    RemoteAddress->SetPort(Port);

    // 尝试连接
    bool bConnected = ClientSocket->Connect(*RemoteAddress);

    if (bConnected)
    {
        UE_LOG(LogTemp, Log, TEXT("[Network] 连接成功！"));

        // 启动接收线程
        // 注意: UNetworkManager 不能直接作为 FRunnable 使用，需要实现一个单独的 FRunnable 类
        // 暂时禁用线程接收，依赖主线程轮询
        bShouldStopThread = false;
        UE_LOG(LogTemp, Warning, TEXT("[Network] 接收线程已禁用，使用主线程轮询模式"));;

        NetworkStatus = ENetworkStatus::Connected;

        // 触发连接成功回调
        HandleConnectionResult(true);
    }
    else
    {
        LastError = TEXT("连接被拒绝");
        UE_LOG(LogTemp, Error, TEXT("[Network] %s"), *LastError);

        CleanupSocket();
        NetworkStatus = ENetworkStatus::Error;
    }

    return bConnected;
}

//==============================================================================
// 清理 Socket 资源
//==============================================================================
void UNetworkManager::CleanupSocket()
{
    // 停止接收线程
    bShouldStopThread = true;

    if (ReceiveThread)
    {
        ReceiveThread->WaitForCompletion();
        delete ReceiveThread;
        ReceiveThread = nullptr;
    }

    // 关闭 Socket
    if (ClientSocket)
    {
        ClientSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
        ClientSocket = nullptr;
    }
}

//==============================================================================
// 断开服务器连接
//==============================================================================
void UNetworkManager::Disconnect()
{
    UE_LOG(LogTemp, Log, TEXT("[Network] 正在断开连接"));

    // 取消重连
    CancelReconnect();

    // 清理 Socket
    CleanupSocket();

    // 清空消息队列
    {
        FScopeLock Lock(&QueueMutex);
        MessageQueue.Empty();
    }

    NetworkStatus = ENetworkStatus::Disconnected;

    UE_LOG(LogTemp, Log, TEXT("[Network] 连接已断开"));
}

//==============================================================================
// 发送消息
//==============================================================================
bool UNetworkManager::SendMessage(const FString& Message)
{
    if (NetworkStatus != ENetworkStatus::Connected)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] 无法发送消息：未连接"));
        return false;
    }

    if (Message.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] 无法发送空消息"));
        return false;
    }

    if (!ClientSocket)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] Socket 无效"));
        return false;
    }

    // 线程安全地发送
    FScopeLock Lock(&SendMutex);

    // 添加消息结束标记（换行符）
    FString MessageWithTerminator = Message + TEXT("\n");
    TArray<uint8> Data;
    Data.Append((uint8*)TCHAR_TO_UTF8(*MessageWithTerminator), MessageWithTerminator.Len());

    // 发送数据
    int32 BytesSent = 0;
    bool bSuccess = ClientSocket->Send(Data.GetData(), Data.Num(), BytesSent);

    if (bSuccess && BytesSent > 0)
    {
        UE_LOG(LogTemp, Verbose, TEXT("[Network] 已发送 %d 字节"), BytesSent);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Network] 发送失败"));
        return false;
    }

    return true;
}

//==============================================================================
// 接收消息
//==============================================================================
FString UNetworkManager::ReceiveMessage()
{
    FString Msg;

    {
        FScopeLock Lock(&QueueMutex);

        if (MessageQueue.Num() > 0)
        {
            Msg = MessageQueue[0];
            MessageQueue.RemoveAt(0);

            UE_LOG(LogTemp, Verbose, TEXT("[Network] 已取出消息，剩余队列长度: %d"), MessageQueue.Num());
        }
    }

    return Msg;
}

//==============================================================================
// 接收线程循环
//==============================================================================
void UNetworkManager::ReceiveThreadLoop()
{
    UE_LOG(LogTemp, Log, TEXT("[Network] 接收线程启动"));

    TArray<uint8> RecvBuffer;
    RecvBuffer.SetNumUninitialized(8192);  // 8KB 缓冲区

    while (!bShouldStopThread && ClientSocket && NetworkStatus == ENetworkStatus::Connected)
    {
        uint32 PendingData = 0;
        if (ClientSocket->HasPendingData(PendingData))  // 检查是否有数据
        {
            int32 BytesRead = 0;

            if (ClientSocket->Recv(RecvBuffer.GetData(), RecvBuffer.Num(), BytesRead))
            {
                if (BytesRead > 0)
                {
                    // 转换为字符串
                    RecvBuffer[BytesRead] = 0;  // 确保 null 结尾
                    FString ReceivedData = UTF8_TO_TCHAR(RecvBuffer.GetData());

                    // 按行分割（处理粘包）
                    TArray<FString> Lines;
                    ReceivedData.ParseIntoArray(Lines, TEXT("\n"), true);

                    for (const FString& Line : Lines)
                    {
                        if (!Line.IsEmpty())
                        {
                            FScopeLock Lock(&QueueMutex);

                            // 检查队列是否已满
                            if (MessageQueue.Num() >= MaxQueueSize)
                            {
                                MessageQueue.RemoveAt(0);
                                UE_LOG(LogTemp, Warning, TEXT("[Network] 消息队列已满，移除最旧消息"));
                            }

                            MessageQueue.Add(Line);
                        }
                    }

                    UE_LOG(LogTemp, Verbose, TEXT("[Network] 收到 %d 字节"), BytesRead);
                }
            }
            else
            {
                // 接收错误，可能是断开连接
                UE_LOG(LogTemp, Warning, TEXT("[Network] 接收数据失败"));
                break;
            }
        }

        // 避免 CPU 占用过高
        FPlatformProcess::Sleep(0.001f);  // 1ms
    }

    UE_LOG(LogTemp, Log, TEXT("[Network] 接收线程结束"));

    // 检测到断开连接，触发重连
    if (!bShouldStopThread && NetworkStatus == ENetworkStatus::Connected)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] 连接意外断开"));

        // 在游戏线程中处理（因为接收线程不能直接操作 UObject）
        // 注意：SessionToken 被保存到成员变量中供 BeginReconnect 使用
        PendingReconnectSessionToken = SessionToken;
        FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
            FSimpleDelegate::CreateUObject(this, &UNetworkManager::BeginReconnect),
            TStatId(), nullptr, ENamedThreads::GameThread
        );
    }
}

//==============================================================================
// 处理连接结果
//==============================================================================
void UNetworkManager::HandleConnectionResult(bool bSuccess)
{
    if (bSuccess)
    {
        OnStatusChanged.ExecuteIfBound();
    }
    else
    {
        OnReconnectCompleteCallback.ExecuteIfBound(false);
    }
}

//==============================================================================
// 开始重连流程
//==============================================================================
void UNetworkManager::BeginReconnect()
{
    if (bIsReconnecting)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] 已在重连中，忽略请求"));
        return;
    }

    SessionToken = PendingReconnectSessionToken;
    bIsReconnecting = true;
    ReconnectAttempts = 0;
    NetworkStatus = ENetworkStatus::Reconnecting;

    UE_LOG(LogTemp, Log, TEXT("[Network] 开始重连流程"));

    AttemptReconnect();
}

//==============================================================================
// 取消重连
//==============================================================================
void UNetworkManager::CancelReconnect()
{
    if (!bIsReconnecting)
    {
        return;
    }

    bIsReconnecting = false;
    ReconnectAttempts = 0;

    UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
    if (World)
    {
        World->GetTimerManager().ClearTimer(ReconnectTimerHandle);
    }

    UE_LOG(LogTemp, Log, TEXT("[Network] 重连已取消"));
}

//==============================================================================
// 尝试重连
//==============================================================================
void UNetworkManager::AttemptReconnect()
{
    if (!bIsReconnecting)
    {
        return;
    }

    if (ReconnectAttempts >= MaxReconnectAttempts)
    {
        UE_LOG(LogTemp, Error, TEXT("[Network] 达到最大重连次数（%d），放弃重连"), MaxReconnectAttempts);

        bIsReconnecting = false;
        NetworkStatus = ENetworkStatus::Error;
        LastError = TEXT("达到最大重连次数");

        OnReconnectCompleteCallback.ExecuteIfBound(false);
        return;
    }

    ReconnectAttempts++;

    // 指数退避算法
    float CurrentDelay = ReconnectDelaySeconds * FMath::Pow(2.0f, ReconnectAttempts - 1);
    CurrentDelay = FMath::Min(CurrentDelay, 30.0f);

    UE_LOG(LogTemp, Log, TEXT("[Network] 重连尝试 %d/%d，%.1f秒后执行"),
        ReconnectAttempts, MaxReconnectAttempts, CurrentDelay);

    // 清理旧连接
    CleanupSocket();

    // 获取世界定时器管理器
    UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
    if (World)
    {
        FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UNetworkManager::OnReconnectTimer);
        World->GetTimerManager().SetTimer(ReconnectTimerHandle, Delegate, CurrentDelay, false);
    }
    else
    {
        OnReconnectTimer();
    }
}

//==============================================================================
// 重连定时器触发
//==============================================================================
void UNetworkManager::OnReconnectTimer()
{
    if (!bIsReconnecting)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Network] 执行重连..."));

    NetworkStatus = ENetworkStatus::Connecting;

    bool bConnected = ConnectToServer(ServerIP, ServerPort);

    if (bConnected)
    {
        bIsReconnecting = false;
        ReconnectAttempts = 0;

        UE_LOG(LogTemp, Log, TEXT("[Network] 重连成功！"));

        OnReconnectCompleteCallback.ExecuteIfBound(true);
        OnStatusChanged.ExecuteIfBound();
    }
    else
    {
        LastError = TEXT("连接失败");

        UE_LOG(LogTemp, Warning, TEXT("[Network] 重连失败，继续重连..."));

        AttemptReconnect();
    }
}