#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "SettingsWidgetBase.generated.h"
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API USettingsWidgetBase : public UUserWidget
{
    GENERATED_BODY()
public:
    USettingsWidgetBase(const FObjectInitializer& ObjectInitializer);
    virtual void NativeConstruct() override;
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void ShowSettingsPanel();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void HideSettingsPanel();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void ApplySettings();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void CancelSettings();
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetMasterVolume(float Volume);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetMusicVolume(float Volume);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetSFXVolume(float Volume);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetGraphicsQuality(int32 Quality);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetResolution(int32 Width, int32 Height);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetFullscreen(bool bFullscreen);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetPlayerName(const FString& Name);
    UFUNCTION(BlueprintCallable, Category="Mahjong|Settings")
    virtual void SetAutoReady(bool bAutoReady);
protected:
    // Event handlers
    UFUNCTION()
    void OnCloseClicked();

    UFUNCTION()
    void OnApplyClicked();

    UFUNCTION()
    void OnCancelClicked();

    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnMusicVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);

    UFUNCTION()
    void OnFullscreenChanged(bool bIsChecked);

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* SettingsPanel;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CloseButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* ApplyButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CancelButton;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    USlider* MasterVolumeSlider;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    USlider* MusicVolumeSlider;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    USlider* SFXVolumeSlider;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UComboBoxString* GraphicsQualityCombo;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UComboBoxString* ResolutionCombo;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCheckBox* FullscreenCheckBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UEditableTextBox* PlayerNameTextBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCheckBox* AutoReadyCheckBox;
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* SettingsTitleText;
};