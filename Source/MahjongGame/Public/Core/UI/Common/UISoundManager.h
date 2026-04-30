#pragma once
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UISoundManager.generated.h"

//==============================================================================
// 通用层 - UI 音效管理器
// 提供按钮点击等 UI 音效播放功能
// 使用 TSoftObjectPtr 缓存音效，避免每次播放时重复加载
//==============================================================================
UCLASS()
class MAHJONGGAME_API UUISoundManager : public UObject
{
    GENERATED_BODY()

public:
    // 播放按钮点击音效
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void PlayButtonClick(UObject* WorldContextObject);

    // 播放确认按钮音效
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void PlayConfirmClick(UObject* WorldContextObject);

    // 播放取消按钮音效
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void PlayCancelClick(UObject* WorldContextObject);

    // 播放成功音效
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void PlaySuccessSound(UObject* WorldContextObject);

    // 播放错误音效
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void PlayErrorSound(UObject* WorldContextObject);

    // 播放麻将相关音效
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void PlayMahjongTileSound(UObject* WorldContextObject, int32 SoundType);

    // 预加载所有音效（可在游戏开始时调用）
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void PreloadAllSounds();

    // 清除音效缓存
    UFUNCTION(BlueprintCallable, Category="UI|Sound")
    static void ClearSoundCache();

protected:
    // 获取音效（带缓存）
    static USoundBase* GetCachedSound(const FSoftObjectPath& SoundPath);

    // UI 音效路径
    static FSoftObjectPath GetButtonClickSoundPath();
    static FSoftObjectPath GetConfirmSoundPath();
    static FSoftObjectPath GetCancelSoundPath();
    static FSoftObjectPath GetSuccessSoundPath();
    static FSoftObjectPath GetErrorSoundPath();
    static FSoftObjectPath GetMahjongSoundPath(int32 SoundType);

private:
    // 音效缓存（使用 TMap 存储已加载的音效）
    static TMap<FSoftObjectPath, TWeakObjectPtr<USoundBase>> SoundCache;

    // 缓存键
    static const FString ButtonClickKey;
    static const FString ConfirmKey;
    static const FString CancelKey;
    static const FString SuccessKey;
    static const FString ErrorKey;
};
