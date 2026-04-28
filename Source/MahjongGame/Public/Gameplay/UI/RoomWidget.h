#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomWidget.generated.h"
class UButton;
class UImage;
class UTextBlock;
UCLASS()
class MAHJONGGAME_API URoomWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    URoomWidget(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnLeaveButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnStartGameButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnDrawTileButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void OnDiscardTileButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Room")
    void UpdateGameState(const FString& State);
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
    UTextBlock* RoomIDText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PlayerCountText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* StatusText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TurnInfoText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* TableImage;
};