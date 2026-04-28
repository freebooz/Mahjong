#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/Lobby/MahjongLobbyPlayerController.h"
#include "MahjongLobbyGameMode.generated.h"

// 存储房间信息的结构体
USTRUCT()
struct FRoomInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString RoomID;

    UPROPERTY()
    FString RoomName;

    UPROPERTY()
    int32 MaxPlayers;

    UPROPERTY()
    TArray<AMahjongLobbyPlayerController*> Players;

    FRoomInfo() : RoomID(""), RoomName(""), MaxPlayers(0) {}
};

/**
 * 本类管理大厅模式，管理房间的创建、加入、列表等
 */
UCLASS()
class MAHJONGGAME_API AMahjongLobbyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMahjongLobbyGameMode();

    // 创建新房间
    UFUNCTION(BlueprintCallable, Category = "Lobby GameMode")
    FString CreateRoom(const FString& RoomName, int32 MaxPlayers, AMahjongLobbyPlayerController* Creator);

    // 加入指定房间
    UFUNCTION(BlueprintCallable, Category = "Lobby GameMode")
    bool JoinRoom(const FString& RoomID, AMahjongLobbyPlayerController* Player);

    // 获取房间列表
    UFUNCTION(BlueprintCallable, Category = "Lobby GameMode")
    void GetRoomList(TArray<FString>& OutRoomIDs, TArray<FString>& OutRoomNames, TArray<int32>& OutPlayerCounts);

protected:
    // 房间列表
    UPROPERTY()
    TMap<FString, FRoomInfo> Rooms;
};