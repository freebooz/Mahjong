#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/CheckBox.h"
#include "GameWidgetBase.generated.h"
USTRUCT(BlueprintType)
struct FPlayerInfo
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 PlayerIndex;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString PlayerName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Score;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsReady;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsOnline;
};
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API UGameWidgetBase : public UUserWidget
{
    GENERATED_BODY()
public:
    UGameWidgetBase(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    // Room management
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Room")
    virtual void ShowCreateRoomDialog();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Room")
    virtual void ShowJoinRoomDialog();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Room")
    virtual void HideRoomDialog();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Room")
    virtual void SetRoomInfo(int32 RoomID, const FString& RoomName, const FString& Password, int32 PlayerCount, int32 MaxPlayers);
    // Player info
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Player")
    virtual void UpdatePlayerList(const TArray<FPlayerInfo>& Players);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Player")
    virtual void UpdateMyInfo(const FPlayerInfo& MyInfo);
    // Tile actions
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Tile")
    virtual void ShowMyHandTiles(const TArray<int32>& TileIDs);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Tile")
    virtual void OnTileClicked(int32 TileIndex);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Tile")
    virtual void SetSelectedTile(int32 TileIndex);
    // Action buttons
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Action")
    virtual void ShowActionButtons(bool bShowChow, bool bShowPong, bool bShowKong, bool bShowHu, bool bShowZhuaJi);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Action")
    virtual void HideAllActionButtons();
    // Turn indicator
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Turn")
    virtual void SetCurrentTurnPlayer(int32 PlayerIndex);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Turn")
    virtual void SetMyTurn(bool bIsMyTurn);
    // Draw/Discard
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Tile")
    virtual void ShowDrawButton(bool bShow);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Tile")
    virtual void ShowDiscardButton(bool bShow);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Tile")
    virtual void AddDiscardedTileToPool(int32 TileID);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Tile")
    virtual void ShowDiscardPool(const TArray<int32>& TileIDs);
    // Status messages
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Status")
    virtual void ShowStatusMessage(const FString& Message);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Game|Status")
    virtual void ShowErrorMessage(const FString& Error);
protected:
    // Event handlers for buttons
    UFUNCTION()
    void OnCreateRoomClicked();

    UFUNCTION()
    void OnJoinRoomClicked();

    UFUNCTION()
    void OnLeaveClicked();

    UFUNCTION()
    void OnDrawTileClicked();

    UFUNCTION()
    void OnDiscardTileClicked();

    UFUNCTION()
    void OnChowClicked();

    UFUNCTION()
    void OnPongClicked();

    UFUNCTION()
    void OnKongClicked();

    UFUNCTION()
    void OnHuClicked();

    UFUNCTION()
    void OnZhuaJiClicked();

    // Selected tile index
    int32 SelectedTileIndex;

    // Room UI elements
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RoomDialogPanel;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CreateRoomButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* JoinRoomButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* LeaveButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* RoomIDText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* RoomNameText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PlayerCountText;
    // Player list
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* PlayerListBox;
    // Hand tiles
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHorizontalBox* HandTilesBox;
    // Action buttons
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* DrawTileButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* DiscardButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ChowButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* PongButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* KongButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* HuButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ZhuaJiButton;
    // Turn info
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TurnInfoText;
    // Status
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ErrorText;
    // Discard pool
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UScrollBox* DiscardPoolScroll;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHorizontalBox* DiscardPoolBox;
};