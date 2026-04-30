#include "Core/UI/Common/SoundButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Core/UI/Common/UISoundManager.h"

USoundButton::USoundButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CurrentStyle = EButtonStyle::Normal;
    CurrentSoundType = EButtonSoundType::Standard;
}

void USoundButton::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &USoundButton::HandleClick);
        Button->OnHovered.AddDynamic(this, &USoundButton::NativeOnHovered);
        Button->OnUnhovered.AddDynamic(this, &USoundButton::NativeOnUnhovered);
    }
}

void USoundButton::NativeOnHovered()
{
}

void USoundButton::NativeOnUnhovered()
{
}

void USoundButton::SetButtonText(const FString& Text)
{
    if (ButtonText)
        ButtonText->SetText(FText::FromString(Text));
}

FString USoundButton::GetButtonText() const
{
    if (ButtonText)
        return ButtonText->GetText().ToString();
    return FString();
}

void USoundButton::SetStyle(EButtonStyle Style)
{
    CurrentStyle = Style;
}

void USoundButton::SetSoundType(EButtonSoundType SoundType)
{
    CurrentSoundType = SoundType;
}

UButton* USoundButton::GetButton() const
{
    return Button;
}

void USoundButton::HandleClick()
{
    // 播放音效
    UObject* WorldContext = GetWorld();
    switch (CurrentSoundType)
    {
        case EButtonSoundType::Standard:
            UUISoundManager::PlayButtonClick(WorldContext);
            break;
        case EButtonSoundType::Confirm:
            UUISoundManager::PlayConfirmClick(WorldContext);
            break;
        case EButtonSoundType::Cancel:
            UUISoundManager::PlayCancelClick(WorldContext);
            break;
        case EButtonSoundType::Success:
            UUISoundManager::PlaySuccessSound(WorldContext);
            break;
        case EButtonSoundType::Error:
            UUISoundManager::PlayErrorSound(WorldContext);
            break;
    }

    // 触发 Blueprint 事件
    OnButtonClicked();
}
