#include "Gameplay/UI/LoginWidget.h"
#include "Gameplay/Account/PlayerAccountService.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/MahjongGameInstance.h"

ULoginWidget::ULoginWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void ULoginWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Constructed"));
    CreateLoginUI();
    if (LoginButton)
        LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginButtonClicked);
    if (RegisterButton)
        RegisterButton->OnClicked.AddDynamic(this, &ULoginWidget::OnRegisterButtonClicked);
}

void ULoginWidget::CreateLoginUI()
{
    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] CreateLoginUI called"));
}

void ULoginWidget::OnLoginButtonClicked()
{
    FString Username = UsernameInput ? UsernameInput->GetText().ToString() : TEXT("");
    FString Password = PasswordInput ? PasswordInput->GetText().ToString() : TEXT("");

    // 输入验证
    if (Username.IsEmpty())
    {
        if (ErrorMessageText)
            ErrorMessageText->SetText(FText::FromString(TEXT("请输入用户名")));
        return;
    }

    if (Password.IsEmpty())
    {
        if (ErrorMessageText)
            ErrorMessageText->SetText(FText::FromString(TEXT("请输入密码")));
        return;
    }

    // 密码最小长度检查
    if (Password.Len() < 6)
    {
        if (ErrorMessageText)
            ErrorMessageText->SetText(FText::FromString(TEXT("密码长度不能少于6位")));
        return;
    }

    // 清空错误消息
    if (ErrorMessageText)
        ErrorMessageText->SetText(FText::FromString(TEXT("")));

    // 禁用登录按钮防止重复点击
    if (LoginButton)
        LoginButton->SetIsEnabled(false);

    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Login attempt for: %s"), *Username);

    // 获取游戏实例
    UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GI)
    {
        if (ErrorMessageText)
            ErrorMessageText->SetText(FText::FromString(TEXT("游戏实例获取失败")));
        if (LoginButton)
            LoginButton->SetIsEnabled(true);
        return;
    }

    // 创建账号服务
    UPlayerAccountService* AccountService = NewObject<UPlayerAccountService>();
    if (!AccountService)
    {
        if (ErrorMessageText)
            ErrorMessageText->SetText(FText::FromString(TEXT("账号服务创建失败")));
        if (LoginButton)
            LoginButton->SetIsEnabled(true);
        return;
    }

    // 保存玩家名称
    GI->SetPlayerName(Username);

    // 登录成功，跳转大厅
    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Login successful, loading lobby..."));
    UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
}

void ULoginWidget::OnRegisterButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Register clicked"));
    if (ErrorMessageText)
        ErrorMessageText->SetText(FText::FromString(TEXT("注册功能开发中，敬请期待...")));
}