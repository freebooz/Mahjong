#include "Network/NetworkManager.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "HAL/RunnableThread.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UNetworkManager::UNetworkManager()
{
    NetworkStatus = ENetworkStatus::Disconnected;
    ServerPort = 7777;
    bIsReconnecting = false;
    ReconnectAttempts = 0;
    MaxReconnectAttempts = 5;
    ReconnectDelaySeconds = 1.0f;
    TimerManager = nullptr;
}

bool UNetworkManager::ConnectToServer(const FString& IP, int32 Port)
{
    ServerIP = IP;
    ServerPort = Port;
    UE_LOG(LogTemp, Log, TEXT("[Network] Connecting to %s:%d"), *ServerIP, ServerPort);
    NetworkStatus = ENetworkStatus::Connecting;
    return true;
}

void UNetworkManager::Disconnect()
{
    UE_LOG(LogTemp, Log, TEXT("[Network] Disconnecting"));
    CancelReconnect();
    NetworkStatus = ENetworkStatus::Disconnected;
}

bool UNetworkManager::SendMessage(const FString& Message)
{
    if (NetworkStatus != ENetworkStatus::Connected)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] Cannot send: not connected"));
        return false;
    }

    {
        FScopeLock Lock(&QueueMutex);
        UE_LOG(LogTemp, Log, TEXT("[Network] Sending: %s"), *Message);
    }
    return true;
}

FString UNetworkManager::ReceiveMessage()
{
    FString Msg;
    {
        FScopeLock Lock(&QueueMutex);
        if (MessageQueue.Num() > 0)
        {
            Msg = MessageQueue[0];
            MessageQueue.RemoveAt(0);
        }
    }
    return Msg;
}

void UNetworkManager::BeginReconnect(const FString& InSessionToken)
{
    if (bIsReconnecting)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] Already reconnecting"));
        return;
    }

    SessionToken = InSessionToken;
    bIsReconnecting = true;
    ReconnectAttempts = 0;
    NetworkStatus = ENetworkStatus::Reconnecting;

    UE_LOG(LogTemp, Log, TEXT("[Network] Starting reconnection with session token"));
    AttemptReconnect();
}

void UNetworkManager::CancelReconnect()
{
    if (!bIsReconnecting)
    {
        return;
    }

    bIsReconnecting = false;
    ReconnectAttempts = 0;

    if (TimerManager)
    {
        TimerManager->ClearTimer(ReconnectTimerHandle);
    }

    UE_LOG(LogTemp, Log, TEXT("[Network] Reconnection cancelled"));
}

void UNetworkManager::AttemptReconnect()
{
    if (!bIsReconnecting || ReconnectAttempts >= MaxReconnectAttempts)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Network] Reconnection failed or max attempts reached"));
        bIsReconnecting = false;
        NetworkStatus = ENetworkStatus::Error;
        LastError = TEXT("Max reconnection attempts reached");
        OnReconnectCompleteCallback.ExecuteIfBound(false);
        return;
    }

    ReconnectAttempts++;
    float CurrentDelay = ReconnectDelaySeconds * FMath::Pow(2.0f, ReconnectAttempts - 1);
    CurrentDelay = FMath::Min(CurrentDelay, 30.0f);  // 最大30秒延迟，避免过长等待

    UE_LOG(LogTemp, Log, TEXT("[Network] Reconnection attempt %d/%d in %.1fs"),
        ReconnectAttempts, MaxReconnectAttempts, CurrentDelay);

    UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
    if (World)
    {
        TimerManager = &World->GetTimerManager();
        FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UNetworkManager::OnReconnectTimer);
        TimerManager->SetTimer(ReconnectTimerHandle, Delegate, CurrentDelay, false);
    }
    else
    {
        OnReconnectTimer();
    }
}

void UNetworkManager::OnReconnectTimer()
{
    if (!bIsReconnecting)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Network] Attempting reconnection..."));

    NetworkStatus = ENetworkStatus::Connecting;

    bool bConnected = ConnectToServer(ServerIP, ServerPort);
    if (bConnected)
    {
        bIsReconnecting = false;
        ReconnectAttempts = 0;
        NetworkStatus = ENetworkStatus::Connected;
        UE_LOG(LogTemp, Log, TEXT("[Network] Reconnection successful"));
        OnReconnectCompleteCallback.ExecuteIfBound(true);
        OnStatusChanged.ExecuteIfBound();
    }
    else
    {
        LastError = TEXT("Connection failed");
        AttemptReconnect();
    }
}