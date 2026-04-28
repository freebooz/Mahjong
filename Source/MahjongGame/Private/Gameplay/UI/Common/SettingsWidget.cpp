#include "Gameplay/UI/Common/SettingsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Gameplay/UI/Common/UISoundManager.h"

USettingsWidget::USettingsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseButton)
        CloseButton->OnClicked.AddDynamic(this, &USettingsWidget::OnCloseClicked);

    if (AccountButton)
        AccountButton->OnClicked.AddDynamic(this, &USettingsWidget::OnAccountClicked);

    if (AboutButton)
        AboutButton->OnClicked.AddDynamic(this, &USettingsWidget::OnAboutClicked);

    HideSettings();
}

void USettingsWidget::ShowSettings()
{
    Show();
}

void USettingsWidget::HideSettings()
{
    Hide();
}

void USettingsWidget::OnCloseClicked()
{
    UUISoundManager::PlayCancelClick(this);
    HideSettings();
}

void USettingsWidget::OnMusicVolumeChanged(float Value)
{
    UE_LOG(LogTemp, Log, TEXT("[Settings] 音乐音量: %.1f"), Value);
}

void USettingsWidget::OnSFXVolumeChanged(float Value)
{
    UE_LOG(LogTemp, Log, TEXT("[Settings] 音效音量: %.1f"), Value);
}

void USettingsWidget::OnMusicMuteChanged(bool bIsMuted)
{
    UE_LOG(LogTemp, Log, TEXT("[Settings] 音乐静音: %s"), bIsMuted ? TEXT("是") : TEXT("否"));
}

void USettingsWidget::OnSFXMuteChanged(bool bIsMuted)
{
    UE_LOG(LogTemp, Log, TEXT("[Settings] 音效静音: %s"), bIsMuted ? TEXT("是") : TEXT("否"));
}

void USettingsWidget::OnVibrationChanged(bool bEnabled)
{
    UE_LOG(LogTemp, Log, TEXT("[Settings] 震动: %s"), bEnabled ? TEXT("开启") : TEXT("关闭"));
}

void USettingsWidget::OnAccountClicked()
{
    UUISoundManager::PlayButtonClick(this);
    UE_LOG(LogTemp, Log, TEXT("[Settings] 账号设置"));
}

void USettingsWidget::OnAboutClicked()
{
    UUISoundManager::PlayButtonClick(this);
    UE_LOG(LogTemp, Log, TEXT("[Settings] 关于"));
}
