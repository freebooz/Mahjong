#include "Gameplay/UI/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Gameplay/MahjongGameInstance.h"
#include "Kismet/GameplayStatics.h"

ULobbyWidget::ULobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Constructed"));

    // 绑定按钮事件
    if (CreateRoomButton)
        CreateRoomButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnCreateRoomButtonClicked);
    if (JoinRoomButton)
        JoinRoomButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnJoinRoomButtonClicked);
    if (RefreshButton)
        RefreshButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnRefreshButtonClicked);
    if (LogoutButton)
        LogoutButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnLogoutButtonClicked);

    // 显示玩家信息
    if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        FString PlayerName = GI->GetPlayerName();
        if (PlayerNameText)
            PlayerNameText->SetText(FText::FromString(PlayerName.IsEmpty() ? TEXT("玩家") : PlayerName));

        // 从会话服务获取金币数
        if (PlayerCoinsText)
            PlayerCoinsText->SetText(FText::FromString(TEXT("1000")));
    }

    // 刷新房间列表
    RefreshRoomList();
}

void ULobbyWidget::OnCreateRoomButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Create room clicked"));

    // 显示创建房间提示
    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("正在创建房间...")));

    // 禁用按钮防止重复点击
    if (CreateRoomButton)
        CreateRoomButton->SetIsEnabled(false);
    if (JoinRoomButton)
        JoinRoomButton->SetIsEnabled(false);

    // TODO: 调用房间服务创建房间
    // 创建成功后跳转到房间
}

void ULobbyWidget::OnJoinRoomButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Join room clicked"));

    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("请从列表中选择房间")));

    // TODO: 显示房间ID输入对话框
}

void ULobbyWidget::OnRefreshButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Refresh clicked"));
    RefreshRoomList();
}

void ULobbyWidget::OnLogoutButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Logout clicked"));

    // 确认登出
    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("正在退出登录...")));

    // 返回登录页面
    UGameplayStatics::OpenLevel(this, TEXT("Login"), true, TEXT("listen"));
}

void ULobbyWidget::RefreshRoomList()
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Refreshing room list"));

    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("正在加载房间列表...")));

    // TODO: 从服务器获取房间列表
    // 模拟：显示可用房间数量

    if (StatusText)
        StatusText->SetText(FText::FromString(TEXT("暂无房间，请创建新房间")));

    // 恢复按钮状态
    if (CreateRoomButton)
        CreateRoomButton->SetIsEnabled(true);
    if (JoinRoomButton)
        JoinRoomButton->SetIsEnabled(true);
}

void ULobbyWidget::OnRoomSelected(int32 RoomID)
{
    UE_LOG(LogTemp, Log, TEXT("[LobbyWidget] Room selected: %d"), RoomID);

    if (StatusText)
        StatusText->SetText(FText::FromString(FString::Printf(TEXT("正在加入房间 %d..."), RoomID)));

    // TODO: 调用房间服务加入房间
    // 加入成功后跳转到游戏
}