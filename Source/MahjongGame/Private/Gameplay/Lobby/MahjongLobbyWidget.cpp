#include "Gameplay/Lobby/MahjongLobbyWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Gameplay/Lobby/MahjongRoomListEntry.h"
#include "Gameplay/Lobby/MahjongLobbyHUD.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"

// 构造函数
UMahjongLobbyWidget::UMahjongLobbyWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    RoomEntries.Empty();
}

void UMahjongLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreateRoomButton)
    {
        CreateRoomButton->OnClicked.AddDynamic(this, &UMahjongLobbyWidget::OnCreateRoomButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("大厅Widget: CreateRoomButton 未绑定"));
    }

    if (JoinRoomButton)
    {
        JoinRoomButton->OnClicked.AddDynamic(this, &UMahjongLobbyWidget::OnJoinRoomButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("大厅Widget: JoinRoomButton 未绑定"));
    }

    RefreshRoomList();
}

void UMahjongLobbyWidget::OnCreateRoomButtonClicked()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (!PlayerController) return;

    AMahjongLobbyHUD* LobbyHUD = Cast<AMahjongLobbyHUD>(PlayerController->GetHUD());
    if (LobbyHUD)
    {
        LobbyHUD->RequestCreateRoom(TEXT("新房间"), 4);
        UE_LOG(LogTemp, Log, TEXT("大厅Widget: 请求创建新房间"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("大厅Widget: 无法获取 MahjongLobbyHUD"));
    }
}

void UMahjongLobbyWidget::OnJoinRoomButtonClicked()
{
    if (!RoomListView)
    {
        UE_LOG(LogTemp, Warning, TEXT("大厅Widget: RoomListView 未绑定"));
        return;
    }

    UObject* SelectedItem = RoomListView->GetSelectedItem();
    if (SelectedItem)
    {
        UMahjongRoomListEntry* SelectedRoom = Cast<UMahjongRoomListEntry>(SelectedItem);
        if (SelectedRoom)
        {
            APlayerController* PlayerController = GetOwningPlayer();
            if (PlayerController)
            {
                AMahjongLobbyHUD* LobbyHUD = Cast<AMahjongLobbyHUD>(PlayerController->GetHUD());
                if (LobbyHUD)
                {
                    LobbyHUD->ClientJoinRoom(SelectedRoom->RoomID);
                    UE_LOG(LogTemp, Log, TEXT("大厅Widget: 请求加入房间 ID: %d"), SelectedRoom->RoomID);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("大厅Widget: 未选中任何房间"));
    }
}

void UMahjongLobbyWidget::RefreshRoomList()
{
    if (!RoomListView)
    {
        UE_LOG(LogTemp, Warning, TEXT("大厅Widget: RoomListView 未绑定"));
        return;
    }

    RoomListView->ClearListItems();
    RoomEntries.Empty();

    for (int32 i = 0; i < 5; ++i)
    {
        UMahjongRoomListEntry* RoomEntry = NewObject<UMahjongRoomListEntry>(this);
        RoomEntry->SetupRoomEntry(FString::Printf(TEXT("Room %d"), i + 1), i + 1, 1, 4);
        RoomEntries.Add(RoomEntry);
        RoomListView->AddItem(RoomEntry);
    }

    UE_LOG(LogTemp, Log, TEXT("大厅Widget: 刷新房间列表，数量: %d"), RoomEntries.Num());
}

void UMahjongLobbyWidget::ShowCreateRoomPopup()
{
    UE_LOG(LogTemp, Log, TEXT("大厅Widget: 显示创建房间弹窗"));
    OnCreateRoomButtonClicked();
}