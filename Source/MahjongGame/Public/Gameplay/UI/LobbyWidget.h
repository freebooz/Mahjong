#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"
class UButton;
class UListView;
class UTextBlock;
UCLASS()
class MAHJONGGAME_API ULobbyWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    ULobbyWidget(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    void OnCreateRoomButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    void OnJoinRoomButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    void OnRefreshButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    void OnLogoutButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    void RefreshRoomList();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Lobby")
    void OnRoomSelected(int32 RoomID);
protected:
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CreateRoomButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* JoinRoomButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* RefreshButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* LogoutButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UListView* RoomListView;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PlayerNameText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PlayerCoinsText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusText;
};