#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MahjongRoomListEntry.generated.h" // 调整为 Lobby 路径

class UTextBlock;
class UButton;

/**
 * 房间列表项 Widget 类，用于大厅界面中显示单个房间的信息。
 */
UCLASS()
class MAHJONGGAME_API UMahjongRoomListEntry : public UUserWidget
{
    GENERATED_BODY()

public:
    UMahjongRoomListEntry(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Room List Entry")
    void SetupRoomEntry(const FString& RoomName, int PlayerCount, int MaxPlayers, int RoomID);

    // 获取 RoomID 的 getter 方法
    UFUNCTION(BlueprintCallable, Category = "Room List Entry")
    int32 GetRoomID() const { return RoomID; }

    // 虚函数声明
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Room List Entry|UI")
    UTextBlock* RoomNameText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Room List Entry|UI")
    UTextBlock* RoomIDText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Room List Entry|UI")
    UTextBlock* PlayerCountText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Room List Entry|UI")
    UButton* JoinRoomButton;

public:
    UPROPERTY()
    int32 RoomID;

    UPROPERTY()
    FString RoomName;

    bool bIsButtonBound;

private:
    // 按钮点击事件
    UFUNCTION()
    void OnJoinRoomButtonClicked();

    // 检查控件绑定
    void ValidateWidgetBindings();
};