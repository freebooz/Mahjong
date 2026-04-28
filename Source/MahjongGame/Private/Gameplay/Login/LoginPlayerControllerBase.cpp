#include "Gameplay/Login/LoginPlayerControllerBase.h"
#include "Gameplay/UI/LoginWidgetBase.h"
#include "Core/MahjongGameInstance.h"
#include "Kismet/GameplayStatics.h"
ALoginPlayerControllerBase::ALoginPlayerControllerBase()
{
    bIsLoggedIn = false;
}
void ALoginPlayerControllerBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[LoginPlayerControllerBase] BeginPlay"));
}
void ALoginPlayerControllerBase::SendLoginRequest(const FString& Username, const FString& Password)
{
    UE_LOG(LogTemp, Log, TEXT("[LoginPlayerControllerBase] Login request: %s"), *Username);
    if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->SetPlayerName(Username);
        bIsLoggedIn = true;
        CurrentPlayerName = Username;
        OnLoginSuccess(Username);
    }
}
void ALoginPlayerControllerBase::SendRegisterRequest(const FString& Username, const FString& Password, const FString& Email)
{
    UE_LOG(LogTemp, Log, TEXT("[LoginPlayerControllerBase] Register request: %s, %s"), *Username, *Email);
    SendLoginRequest(Username, Password);
}
void ALoginPlayerControllerBase::OnLoginSuccess(const FString& PlayerName)
{
    UE_LOG(LogTemp, Log, TEXT("[LoginPlayerControllerBase] Login success: %s"), *PlayerName);
}
void ALoginPlayerControllerBase::OnLoginFailed(const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Warning, TEXT("[LoginPlayerControllerBase] Login failed: %s"), *ErrorMessage);
}