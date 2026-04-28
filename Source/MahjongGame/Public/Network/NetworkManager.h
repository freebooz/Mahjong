#pragma once
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "UObject/NoExportTypes.h"
#include "NetworkManager.generated.h"
class USocket;
class UNetConnection;

UENUM(BlueprintType)
enum class ENetworkStatus : uint8
{
    Disconnected,
    Connecting,
    Connected,
    Reconnecting,
    Error
};

DECLARE_DYNAMIC_DELEGATE(FOnNetworkStatusChanged);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnReconnectComplete, bool, bSuccess);

UCLASS(Blueprintable)
class MAHJONGGAME_API UNetworkManager : public UObject
{
    GENERATED_BODY()
public:
    UNetworkManager();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    bool ConnectToServer(const FString& IP, int32 Port);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void Disconnect();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    bool SendMessage(const FString& Message);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    FString ReceiveMessage();

    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void BeginReconnect(const FString& SessionToken);

    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void CancelReconnect();

    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    ENetworkStatus GetStatus() const { return NetworkStatus; }

    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    FString GetLastError() const { return LastError; }

    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    bool IsReconnecting() const { return bIsReconnecting; }

    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    int32 GetReconnectAttempts() const { return ReconnectAttempts; }

    void SetOnStatusChanged(FOnNetworkStatusChanged Callback) { OnStatusChanged = Callback; }
    void SetOnReconnectComplete(FOnReconnectComplete Callback) { OnReconnectCompleteCallback = Callback; }

protected:
    void AttemptReconnect();
    void OnReconnectTimer();

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

    TArray<FString> MessageQueue;

    FCriticalSection QueueMutex;

    bool bIsReconnecting;
    int32 ReconnectAttempts;
    int32 MaxReconnectAttempts;
    float ReconnectDelaySeconds;
    FTimerHandle ReconnectTimerHandle;
    FTimerManager* TimerManager;

    FOnNetworkStatusChanged OnStatusChanged;
    FOnReconnectComplete OnReconnectCompleteCallback;
};