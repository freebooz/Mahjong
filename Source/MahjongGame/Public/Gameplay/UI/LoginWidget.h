#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"
class UButton;
class UEditableTextBox;
class UTextBlock;
UCLASS()
class MAHJONGGAME_API ULoginWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    ULoginWidget(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    void OnLoginButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    void OnRegisterButtonClicked();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Login")
    void CreateLoginUI();
protected:
    UPROPERTY(BlueprintReadOnly)
    UButton* LoginButton;
    UPROPERTY(BlueprintReadOnly)
    UButton* RegisterButton;
    UPROPERTY(BlueprintReadOnly)
    UEditableTextBox* UsernameInput;
    UPROPERTY(BlueprintReadOnly)
    UEditableTextBox* PasswordInput;
    UPROPERTY(BlueprintReadOnly)
    UTextBlock* ErrorMessageText;
};