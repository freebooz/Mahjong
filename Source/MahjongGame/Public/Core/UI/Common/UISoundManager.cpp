#include "Core/UI/Common/UISoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// 静态成员初始化
TMap<FSoftObjectPath, TWeakObjectPtr<USoundBase>> UUISoundManager::SoundCache;

const FString UUISoundManager::ButtonClickKey = TEXT("ButtonClick");
const FString UUISoundManager::ConfirmKey = TEXT("Confirm");
const FString UUISoundManager::CancelKey = TEXT("Cancel");
const FString UUISoundManager::SuccessKey = TEXT("Success");
const FString UUISoundManager::ErrorKey = TEXT("Error");

FSoftObjectPath UUISoundManager::GetButtonClickSoundPath()
{
    return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/UI/ButtonClick_Cue.ButtonClick_Cue'"));
}

FSoftObjectPath UUISoundManager::GetConfirmSoundPath()
{
    return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/UI/Confirm_Cue.Confirm_Cue'"));
}

FSoftObjectPath UUISoundManager::GetCancelSoundPath()
{
    return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/UI/Cancel_Cue.Cancel_Cue'"));
}

FSoftObjectPath UUISoundManager::GetSuccessSoundPath()
{
    return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/UI/Success_Cue.Success_Cue'"));
}

FSoftObjectPath UUISoundManager::GetErrorSoundPath()
{
    return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/UI/Error_Cue.Error_Cue'"));
}

FSoftObjectPath UUISoundManager::GetMahjongSoundPath(int32 SoundType)
{
    switch (SoundType)
    {
        case 0: return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/Mahjong/DrawTile_Cue.DrawTile_Cue'"));    // 摸牌
        case 1: return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/Mahjong/DiscardTile_Cue.DiscardTile_Cue'")); // 出牌
        case 2: return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/Mahjong/Chow_Cue.Chow_Cue'"));          // 吃
        case 3: return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/Mahjong/Pong_Cue.Pong_Cue'"));          // 碰
        case 4: return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/Mahjong/Kong_Cue.Kong_Cue'"));          // 杠
        case 5: return FSoftObjectPath(TEXT("SoundCue'/Game/Audio/Mahjong/Hu_Cue.Hu_Cue'"));              // 胡
        default: return GetButtonClickSoundPath();
    }
}

USoundBase* UUISoundManager::GetCachedSound(const FSoftObjectPath& SoundPath)
{
    // 检查缓存中是否已有
    TWeakObjectPtr<USoundBase>* CachedSound = SoundCache.Find(SoundPath);
    if (CachedSound && CachedSound->IsValid())
    {
        return CachedSound->Get();
    }

    // 缓存无效或不存在，加载新资源
    USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *SoundPath.ToString()));

    if (Sound)
    {
        // 加入缓存
        SoundCache.Add(SoundPath, Sound);
    }

    return Sound;
}

void UUISoundManager::PlayButtonClick(UObject* WorldContextObject)
{
    if (!WorldContextObject)
        return;

    USoundBase* Sound = GetCachedSound(GetButtonClickSoundPath());
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
    }
}

void UUISoundManager::PlayConfirmClick(UObject* WorldContextObject)
{
    if (!WorldContextObject)
        return;

    USoundBase* Sound = GetCachedSound(GetConfirmSoundPath());
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
    }
}

void UUISoundManager::PlayCancelClick(UObject* WorldContextObject)
{
    if (!WorldContextObject)
        return;

    USoundBase* Sound = GetCachedSound(GetCancelSoundPath());
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
    }
}

void UUISoundManager::PlaySuccessSound(UObject* WorldContextObject)
{
    if (!WorldContextObject)
        return;

    USoundBase* Sound = GetCachedSound(GetSuccessSoundPath());
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
    }
}

void UUISoundManager::PlayErrorSound(UObject* WorldContextObject)
{
    if (!WorldContextObject)
        return;

    USoundBase* Sound = GetCachedSound(GetErrorSoundPath());
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
    }
}

void UUISoundManager::PlayMahjongTileSound(UObject* WorldContextObject, int32 SoundType)
{
    if (!WorldContextObject)
        return;

    USoundBase* Sound = GetCachedSound(GetMahjongSoundPath(SoundType));
    if (Sound)
    {
        UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
    }
}

void UUISoundManager::PreloadAllSounds()
{
    // 预加载所有 UI 音效
    GetCachedSound(GetButtonClickSoundPath());
    GetCachedSound(GetConfirmSoundPath());
    GetCachedSound(GetCancelSoundPath());
    GetCachedSound(GetSuccessSoundPath());
    GetCachedSound(GetErrorSoundPath());

    // 预加载所有麻将音效
    for (int32 i = 0; i <= 5; ++i)
    {
        GetCachedSound(GetMahjongSoundPath(i));
    }
}

void UUISoundManager::ClearSoundCache()
{
    SoundCache.Empty();
}
