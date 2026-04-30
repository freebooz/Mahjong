#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/Monitor/MonitorManager.h"
#include "MahjongLobbyGameState.generated.h"
UCLASS()
class MAHJONGGAME_API AMahjongLobbyGameState : public AGameStateBase
{
    GENERATED_BODY()
public:
    AMahjongLobbyGameState();
    UFUNCTION(Server, Reliable) void SetPlayerReady(AController* Controller, bool bReady);
    UFUNCTION(Server, Reliable) void TryStartGame(AController* Controller);
    UFUNCTION(Server, Reliable) void Server_RefreshRoomList();
    UFUNCTION(Server, Reliable) void Server_JoinRoom(int32 RoomID);
    UFUNCTION(Client, Reliable) void Client_ReceiveRoomList(const TArray<FString>& RoomNames, const TArray<int32>& RoomIDs);
    UPROPERTY(Replicated, BlueprintReadOnly) int32 ReadyPlayerCount;
    UPROPERTY(Replicated, BlueprintReadOnly) int32 TotalPlayerCount;
    // 玩家登录/登出事件
    UFUNCTION(Server, Reliable) void Server_PlayerLogin(AController* Controller);
    UFUNCTION(Server, Reliable) void Server_PlayerLogout(AController* Controller);
protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
    void SetPlayerReady_Implementation(AController* Controller, bool bReady);
    bool SetPlayerReady_Validate(AController* Controller, bool bReady);
    void TryStartGame_Implementation(AController* Controller);
    bool TryStartGame_Validate(AController* Controller);
    // 内部方法
    void UpdateMonitorPlayerList();
    void NotifyPlayerLogin(AController* Controller);
    void NotifyPlayerLogout(AController* Controller);
};
