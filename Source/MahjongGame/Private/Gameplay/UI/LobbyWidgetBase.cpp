#include "Gameplay/UI/LobbyWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MahjongGameInstance.h"
ULobbyWidgetBase::ULobbyWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
void ULobbyWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    if (CreateRoomButton)
        CreateRoomButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OnCreateRoomClicked);
    if (RefreshButton)
        RefreshButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OnRefreshClicked);
    if (LogoutButton)
        LogoutButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::OnLogoutClicked);
    RefreshRoomList();
    if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        if (PlayerNameText)
            PlayerNameText->SetText(FText::FromString(GI->GetPlayerName()));
        if (CoinsText)
            CoinsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), 1000)));
    }
}
void ULobbyWidgetBase::OnCreateRoomClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidgetBase] Create room clicked"));
    UGameplayStatics::OpenLevel(this, TEXT("Room"), true, TEXT("listen"));
}
void ULobbyWidgetBase::OnJoinRoomClicked(int32 RoomID)
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidgetBase] Join room: %d"), RoomID);
    UGameplayStatics::OpenLevel(this, TEXT("Room"), true, TEXT("listen"));
}
void ULobbyWidgetBase::OnRefreshClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidgetBase] Refresh clicked"));
    RefreshRoomList();
}
void ULobbyWidgetBase::OnLogoutClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidgetBase] Logout clicked"));
    UGameplayStatics::OpenLevel(this, TEXT("Login"), true, TEXT("listen"));
}
void ULobbyWidgetBase::RefreshRoomList()
{
    ClearRoomList();
    FLobbyRoomInfo Room1;
    Room1.RoomID = 1;
    Room1.RoomName = TEXT("房间1");
    Room1.CurrentPlayers = 2;
    Room1.MaxPlayers = 4;
    Room1.bIsJoinable = true;
    AddRoomToList(Room1);
    FLobbyRoomInfo Room2;
    Room2.RoomID = 2;
    Room2.RoomName = TEXT("房间2");
    Room2.CurrentPlayers = 3;
    Room2.MaxPlayers = 4;
    Room2.bIsJoinable = true;
    AddRoomToList(Room2);
    if (StatusText)
        StatusText->SetText(FText::FromString(FString::Printf(TEXT("%d 个房间可用"), RoomInfos.Num())));
}
void ULobbyWidgetBase::AddRoomToList(const FLobbyRoomInfo& RoomInfo)
{
    RoomInfos.Add(RoomInfo);
}
void ULobbyWidgetBase::ClearRoomList()
{
    RoomInfos.Reset();
}
void ULobbyWidgetBase::SetPlayerInfo(const FString& PlayerName, int32 Coins)
{
    if (PlayerNameText)
        PlayerNameText->SetText(FText::FromString(PlayerName));
    if (CoinsText)
        CoinsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Coins)));
}
FString ULobbyWidgetBase::GetSelectedRoomName() const
{
    return TEXT("");
}