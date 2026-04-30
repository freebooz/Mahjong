#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "MahjongAudioManager.generated.h"

/**
 * 麻将游戏的音频管理器类
 * 负责处理背景音乐和音效的播放
 */
UCLASS()
class MAHJONGGAME_API UMahjongAudioManager : public UObject
{
    GENERATED_BODY()

public:
    // 构造函数
    UMahjongAudioManager();

    // 播放音效
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySoundEffect(UObject* WorldContextObject, USoundCue* SoundCue, FVector Location);

    // 播放背景音乐
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayBackgroundMusic(UObject* WorldContextObject, USoundCue* MusicCue);

    // 停止背景音乐
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void StopBackgroundMusic();

private:
    // 背景音乐组件
    UPROPERTY()
    UAudioComponent* BackgroundMusicComponent;
};