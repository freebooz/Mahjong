#include "Gameplay/UI/LoginWidgetBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/MahjongGameInstance.h"
#include "Account/PlayerAccountService.h"
#include "Account/SessionService.h"
#include "Account/WeChatAuthService.h"
#include "Network/NetworkManager.h"

ULoginWidgetBase::ULoginWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void ULoginWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    if (LoginButton)
        LoginButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::OnLoginClicked);
    if (RegisterButton)
        RegisterButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::OnRegisterClicked);
    if (WeChatLoginButton)
        WeChatLoginButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::OnWeChatLoginClicked);
    if (GuestLoginButton)
        GuestLoginButton->OnClicked.AddDynamic(this, &ULoginWidgetBase::OnGuestLoginClicked);
}

void ULoginWidgetBase::OnLoginClicked()
{
    FString Username = GetUsername();
    FString Password = GetPassword();
    if (Username.IsEmpty() || Password.IsEmpty())
    {
        ShowError(TEXT("请输入用户名和密码"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("[LoginWidgetBase] Login: %s"), *Username);
    if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        UPlayerAccountService* AccountService = GI->GetPlayerAccountService();
        if (!AccountService)
        {
            AccountService = NewObject<UPlayerAccountService>();
            AccountService->InitializeService(TEXT("https://api.mahjong.game"), TEXT(""), TEXT(""));
            GI->SetPlayerAccountService(AccountService);
        }
        AccountService->Login(Username, Password);
        GI->SetPlayerName(Username);
        UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
    }
}

void ULoginWidgetBase::OnRegisterClicked()
{
    ShowError(TEXT("注册功能开发中..."));
}

void ULoginWidgetBase::OnWeChatLoginClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LoginWidgetBase] WeChat login clicked - feature coming soon"));
    ShowError(TEXT("微信登录功能开发中..."));
}

void ULoginWidgetBase::OnGuestLoginClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[LoginWidgetBase] Guest login clicked"));

    if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->SetPlayerName(TEXT("Guest_") + FGuid::NewGuid().ToString().Left(8));
        UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
    }
}

void ULoginWidgetBase::OnLoginComplete(const FLoginResult& Result)
{
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[LoginWidgetBase] Login success: %s"), *Result.Session.Nickname);
        if (UMahjongGameInstance* GI = Cast<UMahjongGameInstance>(UGameplayStatics::GetGameInstance(this)))
        {
            GI->SetPlayerName(Result.Session.Nickname);
            UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true, TEXT("listen"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[LoginWidgetBase] Login failed: %s"), *Result.ErrorMessage);
        ShowError(Result.ErrorMessage);
    }
}

void ULoginWidgetBase::ShowReconnectStatus(const FString& Status, float Progress)
{
    if (ReconnectStatusText)
    {
        ReconnectStatusText->SetText(FText::FromString(Status));
        ReconnectStatusText->SetVisibility(ESlateVisibility::Visible);
    }
    if (ReconnectProgressBar)
    {
        ReconnectProgressBar->SetPercent(Progress);
        ReconnectProgressBar->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULoginWidgetBase::ShowError(const FString& Message)
{
    if (ErrorText)
        ErrorText->SetText(FText::FromString(Message));
}

void ULoginWidgetBase::ClearInputs()
{
    if (UsernameInput)
        UsernameInput->SetText(FText::GetEmpty());
    if (PasswordInput)
        PasswordInput->SetText(FText::GetEmpty());
    if (ErrorText)
        ErrorText->SetText(FText::GetEmpty());
}

FString ULoginWidgetBase::GetUsername() const
{
    return UsernameInput ? UsernameInput->GetText().ToString() : TEXT("");
}

FString ULoginWidgetBase::GetPassword() const
{
    return PasswordInput ? PasswordInput->GetText().ToString() : TEXT("");
}