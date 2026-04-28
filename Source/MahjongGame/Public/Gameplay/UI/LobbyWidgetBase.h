#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "LobbyWidgetBase.generated.h"
USTRUCT(BlueprintType)
struct FLobbyRoomInfo
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RoomID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString RoomName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CurrentPlayers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxPlayers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsJoinable;
};
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API ULobbyWidgetBase : public UUserWidget
{
    GENERATED_BODY()
public:
    ULobbyWidgetBase(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void OnCreateRoomClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void OnJoinRoomClicked(int32 RoomID);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void OnRefreshClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void OnLogoutClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void RefreshRoomList();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void AddRoomToList(const FLobbyRoomInfo& RoomInfo);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void ClearRoomList();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual void SetPlayerInfo(const FString& PlayerName, int32 Coins);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    virtual FString GetSelectedRoomName() const;
protected:
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CreateRoomButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* RefreshButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* LogoutButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PlayerNameText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* CoinsText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UListView* RoomListView;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UScrollBox* RoomScrollBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* RoomListContainer;
private:
    TArray<FLobbyRoomInfo> RoomInfos;
};