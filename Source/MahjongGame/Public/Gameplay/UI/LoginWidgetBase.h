#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/ProgressBar.h"
#include "Account/PlayerAccountService.h"
#include "LoginWidgetBase.generated.h"
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API ULoginWidgetBase : public UUserWidget
{
    GENERATED_BODY()
public:
    ULoginWidgetBase(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnLoginClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnRegisterClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void ShowError(const FString& Message);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void ClearInputs();
    UFUNCTION(BlueprintPure, Category="Mahjong|Login")
    virtual FString GetUsername() const;
    UFUNCTION(BlueprintPure, Category="Mahjong|Login")
    virtual FString GetPassword() const;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnWeChatLoginClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnGuestLoginClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void OnLoginComplete(const FLoginResult& Result);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    virtual void ShowReconnectStatus(const FString& Status, float Progress);
protected:
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* LoginButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* RegisterButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* WeChatLoginButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* GuestLoginButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UEditableTextBox* UsernameInput;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UEditableTextBox* PasswordInput;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ErrorText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TitleText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* UsernameLabel;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* PasswordLabel;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ReconnectStatusText;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UProgressBar* ReconnectProgressBar;
};