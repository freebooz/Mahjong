#include "Gameplay/MahjongGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Account/PlayerAccountService.h"
#include "Gameplay/Network/NetworkManager.h"
UMahjongGameInstance::UMahjongGameInstance()
{
    PlayerName = TEXT("Player");
    bIsLoggedIn = false;
    ServerPort = 7777;
    PlayerAccountService = nullptr;
    NetworkManager = nullptr;
}
void UMahjongGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Initialized"));
}
void UMahjongGameInstance::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Shutdown"));
    DisconnectFromServer();
    Super::Shutdown();
}
void UMahjongGameInstance::OnStart()
{
    Super::OnStart();
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Started"));
}
void UMahjongGameInstance::CreateGameSession()
{
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Creating game session"));
}
void UMahjongGameInstance::JoinGameSession()
{
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Joining game session"));
}
void UMahjongGameInstance::FindAvailableSessions()
{
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Finding available sessions"));
}
void UMahjongGameInstance::DestroySession()
{
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Destroying session"));
}
void UMahjongGameInstance::ConnectToServer(const FString& IP, int32 Port)
{
    ServerIP = IP;
    ServerPort = Port;
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Connecting to %s:%d"), *ServerIP, ServerPort);
}
void UMahjongGameInstance::DisconnectFromServer()
{
    UE_LOG(LogTemp, Log, TEXT("[GameInstance] Disconnecting from server"));
}