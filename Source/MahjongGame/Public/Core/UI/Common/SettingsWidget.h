#pragma once
#include "CoreMinimal.h"
#include "Core/UI/Common/BaseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "SettingsWidget.generated.h"

//==============================================================================
// 通用层 - 设置界面
// 游戏设置面板，包含音量、网络、账号等配置
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API USettingsWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    // 构造函数
    USettingsWidget(const FObjectInitializer& ObjectInitializer);

    // 显示设置
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void ShowSettings();

    // 隐藏设置
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void HideSettings();

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 关闭按钮点击
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnCloseClicked();

    // 音乐音量改变
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnMusicVolumeChanged(float Value);

    // 音效音量改变
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnSFXVolumeChanged(float Value);

    // 音乐开关改变
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnMusicMuteChanged(bool bIsMuted);

    // 音效开关改变
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnSFXMuteChanged(bool bIsMuted);

    // 震动开关改变
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnVibrationChanged(bool bEnabled);

    // 账号按钮点击
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnAccountClicked();

    // 关于按钮点击
    UFUNCTION(BlueprintCallable, Category="Common|Settings")
    void OnAboutClicked();

    // 根面板
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCanvasPanel* RootCanvas;

    // 标题
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* TitleText;

    // 关闭按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* CloseButton;

    // 设置项容器
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UVerticalBox* SettingsContainer;

    // 音乐音量滑块
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    USlider* MusicVolumeSlider;

    // 音效音量滑块
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    USlider* SFXVolumeSlider;

    // 音乐静音复选框
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCheckBox* MusicMuteCheckBox;

    // 音效静音复选框
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCheckBox* SFXMuteCheckBox;

    // 震动复选框
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UCheckBox* VibrationCheckBox;

    // 账号按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* AccountButton;

    // 关于按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* AboutButton;
};
