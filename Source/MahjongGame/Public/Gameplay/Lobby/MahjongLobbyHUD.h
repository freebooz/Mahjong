#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Gameplay/Lobby/MahjongLobbyPlayerController.h"
#include "MahjongLobbyHUD.generated.h"

// 前向声明：大厅窗口组件类
// 使用前向声明可以避免循环 include
class UMahjongLobbyWidget;

/**
 * 大厅HUD类，负责管理大厅界面的显示和交互逻辑。
 * 在独立服务器模式下，此类区分了服务器端和客户端的职责：
 * - 客户端：负责UI显示和用户输入
 * - 服务器：负责数据同步和房间管理
 *
 * 设计说明：
 * - 使用 BlueprintCallable 函数允许蓝图调用
 * - 使用 IsClient/IsServer 区分运行模式
 * - 房间操作通过 PlayerController 转发到服务器
 */
UCLASS()
class MAHJONGGAME_API AMahjongLobbyHUD : public AHUD
{
    GENERATED_BODY()

public:
    // 构造函数：初始化默认参数
    AMahjongLobbyHUD();

    // BeginPlay：游戏开始时初始化UI
    virtual void BeginPlay() override;

    // Tick：每帧更新（如有需要）
    virtual void Tick(float DeltaSeconds) override;

    // 显示大厅主界面
    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void ShowLobbyMainUI();

    // 刷新房间列表
    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void RefreshRoomList();

    // 显示创建房间弹窗
    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void ShowCreateRoomPopup();

    // 请求加入指定房间
    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void ClientJoinRoom(int32 RoomID);

    // 请求创建新房间
    UFUNCTION(BlueprintCallable, Category = "Lobby HUD|Client")
    void RequestCreateRoom(const FString& RoomName, int32 MaxPlayers);

protected:
    // 大厅窗口类的引用（用于在编辑器中指定）
    UPROPERTY(EditDefaultsOnly, Category = "Lobby HUD|UI")
    TSubclassOf<UMahjongLobbyWidget> LobbyWidgetClass;

    // 大厅窗口实例（运行时创建）
    UPROPERTY(Transient)
    UMahjongLobbyWidget* LobbyWidgetInstance;

private:
    // 初始化大厅UI组件
    void InitializeLobbyUI();

    // 判断是否为客户端（不是专用服务器）
    bool IsClient() const { return GetNetMode() != NM_DedicatedServer; }

    // 判断是否为服务器（专用服务器或监听服务器）
    bool IsServer() const { return GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer; }
};