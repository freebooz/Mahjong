#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeChatAuthService.generated.h"

// 前向声明
class IHttpRequest;
class IHttpResponse;

//==============================================================================
// 微信认证结果数据结构
// 包含登录成功后的访问令牌、刷新令牌、用户标识等信息
//==============================================================================
USTRUCT(BlueprintType)
struct FWeChatAuthResult
{
    GENERATED_BODY()

    // 访问令牌，用于调用微信API
    UPROPERTY(BlueprintReadOnly)
    FString AccessToken;

    // 刷新令牌，用于刷新访问令牌
    UPROPERTY(BlueprintReadOnly)
    FString RefreshToken;

    // 令牌有效期（秒）
    UPROPERTY(BlueprintReadOnly)
    int32 ExpiresIn;

    // 用户的唯一标识
    UPROPERTY(BlueprintReadOnly)
    FString OpenID;

    // 用户唯一标识
    UPROPERTY(BlueprintReadOnly)
    FString UnionID;

    // 认证是否成功
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 错误信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;
};

//==============================================================================
// 会话信息结构
// 用于保存和恢复用户登录状态
//==============================================================================
USTRUCT(BlueprintType)
struct FSessionInfo
{
    GENERATED_BODY()

    // 会话令牌
    UPROPERTY()
    FString SessionToken;

    // 刷新令牌
    UPROPERTY()
    FString RefreshToken;

    // 会话过期时间戳
    UPROPERTY()
    int64 ExpiresAt;

    // 用户微信OpenID
    UPROPERTY()
    FString OpenID;

    // 会话是否有效
    UPROPERTY()
    bool bIsValid;
};

//==============================================================================
// 回调代理声明
//==============================================================================
DECLARE_DYNAMIC_DELEGATE_OneParam(FWeChatAuthCallback, const FWeChatAuthResult&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FWeChatTokenCallback, const FString&, Token);

//==============================================================================
// 微信认证服务
// 处理微信登录、令牌管理、会话维护等功能
//==============================================================================
UCLASS(Blueprintable)
class MAHJONGGAME_API UWeChatAuthService : public UObject
{
    GENERATED_BODY()

public:
    // 构造函数
    UWeChatAuthService();

    // 初始化认证服务
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void Initialize(const FString& AppID, const FString& AppSecret);

    // 使用微信账号登录
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void LoginWithWeChat(FWeChatAuthCallback Callback);

    // 刷新会话
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void RefreshSession(FWeChatTokenCallback Callback);

    // 获取保存的会话信息
    UFUNCTION(BlueprintPure, Category="Mahjong|WeChat")
    FSessionInfo GetSavedSession() const;

    // 保存会话信息
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void SaveSession(const FSessionInfo& Session);

    // 清除会话
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void ClearSession();

    // 检查微信是否可用
    UFUNCTION(BlueprintPure, Category="Mahjong|WeChat")
    bool IsWeChatAvailable() const { return bInitialized; }

    // 生成微信授权URL
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    FString GenerateAuthURL(const FString& RedirectURI, const FString& State);

    // 使用授权码完成登录
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat")
    void CompleteLoginWithAuthCode(const FString& AuthCode, FWeChatAuthCallback Callback);

    // 设置后端API地址
    UFUNCTION(BlueprintCallable, Category="Mahjong|WeChat|Config")
    void SetBackendURL(const FString& URL);

    // 获取后端API地址
    UFUNCTION(BlueprintPure, Category="Mahjong|WeChat|Config")
    FString GetBackendURL() const { return BackendAPIURL; }

protected:
    // 处理收到的授权码
    void OnAuthCodeReceived(const FString& AuthCode);

    // 使用授权码换取访问令牌
    void ExchangeAuthCodeForToken(const FString& AuthCode);

    // 解析令牌响应
    void ParseTokenResponse(const FString& ResponseContent);

    // 向后端注册会话
    void RegisterSessionWithBackend(const FString& WeChatAccessToken, const FString& OpenID);

    // 解析后端会话响应
    void ParseSessionResponse(const FString& ResponseContent);

    // HTTP响应处理
    void HandleBackendSessionResponse(TSharedPtr<IHttpRequest> Request, bool bSuccess);

    // UPROPERTY 标记的成员（会被 UE 垃圾回收管理）
    UPROPERTY()
    FString WeChatAppID;

    UPROPERTY()
    FString WeChatAppSecret;

    UPROPERTY()
    FSessionInfo CurrentSession;

    UPROPERTY()
    bool bInitialized;

    // 后端API基础地址
    UPROPERTY(EditAnywhere, Category="Mahjong|WeChat|Config")
    FString BackendAPIURL;

private:
    // 静态URL模板
    static const TCHAR* WeChatAuthURLTemplate;
    static const TCHAR* WeChatQRConnectURLTemplate;
};