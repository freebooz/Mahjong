#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MahjongLobbyWidget.generated.h"

// 前向声明
class UButton;
class UListView;
class UMahjongRoomListEntry;

/**
 * 大厅界面 Widget，负责显示房间列表和交互按钮。
 */
UCLASS()
class MAHJONGGAME_API UMahjongLobbyWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    UMahjongLobbyWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化函数，绑定 UI 控件的事件
    virtual void NativeConstruct() override;

    // 刷新房间列表的函数
    UFUNCTION(BlueprintCallable, Category = "Lobby Widget")
    void RefreshRoomList();

    // 显示创建房间弹窗
    UFUNCTION(BlueprintCallable, Category = "Lobby Widget")
    void ShowCreateRoomPopup();

protected:
    // 绑定的 UI 控件（需要在蓝图中设置）
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Lobby Widget|UI")
    UButton* CreateRoomButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Lobby Widget|UI")
    UButton* JoinRoomButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Lobby Widget|UI")
    UListView* RoomListView;

    // 按钮点击事件
    UFUNCTION()
    void OnCreateRoomButtonClicked();

    UFUNCTION()
    void OnJoinRoomButtonClicked();

private:
    // 存储房间数据的数组
    UPROPERTY()
    TArray<TObjectPtr<UMahjongRoomListEntry>> RoomEntries;
};