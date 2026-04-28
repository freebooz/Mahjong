#include "Gameplay/Lobby/MahjongLobbyHUD.h"
#include "Gameplay/Lobby/MahjongLobbyWidget.h" // 假设这是你的Widget类的头文件
#include "Net/UnrealNetwork.h"

AMahjongLobbyHUD::AMahjongLobbyHUD()
{
    // 默认构造函数
}

void AMahjongLobbyHUD::BeginPlay()
{
    Super::BeginPlay();

    if (IsClient())
    {
        InitializeLobbyUI();
    }
}

void AMahjongLobbyHUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMahjongLobbyHUD::ShowLobbyMainUI()
{
    if (IsClient() && LobbyWidgetInstance)
    {
        LobbyWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMahjongLobbyHUD::RefreshRoomList()
{
    if (IsClient())
    {
        if (AMahjongLobbyPlayerController* PC = Cast<AMahjongLobbyPlayerController>(GetOwningPlayerController()))
        {
            PC->ClientRequestRefreshRoomList();
        }
    }
}

void AMahjongLobbyHUD::ShowCreateRoomPopup()
{
    if (IsClient() && LobbyWidgetInstance)
    {
        // 假设 LobbyWidgetInstance 有个 ShowCreateRoomPopup 方法
        // 你需要在 UMahjongLobbyWidget 中实现这个功能
    }
}

void AMahjongLobbyHUD::ClientJoinRoom(int32 RoomID)
{
    if (IsClient())
    {
        if (AMahjongLobbyPlayerController* PC = Cast<AMahjongLobbyPlayerController>(GetOwningPlayerController()))
        {
            PC->ClientRequestJoinRoom(FString::FromInt(RoomID));
        }
    }
}

void AMahjongLobbyHUD::RequestCreateRoom(const FString& RoomName, int32 MaxPlayers)
{
    if (IsClient())
    {
        //ServerCreateRoom(RoomName, MaxPlayers);
    }
}

//void AMahjongLobbyHUD::ServerCreateRoom_Validate(const FString& RoomName, int32 MaxPlayers)
//{
//    return !RoomName.IsEmpty() && MaxPlayers > 0;
//}
//
//void AMahjongLobbyHUD::ServerCreateRoom_Implementation(const FString& RoomName, int32 MaxPlayers)
//{
//    if (IsServer())
//    {
//        if (AMahjongLobbyPlayerController* PC = Cast<AMahjongLobbyPlayerController>(GetOwningPlayerController()))
//        {
//            PC->ServerCreateRoom(RoomName, MaxPlayers);
//        }
//    }
//}
//
//bool AMahjongLobbyHUD::ServerJoinRoom_Validate(int32 RoomID)
//{
//    return RoomID >= 0;
//}
//
//void AMahjongLobbyHUD::ServerJoinRoom_Implementation(int32 RoomID)
//{
//    if (IsServer())
//    {
//        if (AMahjongLobbyPlayerController* PC = Cast<AMahjongLobbyPlayerController>(GetOwningPlayerController()))
//        {
//            PC->ServerJoinRoom(FString::FromInt(RoomID));
//        }
//    }
//}
//
//void AMahjongLobbyHUD::MulticastUpdateRoomList_Implementation(const TArray<FString>& RoomNames, const TArray<int32>& RoomIDs)
//{
//    if (IsClient() && LobbyWidgetInstance)
//    {
//        // 假设 LobbyWidgetInstance 有个 UpdateRoomList 方法
//        // 你需要在 UMahjongLobbyWidget 中实现这个功能
//    }
//}
//
//void AMahjongLobbyHUD::ClientOnRoomJoined_Implementation(int32 RoomID)
//{
//    if (IsClient())
//    {
//        UE_LOG(LogTemp, Log, TEXT("成功加入房间：%d"), RoomID);
//        // 这里可以添加加入房间后的UI更新逻辑
//    }
//}

void AMahjongLobbyHUD::InitializeLobbyUI()
{
    if (LobbyWidgetClass && !LobbyWidgetInstance)
    {
        LobbyWidgetInstance = CreateWidget<UMahjongLobbyWidget>(GetWorld(), LobbyWidgetClass);
        if (LobbyWidgetInstance)
        {
            LobbyWidgetInstance->AddToViewport();
            LobbyWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

//void AMahjongLobbyHUD::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}