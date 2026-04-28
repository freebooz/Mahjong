//==============================================================================
// MahjongGame - UI GameMode Base Class Implementation
// UIGameModeBase.cpp
//==============================================================================

#include "Gameplay/Modes/UIGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void AUIGameModeBase::ShowUIWidget(
    TSubclassOf<UUserWidget> WidgetClass,
    UUserWidget*& WidgetInstance,
    const FString& LogPrefix)
{
    if (WidgetInstance)
    {
        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;
    }

    if (WidgetClass)
    {
        WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
        if (WidgetInstance)
        {
            WidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("[%s] UI widget shown"), *LogPrefix);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to create widget"), *LogPrefix);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Widget class is null"), *LogPrefix);
    }
}

void AUIGameModeBase::HideUIWidget(UUserWidget* WidgetInstance, const FString& LogPrefix)
{
    if (WidgetInstance)
    {
        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;
        UE_LOG(LogTemp, Log, TEXT("[%s] UI widget hidden"), *LogPrefix);
    }
}

void AUIGameModeBase::TransitionToLevel(const FString& LevelName, bool bIsListen)
{
    UE_LOG(LogTemp, Log, TEXT("[AUIGameModeBase] Transitioning to level: %s"), *LevelName);
    FString Options = bIsListen ? TEXT("listen") : TEXT("");
    UGameplayStatics::OpenLevel(this, FName(*LevelName), bIsListen, Options);
}
