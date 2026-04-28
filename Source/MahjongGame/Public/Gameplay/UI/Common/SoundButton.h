#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "SoundButton.generated.h"

//==============================================================================
// 通用层 - 带音效的按钮组件
// 所有 UI 按钮的基类，自动播放点击音效
// 支持普通按钮、图标按钮、文字按钮三种样式
//==============================================================================
UCLASS(Abstract, Blueprintable)
class MAHJONGGAME_API USoundButton : public UUserWidget
{
    GENERATED_BODY()

public:
    // 按钮样式
    UENUM(BlueprintType)
    enum class EButtonStyle : uint8
    {
        Normal,     // 普通样式
        Icon,       // 图标按钮
        TextOnly    // 仅文字
    };

    // 按钮类型（决定播放哪种音效）
    UENUM(BlueprintType)
    enum class EButtonSoundType : uint8
    {
        Standard,   // 标准点击
        Confirm,    // 确认按钮
        Cancel,     // 取消按钮
        Success,    // 成功
        Error       // 错误
    };

    // 构造函数
    USoundButton(const FObjectInitializer& ObjectInitializer);

    // 设置按钮文本
    UFUNCTION(BlueprintCallable, Category="UI|Button")
    void SetButtonText(const FString& Text);

    // 获取按钮文本
    UFUNCTION(BlueprintCallable, Category="UI|Button")
    FString GetButtonText() const;

    // 设置按钮样式
    UFUNCTION(BlueprintCallable, Category="UI|Button")
    void SetStyle(EButtonStyle Style);

    // 设置音效类型
    UFUNCTION(BlueprintCallable, Category="UI|Button")
    void SetSoundType(EButtonSoundType SoundType);

    // 获取原生按钮组件
    UFUNCTION(BlueprintCallable, Category="UI|Button")
    UButton* GetButton() const;

    // 按钮点击事件
    UFUNCTION(BlueprintImplementableEvent, Category="UI|Button")
    void OnButtonClicked();

protected:
    // 界面构建完成
    virtual void NativeConstruct() override;

    // 鼠标悬停事件
    virtual void NativeOnHovered() override;

    // 鼠标离开事件
    virtual void NativeOnUnhovered() override;

    // 内部点击处理
    UFUNCTION(BlueprintCallable, Category="UI|Button")
    void HandleClick();

    // 根按钮
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UButton* Button;

    // 按钮文本
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UTextBlock* ButtonText;

    // 按钮图标
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* ButtonIcon;

    // 背景图片
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    UImage* BackgroundImage;

    // 当前样式
    UPROPERTY()
    EButtonStyle CurrentStyle;

    // 音效类型
    UPROPERTY()
    EButtonSoundType CurrentSoundType;
};
