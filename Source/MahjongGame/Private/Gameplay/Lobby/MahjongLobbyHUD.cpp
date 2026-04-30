#include "Gameplay/Lobby/MahjongLobbyHUD.h"
#include "Gameplay/Lobby/MahjongLobbyWidget.h"
#include "Net/UnrealNetwork.h"

// ==================== 构造函数 ====================

AMahjongLobbyHUD::AMahjongLobbyHUD()
{
    // 默认构造函数，空实现
    // HUD 组件通常不需要特殊初始化
}

// ==================== 游戏生命周期函数 ====================

void AMahjongLobbyHUD::BeginPlay()
{
    // 调用父类的 BeginPlay
    Super::BeginPlay();

    // 仅在客户端初始化 UI
    // 服务器不需要显示大厅界面
    if (IsClient())
    {
        InitializeLobbyUI();
    }
}

void AMahjongLobbyHUD::Tick(float DeltaSeconds)
{
    // 调用父类的 Tick
    Super::Tick(DeltaSeconds);

    // 此处可添加每帧更新的逻辑（如有需要）
    // 当前大厅 HUD 不需要每帧更新
}

// ==================== 客户端 UI 函数 ====================

void AMahjongLobbyHUD::ShowLobbyMainUI()
{
    // 仅在客户端执行
    if (IsClient() && LobbyWidgetInstance)
    {
        // 显示大厅窗口
        LobbyWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMahjongLobbyHUD::RefreshRoomList()
{
    // 仅在客户端执行
    if (IsClient())
    {
        // 获取当前玩家控制器并请求刷新房间列表
        if (AMahjongLobbyPlayerController* PC = Cast<AMahjongLobbyPlayerController>(GetOwningPlayerController()))
        {
            PC->ClientRequestRefreshRoomList();
        }
    }
}

void AMahjongLobbyHUD::ShowCreateRoomPopup()
{
    // 仅在客户端执行
    if (IsClient() && LobbyWidgetInstance)
    {
        // 调用大厅窗口的创建房间弹窗方法
        // 注意：需要在 UMahjongLobbyWidget 中实现 ShowCreateRoomPopup 方法
        LobbyWidgetInstance->ShowCreateRoomPopup();
    }
}

void AMahjongLobbyHUD::ClientJoinRoom(int32 RoomID)
{
    // 仅在客户端执行
    if (IsClient())
    {
        // 获取当前玩家控制器并请求加入房间
        if (AMahjongLobbyPlayerController* PC = Cast<AMahjongLobbyPlayerController>(GetOwningPlayerController()))
        {
            PC->ClientRequestJoinRoom(FString::FromInt(RoomID));
        }
    }
}

void AMahjongLobbyHUD::RequestCreateRoom(const FString& RoomName, int32 MaxPlayers)
{
    // 仅在客户端执行
    if (IsClient())
    {
        // 获取当前玩家控制器并请求创建房间
        // 创建房间的逻辑在 PlayerController 中处理
        if (AMahjongLobbyPlayerController* PC = Cast<AMahjongLobbyPlayerController>(GetOwningPlayerController()))
        {
            PC->ClientRequestCreateRoom(RoomName, MaxPlayers);
        }
    }
}

// ==================== 私有函数 ====================

void AMahjongLobbyHUD::InitializeLobbyUI()
{
    // 检查是否已经创建过窗口实例（防止重复创建）
    if (LobbyWidgetClass && !LobbyWidgetInstance)
    {
        UWorld* World = GetWorld();
        if (!World)
        {
            UE_LOG(LogTemp, Error, TEXT("[MahjongLobbyHUD] GetWorld() returned null"));
            return;
        }

        // 创建大厅窗口实例
        LobbyWidgetInstance = CreateWidget<UMahjongLobbyWidget>(World, LobbyWidgetClass);

        if (LobbyWidgetInstance)
        {
            // 将窗口添加到视口
            LobbyWidgetInstance->AddToViewport();

            // 默认隐藏窗口，等待玩家主动显示
            LobbyWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}