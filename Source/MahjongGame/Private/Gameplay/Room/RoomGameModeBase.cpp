//==============================================================================
// RoomGameModeBase.cpp
//==============================================================================

#include "Gameplay/Room/RoomGameModeBase.h"
#include "Gameplay/UI/RoomWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MahjongGameInstance.h"

ARoomGameModeBase::ARoomGameModeBase()
{
    RoomWidgetClass = nullptr;
    RoomWidgetInstance = nullptr;
    RoomID = 0;
    CurrentPlayerCount = 0;
    MaxPlayerCount = 4;
}

void ARoomGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] BeginPlay"));
    ShowRoomUI();
}

void ARoomGameModeBase::ShowRoomUI()
{
    UUserWidget* WidgetPtr = RoomWidgetInstance;
    ShowUIWidget(RoomWidgetClass, WidgetPtr, TEXT("RoomGameModeBase"));
    RoomWidgetInstance = Cast<URoomWidgetBase>(WidgetPtr);
}

void ARoomGameModeBase::HideRoomUI()
{
    UUserWidget* WidgetPtr = RoomWidgetInstance;
    HideUIWidget(WidgetPtr, TEXT("RoomGameModeBase"));
}

void ARoomGameModeBase::UpdatePlayerCount(int32 Current, int32 Max)
{
    CurrentPlayerCount = Current;
    MaxPlayerCount = Max;
    if (RoomWidgetInstance)
        RoomWidgetInstance->UpdateRoomInfo(RoomID, CurrentPlayerCount, MaxPlayerCount);
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] Players: %d/%d"), CurrentPlayerCount, MaxPlayerCount);
}

void ARoomGameModeBase::StartGame()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] Starting game"));
    if (RoomWidgetInstance)
        RoomWidgetInstance->SetGameStatus(TEXT("游戏开始!"));
}

void ARoomGameModeBase::OnPlayerReady(const FString& PlayerName)
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] Player ready: %s"), *PlayerName);
}

void ARoomGameModeBase::LeaveRoom()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] Leaving room"));
    HideRoomUI();
    TransitionToLevel(TEXT("Lobby"), true);
}

void ARoomGameModeBase::OnTileDrawn(int32 PlayerIndex)
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] Player %d drew a tile"), PlayerIndex);
}

void ARoomGameModeBase::OnTileDiscarded(int32 PlayerIndex, int32 TileID)
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] Player %d discarded tile %d"), PlayerIndex, TileID);
}

void ARoomGameModeBase::SetRoomRulesConfig(const FRoomGameRulesConfig& Config)
{
    RoomRulesConfig = Config;
    UE_LOG(LogTemp, Log, TEXT("[RoomGameModeBase] Room rules updated: %s"), *RoomRulesConfig.GetRulesDescription());
}
