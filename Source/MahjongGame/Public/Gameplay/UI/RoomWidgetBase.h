#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "RoomWidgetBase.generated.h"
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API URoomWidgetBase : public UUserWidget
{
    GENERATED_BODY()
public:
    URoomWidgetBase(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnLeaveClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnStartGameClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnDrawTileClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void OnDiscardTileClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void UpdateRoomInfo(int32 RoomID, int32 CurrentPlayers, int32 MaxPlayers);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void UpdateTurnInfo(int32 CurrentPlayer, bool bIsMyTurn);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void ShowActionButtons(bool bShowDraw, bool bShowDiscard, bool bShowChow, bool bShowPong, bool bShowKong, bool bShowHu);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    virtual void SetGameStatus(const FString& Status);
protected:
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* LeaveButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* StartGameButton;
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
    UTextBlock* RoomIDText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PlayerCountText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TurnInfoText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* TableImage;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UHorizontalBox* PlayerHandBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* PlayerPositionBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;
};