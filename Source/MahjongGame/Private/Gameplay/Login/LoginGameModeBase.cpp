#include "Gameplay/Login/LoginGameModeBase.h"
#include "Gameplay/UI/LoginWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/MahjongGameInstance.h"

ALoginGameModeBase::ALoginGameModeBase()
{
    LoginWidgetClass = nullptr;
    LoginWidgetInstance = nullptr;
}

void ALoginGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[LoginGameModeBase] BeginPlay"));
    ShowLoginUI();
}

void ALoginGameModeBase::ShowLoginUI()
{
    UUserWidget* WidgetPtr = LoginWidgetInstance;
    ShowUIWidget(LoginWidgetClass, WidgetPtr, TEXT("LoginGameModeBase"));
    LoginWidgetInstance = Cast<ULoginWidgetBase>(WidgetPtr);
}

void ALoginGameModeBase::HideLoginUI()
{
    UUserWidget* WidgetPtr = LoginWidgetInstance;
    HideUIWidget(WidgetPtr, TEXT("LoginGameModeBase"));
}

void ALoginGameModeBase::OnLoginSuccess(const FString& PlayerName)
{
    UE_LOG(LogTemp, Log, TEXT("[LoginGameModeBase] Login success: %s"), *PlayerName);
    HideLoginUI();
    TransitionToLobby();
}

void ALoginGameModeBase::OnLoginFailed(const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Warning, TEXT("[LoginGameModeBase] Login failed: %s"), *ErrorMessage);
}

void ALoginGameModeBase::TransitionToLobby()
{
    TransitionToLevel(TEXT("Lobby"), true);
}