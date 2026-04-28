#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeChatAuthService.generated.h"

USTRUCT(BlueprintType)
struct FWeChatAuthResult
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadOnly)
    FString AccessToken;
    UPROPERTY(BlueprintReadOnly)
    FString RefreshToken;
    UPROPERTY(BlueprintReadOnly)
    int32 ExpiresIn;
    UPROPERTY(BlueprintReadOnly)
    FString OpenID;
    UPROPERTY(BlueprintReadOnly)
    FString UnionID;
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;
};

USTRUCT(BlueprintType)
struct FSessionInfo
{
    GENERATED_BODY()
    UPROPERTY()
    FString SessionToken;
    UPROPERTY()
    FString RefreshToken;
    UPROPERTY()
    int64 ExpiresAt;
    UPROPERTY()
    FString OpenID;
    UPROPERTY()
    bool bIsValid;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FWeChatAuthCallback, const FWeChatAuthResult&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FWeChatTokenCallback, const FString&, Token);

UCLASS(Blueprintable)
class MAHJONGGAME_API UWeChatAuthService : public UObject
{
    GENERATED_BODY()
public:
    UWeChatAuthService();

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void Initialize(const FString& AppID, const FString& AppSecret);

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void LoginWithWeChat(FWeChatAuthCallback Callback);

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void RefreshSession(FWeChatTokenCallback Callback);

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    FSessionInfo GetSavedSession() const;

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void SaveSession(const FSessionInfo& Session);

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void ClearSession();

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    bool IsWeChatAvailable() const;

    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    FString GenerateAuthURL(const FString& RedirectURI, const FString& State);

protected:
    void OnAuthCodeReceived(const FString& AuthCode);
    void ExchangeAuthCodeForToken(const FString& AuthCode);
    void ParseTokenResponse(const FString& ResponseContent);

    UPROPERTY()
    FString WeChatAppID;

    UPROPERTY()
    FString WeChatAppSecret;

    UPROPERTY()
    FSessionInfo CurrentSession;

    UPROPERTY()
    bool bInitialized;

    FWeChatAuthCallback PendingCallback;

private:
    // 静态URL模板，避免每次调用时重新解析
    static const TCHAR* WeChatAuthURLTemplate;
};