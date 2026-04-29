#include "Gameplay/Room/RoomGameMode.h"
#include "Gameplay/UI/RoomWidget.h"
#include "Gameplay/MahjongGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
ARoomGameMode::ARoomGameMode()
{
    RoomWidgetClass = nullptr;
    RoomWidgetInstance = nullptr;
    RoomID = 0;
}
void ARoomGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[RoomGameMode] BeginPlay"));
    ShowRoomUI();
}
void ARoomGameMode::ShowRoomUI()
{
    if (RoomWidgetInstance)
    {
        RoomWidgetInstance->RemoveFromParent();
        RoomWidgetInstance = nullptr;
    }
    if (RoomWidgetClass)
    {
        RoomWidgetInstance = CreateWidget<URoomWidget>(GetWorld(), RoomWidgetClass);
        if (RoomWidgetInstance)
        {
            RoomWidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("[RoomGameMode] Room UI shown"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[RoomGameMode] RoomWidgetClass is null"));
    }
}
void ARoomGameMode::HideRoomUI()
{
    if (RoomWidgetInstance)
    {
        RoomWidgetInstance->RemoveFromParent();
        RoomWidgetInstance = nullptr;
    }
}
void ARoomGameMode::UpdatePlayerCount(int32 Current, int32 Max)
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameMode] Player count: %d/%d"), Current, Max);
}
void ARoomGameMode::StartGame()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameMode] Starting game..."));
}
void ARoomGameMode::OnPlayerJoined(const FString& PlayerName)
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameMode] Player joined: %s"), *PlayerName);
}
void ARoomGameMode::OnPlayerLeft(const FString& PlayerName)
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameMode] Player left: %s"), *PlayerName);
}
void ARoomGameMode::LeaveRoom()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomGameMode] Leaving room..."));
    HideRoomUI();
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}