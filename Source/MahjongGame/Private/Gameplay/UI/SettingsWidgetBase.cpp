#include "Gameplay/UI/SettingsWidgetBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

USettingsWidgetBase::USettingsWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void USettingsWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    if (CloseButton)
        CloseButton->OnClicked.AddDynamic(this, &USettingsWidgetBase::OnCloseClicked);
    if (ApplyButton)
        ApplyButton->OnClicked.AddDynamic(this, &USettingsWidgetBase::OnApplyClicked);
    if (CancelButton)
        CancelButton->OnClicked.AddDynamic(this, &USettingsWidgetBase::OnCancelClicked);
    if (MasterVolumeSlider)
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidgetBase::OnMasterVolumeChanged);
    if (MusicVolumeSlider)
        MusicVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidgetBase::OnMusicVolumeChanged);
    if (SFXVolumeSlider)
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidgetBase::OnSFXVolumeChanged);
    if (FullscreenCheckBox)
        FullscreenCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingsWidgetBase::OnFullscreenChanged);
    if (SettingsPanel)
        SettingsPanel->SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] NativeConstruct"));
}

void USettingsWidgetBase::ShowSettingsPanel()
{
    if (SettingsPanel)
        SettingsPanel->SetVisibility(ESlateVisibility::Visible);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] ShowSettingsPanel"));
}

void USettingsWidgetBase::HideSettingsPanel()
{
    if (SettingsPanel)
        SettingsPanel->SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] HideSettingsPanel"));
}

void USettingsWidgetBase::ApplySettings()
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] ApplySettings"));
    float MasterVol = MasterVolumeSlider ? MasterVolumeSlider->GetValue() : 1.0f;
    float MusicVol = MusicVolumeSlider ? MusicVolumeSlider->GetValue() : 1.0f;
    float SFXVol = SFXVolumeSlider ? SFXVolumeSlider->GetValue() : 1.0f;
    bool bFullscreen = FullscreenCheckBox ? FullscreenCheckBox->IsChecked() : false;
    FString PlayerName = PlayerNameTextBox ? PlayerNameTextBox->GetText().ToString() : TEXT("");
    bool bAutoReady = AutoReadyCheckBox ? AutoReadyCheckBox->IsChecked() : false;
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] Applied: MasterVol=%.2f, MusicVol=%.2f, SFXVol=%.2f, Fullscreen=%s, Name=%s, AutoReady=%s"),
        MasterVol, MusicVol, SFXVol, bFullscreen ? TEXT("true") : TEXT("false"), *PlayerName, bAutoReady ? TEXT("true") : TEXT("false"));
    HideSettingsPanel();
}

void USettingsWidgetBase::CancelSettings()
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] CancelSettings"));
    HideSettingsPanel();
}

void USettingsWidgetBase::SetMasterVolume(float Volume)
{
    if (MasterVolumeSlider)
        MasterVolumeSlider->SetValue(Volume);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetMasterVolume: %.2f"), Volume);
}

void USettingsWidgetBase::SetMusicVolume(float Volume)
{
    if (MusicVolumeSlider)
        MusicVolumeSlider->SetValue(Volume);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetMusicVolume: %.2f"), Volume);
}

void USettingsWidgetBase::SetSFXVolume(float Volume)
{
    if (SFXVolumeSlider)
        SFXVolumeSlider->SetValue(Volume);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetSFXVolume: %.2f"), Volume);
}

void USettingsWidgetBase::SetGraphicsQuality(int32 Quality)
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetGraphicsQuality: %d"), Quality);
}

void USettingsWidgetBase::SetResolution(int32 Width, int32 Height)
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetResolution: %dx%d"), Width, Height);
}

void USettingsWidgetBase::SetFullscreen(bool bFullscreen)
{
    if (FullscreenCheckBox)
        FullscreenCheckBox->SetIsChecked(bFullscreen);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetFullscreen: %s"), bFullscreen ? TEXT("true") : TEXT("false"));
}

void USettingsWidgetBase::SetPlayerName(const FString& Name)
{
    if (PlayerNameTextBox)
        PlayerNameTextBox->SetText(FText::FromString(Name));
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetPlayerName: %s"), *Name);
}

void USettingsWidgetBase::SetAutoReady(bool bAutoReady)
{
    if (AutoReadyCheckBox)
        AutoReadyCheckBox->SetIsChecked(bAutoReady);
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] SetAutoReady: %s"), bAutoReady ? TEXT("true") : TEXT("false"));
}

void USettingsWidgetBase::OnCloseClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] OnCloseClicked"));
    HideSettingsPanel();
}

void USettingsWidgetBase::OnApplyClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] OnApplyClicked"));
    ApplySettings();
}

void USettingsWidgetBase::OnCancelClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] OnCancelClicked"));
    CancelSettings();
}

void USettingsWidgetBase::OnMasterVolumeChanged(float Value)
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] OnMasterVolumeChanged: %.2f"), Value);
}

void USettingsWidgetBase::OnMusicVolumeChanged(float Value)
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] OnMusicVolumeChanged: %.2f"), Value);
}

void USettingsWidgetBase::OnSFXVolumeChanged(float Value)
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] OnSFXVolumeChanged: %.2f"), Value);
}

void USettingsWidgetBase::OnFullscreenChanged(bool bIsChecked)
{
    UE_LOG(LogTemp, Log, TEXT("[SettingsWidgetBase] OnFullscreenChanged: %s"), bIsChecked ? TEXT("true") : TEXT("false"));
}