#include "Gameplay/UI/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Core/MahjongGameInstance.h"
#include "Kismet/GameplayStatics.h"
ULobbyWidget::ULobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Constructed"));
    if (CreateRoomButton)
        CreateRoomButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnCreateRoomButtonClicked);
    if (JoinRoomButton)
        JoinRoomButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnJoinRoomButtonClicked);
    if (RefreshButton)
        RefreshButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnRefreshButtonClicked);
    if (LogoutButton)
        LogoutButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnLogoutButtonClicked);
    if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        if (PlayerNameText)
            PlayerNameText->SetText(FText::FromString(GI->GetPlayerName()));
        if (PlayerCoinsText)
            PlayerCoinsText->SetText(FText::FromString(TEXT("1000")));
    }
    RefreshRoomList();
}
void ULobbyWidget::OnCreateRoomButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Create room clicked"));
}
void ULobbyWidget::OnJoinRoomButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Join room clicked"));
}
void ULobbyWidget::OnRefreshButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Refresh clicked"));
    RefreshRoomList();
}
void ULobbyWidget::OnLogoutButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Logout clicked"));
}
void ULobbyWidget::RefreshRoomList()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Refreshing room list"));
    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("4 rooms available")));
}
void ULobbyWidget::OnRoomSelected(int32 RoomID)
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Room selected: %d"), RoomID);
}