#include "Gameplay/Lobby/MahjongRoomListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Gameplay/Lobby/MahjongLobbyHUD.h"
#include "Gameplay/Lobby/MahjongLobbyPlayerController.h"
#include "GameFramework/PlayerController.h"

// 构造函数
UMahjongRoomListEntry::UMahjongRoomListEntry(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    RoomID = -1;
    RoomName = TEXT("");
    bIsButtonBound = false;
}

void UMahjongRoomListEntry::NativeConstruct()
{
    Super::NativeConstruct();

    // 验证控件绑定
    ValidateWidgetBindings();

    // 绑定按钮点击事件
    if (JoinRoomButton && !bIsButtonBound)
    {
        JoinRoomButton->OnClicked.AddDynamic(this, &UMahjongRoomListEntry::OnJoinRoomButtonClicked);
        bIsButtonBound = true;
        UE_LOG(LogTemp, Log, TEXT("房间列表项: 加入房间按钮事件已绑定"));
    }
}

void UMahjongRoomListEntry::NativeDestruct()
{
    if (JoinRoomButton && bIsButtonBound)
    {
        JoinRoomButton->OnClicked.RemoveDynamic(this, &UMahjongRoomListEntry::OnJoinRoomButtonClicked);
        bIsButtonBound = false;
        UE_LOG(LogTemp, Log, TEXT("房间列表项: 加入房间按钮事件已解除绑定"));
    }

    Super::NativeDestruct();
}

void UMahjongRoomListEntry::SetupRoomEntry(const FString& InRoomName, int PlayerCount, int MaxPlayers, int InRoomID)
{
    RoomName = InRoomName;
    RoomID = InRoomID;

    if (RoomNameText)
    {
        RoomNameText->SetText(FText::FromString(RoomName));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("房间列表项: RoomNameText 未绑定"));
    }

    if (RoomIDText)
    {
        RoomIDText->SetText(FText::AsNumber(RoomID));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("房间列表项: RoomIDText 未绑定"));
    }

    if (PlayerCountText)
    {
        FString PlayerCountStr = FString::Printf(TEXT("%d/%d"), PlayerCount, MaxPlayers);
        PlayerCountText->SetText(FText::FromString(PlayerCountStr));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("房间列表项: PlayerCountText 未绑定"));
    }

    if (JoinRoomButton)
    {
        JoinRoomButton->SetIsEnabled(true);
    }

    UE_LOG(LogTemp, Log, TEXT("房间列表项: 已设置房间信息 - 名称: %s, ID: %d, 玩家: %d/%d"), *RoomName, RoomID, PlayerCount, MaxPlayers);
}

void UMahjongRoomListEntry::OnJoinRoomButtonClicked()
{
    APlayerController* PlayerController = GetOwningPlayer(); // UUserWidget 提供的函数
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("房间列表项: 无法获取 PlayerController"));
        return;
    }

    // 优先使用 MahjongLobbyPlayerController
    AMahjongLobbyPlayerController* LobbyPlayerController = Cast<AMahjongLobbyPlayerController>(PlayerController);
    if (LobbyPlayerController)
    {
        FString RoomIDStr = FString::FromInt(RoomID);
        LobbyPlayerController->ClientRequestJoinRoom(RoomIDStr);
        UE_LOG(LogTemp, Log, TEXT("房间列表项: 通过 PlayerController 请求加入房间 %s"), *RoomIDStr);
    }
    else
    {
        // 回退到 MahjongLobbyHUD
        AMahjongLobbyHUD* LobbyHUD = Cast<AMahjongLobbyHUD>(PlayerController->GetHUD());
        if (LobbyHUD)
        {
            LobbyHUD->ClientJoinRoom(RoomID);
            UE_LOG(LogTemp, Log, TEXT("房间列表项: 通过 HUD 请求加入房间 %d"), RoomID);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("房间列表项: 无法获取 MahjongLobbyPlayerController 或 MahjongLobbyHUD"));
        }
    }

    if (JoinRoomButton)
    {
        JoinRoomButton->SetIsEnabled(false);
    }
}

void UMahjongRoomListEntry::ValidateWidgetBindings()
{
    if (!RoomNameText)
    {
        UE_LOG(LogTemp, Error, TEXT("房间列表项: RoomNameText 未在蓝图中绑定"));
    }
    if (!RoomIDText)
    {
        UE_LOG(LogTemp, Error, TEXT("房间列表项: RoomIDText 未在蓝图中绑定"));
    }
    if (!PlayerCountText)
    {
        UE_LOG(LogTemp, Error, TEXT("房间列表项: PlayerCountText 未在蓝图中绑定"));
    }
    if (!JoinRoomButton)
    {
        UE_LOG(LogTemp, Error, TEXT("房间列表项: JoinRoomButton 未在蓝图中绑定"));
    }
}