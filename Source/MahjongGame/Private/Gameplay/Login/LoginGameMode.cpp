#include "Gameplay/Login/LoginGameMode.h"
#include "Gameplay/UI/LoginWidget.h"
#include "Core/MahjongGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
ALoginGameMode::ALoginGameMode()
{
    LoginWidgetClass = nullptr;
    LoginWidgetInstance = nullptr;
}
void ALoginGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("[LoginGameMode] BeginPlay"));
    ShowLoginUI();
}
void ALoginGameMode::ShowLoginUI()
{
    if (LoginWidgetInstance)
    {
        LoginWidgetInstance->RemoveFromParent();
        LoginWidgetInstance = nullptr;
    }
    if (LoginWidgetClass)
    {
        LoginWidgetInstance = CreateWidget<ULoginWidget>(GetWorld(), LoginWidgetClass);
        if (LoginWidgetInstance)
        {
            LoginWidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("[LoginGameMode] Login UI shown"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[LoginGameMode] LoginWidgetClass is null, cannot create widget"));
    }
}
void ALoginGameMode::HideLoginUI()
{
    if (LoginWidgetInstance)
    {
        LoginWidgetInstance->RemoveFromParent();
        LoginWidgetInstance = nullptr;
        UE_LOG(LogTemp, Log, TEXT("[LoginGameMode] Login UI hidden"));
    }
}
void ALoginGameMode::OnLoginSuccess(const FString& PlayerName)
{
    UE_LOG(LogTemp, Log, TEXT("[LoginGameMode] Login success: %s"), *PlayerName);
    HideLoginUI();
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}
void ALoginGameMode::OnLoginFailed(const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Warning, TEXT("[LoginGameMode] Login failed: %s"), *ErrorMessage);
}