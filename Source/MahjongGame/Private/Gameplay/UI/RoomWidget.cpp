#include "Gameplay/UI/RoomWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Core/MahjongGameInstance.h"
#include "Kismet/GameplayStatics.h"
URoomWidget::URoomWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
void URoomWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("[RoomWidget] Constructed"));
    if (LeaveButton)
        LeaveButton->OnClicked.AddDynamic(this, &URoomWidget::OnLeaveButtonClicked);
    if (StartGameButton)
        StartGameButton->OnClicked.AddDynamic(this, &URoomWidget::OnStartGameButtonClicked);
    if (DrawTileButton)
        DrawTileButton->OnClicked.AddDynamic(this, &URoomWidget::OnDrawTileButtonClicked);
    if (DiscardButton)
        DiscardButton->OnClicked.AddDynamic(this, &URoomWidget::OnDiscardTileButtonClicked);
    if (RoomIDText)
        RoomIDText->SetText(FText::FromString(TEXT("Room #1234")));
    if (PlayerCountText)
        PlayerCountText->SetText(FText::FromString(TEXT("0/4 Players")));
    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("Waiting for players...")));
}
void URoomWidget::OnLeaveButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidget] Leave room clicked"));
}
void URoomWidget::OnStartGameButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidget] Start game clicked"));
}
void URoomWidget::OnDrawTileButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidget] Draw tile clicked"));
}
void URoomWidget::OnDiscardTileButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[RoomWidget] Discard tile clicked"));
}
void URoomWidget::UpdateGameState(const FString& State)
{
    if (StatusText)
        StatusText->SetText(FText::FromString(State));
}