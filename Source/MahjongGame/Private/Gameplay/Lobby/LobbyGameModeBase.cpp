#include "Gameplay/Lobby/LobbyGameModeBase.h"
#include "Gameplay/UI/LobbyWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MahjongGameInstance.h"
ALobbyGameModeBase::ALobbyGameModeBase()
{
    LobbyWidgetClass = nullptr;
    LobbyWidgetInstance = nullptr;
    ConnectedPlayerCount = 0;
}
void ALobbyGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[LobbyGameModeBase] BeginPlay"));
    ShowLobbyUI();
}
void ALobbyGameModeBase::ShowLobbyUI()
{
    if (LobbyWidgetInstance)
    {
        LobbyWidgetInstance->RemoveFromParent();
        LobbyWidgetInstance = nullptr;
    }
    if (LobbyWidgetClass)
    {
        LobbyWidgetInstance = CreateWidget<ULobbyWidgetBase>(GetWorld(), LobbyWidgetClass);
        if (LobbyWidgetInstance)
        {
            LobbyWidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("[LobbyGameModeBase] Lobby UI shown"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[LobbyGameModeBase] LobbyWidgetClass is null"));
    }
}
void ALobbyGameModeBase::HideLobbyUI()
{
    if (LobbyWidgetInstance)
    {
        LobbyWidgetInstance->RemoveFromParent();
        LobbyWidgetInstance = nullptr;
    }
}
void ALobbyGameModeBase::OnPlayerJoined(const FString& PlayerName)
{
    ConnectedPlayerCount++;
    UE_LOG(LogTemp, Log, TEXT("[LobbyGameModeBase] Player joined: %s (%d players)"), *PlayerName, ConnectedPlayerCount);
}
void ALobbyGameModeBase::OnPlayerLeft(const FString& PlayerName)
{
    ConnectedPlayerCount--;
    UE_LOG(LogTemp, Log, TEXT("[LobbyGameModeBase] Player left: %s (%d players)"), *PlayerName, ConnectedPlayerCount);
}
void ALobbyGameModeBase::CreateNewRoom()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyGameModeBase] Creating new room"));
    HideLobbyUI();
    UGameplayStatics::OpenLevel(this, TEXT("Room"), true, TEXT("listen"));
}
void ALobbyGameModeBase::JoinRoom(int32 RoomID)
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyGameModeBase] Joining room: %d"), RoomID);
    HideLobbyUI();
    UGameplayStatics::OpenLevel(this, TEXT("Room"), true, TEXT("listen"));
}
void ALobbyGameModeBase::LeaveLobby()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyGameModeBase] Leaving lobby"));
    HideLobbyUI();
    UGameplayStatics::OpenLevel(this, TEXT("Login"), true, TEXT("listen"));
}