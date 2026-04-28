#include "Gameplay/UI/LoginWidget.h"
#include "Account/PlayerAccountService.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MahjongGameInstance.h"
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
    if (Username.IsEmpty() || Password.IsEmpty())
    {
        if (ErrorMessageText)
            ErrorMessageText->SetText(FText::FromString(TEXT("请输入用户名和密码")));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Login attempt for: %s"), *Username);
    if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        UPlayerAccountService* AccountService = NewObject<UPlayerAccountService>();
        AccountService->Login(Username, Password);
        GI->SetPlayerName(Username);
        UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Login successful, loading lobby..."));
        UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
    }
    else
    {
        if (ErrorMessageText)
            ErrorMessageText->SetText(FText::FromString(TEXT("游戏实例获取失败")));
    }
}
void ULoginWidget::OnRegisterButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Register clicked"));
    if (ErrorMessageText)
        ErrorMessageText->SetText(FText::FromString(TEXT("注册功能开发中...")));
}