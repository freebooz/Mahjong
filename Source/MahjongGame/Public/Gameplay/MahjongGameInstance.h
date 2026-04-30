#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UObject/NoExportTypes.h"
#include "Gameplay/Account/PlayerAccountService.h"
#include "Gameplay/Network/NetworkManager.h"
#include "MahjongGameInstance.generated.h"
UCLASS(Blueprintable)
class MAHJONGGAME_API UMahjongGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    UMahjongGameInstance();
    virtual void Init() override;
    virtual void Shutdown() override;
    virtual void OnStart() override;
    // Session management
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void CreateGameSession();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void JoinGameSession();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void FindAvailableSessions();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void DestroySession();
    // Player info
    UFUNCTION(BlueprintPure, Category="Mahjong|Player")
    FString GetPlayerName() const { return PlayerName; }
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void SetPlayerName(const FString& Name) { PlayerName = Name; }
    UFUNCTION(BlueprintPure, Category="Mahjong|Player")
    bool IsLoggedIn() const { return bIsLoggedIn; }
    UFUNCTION(BlueprintPure, Category="Mahjong|Player")
    UPlayerAccountService* GetPlayerAccountService() const { return PlayerAccountService; }
    UFUNCTION(BlueprintCallable, Category="Mahjong|Player")
    void SetPlayerAccountService(UPlayerAccountService* Service) { PlayerAccountService = Service; }
    UFUNCTION(BlueprintPure, Category="Mahjong|Network")
    UNetworkManager* GetNetworkManager() const { return NetworkManager; }
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void SetNetworkManager(UNetworkManager* Manager) { NetworkManager = Manager; }
    // Server connection
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void ConnectToServer(const FString& IP, int32 Port);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Network")
    void DisconnectFromServer();
protected:
    UPROPERTY(BlueprintReadWrite, Category="Mahjong|Player")
    FString PlayerName;
    UPROPERTY(BlueprintReadWrite, Category="Mahjong|Player")
    bool bIsLoggedIn;
    UPROPERTY()
    FString ServerIP;
    UPROPERTY()
    int32 ServerPort;
    UPROPERTY()
    UPlayerAccountService* PlayerAccountService;
    UPROPERTY()
    UNetworkManager* NetworkManager;
};