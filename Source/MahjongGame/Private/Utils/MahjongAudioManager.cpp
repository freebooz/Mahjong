#include "Utils/MahjongAudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h" // 添加 USoundCue 的头文件

UMahjongAudioManager::UMahjongAudioManager()
{
    BackgroundMusicComponent = nullptr;
}

void UMahjongAudioManager::PlaySoundEffect(UObject* WorldContextObject, USoundCue* SoundCue, FVector Location)
{
    if (SoundCue && WorldContextObject)
    {
        UGameplayStatics::PlaySoundAtLocation(WorldContextObject, SoundCue, Location);
    }
}

void UMahjongAudioManager::PlayBackgroundMusic(UObject* WorldContextObject, USoundCue* MusicCue)
{
    if (MusicCue && WorldContextObject)
    {
        if (BackgroundMusicComponent)
        {
            BackgroundMusicComponent->Stop();
        }
        // 显式转换为 USoundBase*
        BackgroundMusicComponent = UGameplayStatics::SpawnSound2D(WorldContextObject, static_cast<USoundBase*>(MusicCue));
    }
}

void UMahjongAudioManager::StopBackgroundMusic()
{
    if (BackgroundMusicComponent)
    {
        BackgroundMusicComponent->Stop();
    }
}