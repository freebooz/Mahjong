#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SessionService.generated.h"

// 前向声明
class IHttpRequest;
class IHttpResponse;

//==============================================================================
// 用户会话数据结构
// 存储玩家的登录会话信息
//==============================================================================
USTRUCT(BlueprintType)
struct FUserSession
{
    GENERATED_BODY()

    // 会话令牌
    UPROPERTY()
    FString SessionToken;

    // 微信 OpenID（用于关联微信账号）
    UPROPERTY()
    FString OpenID;

    // 玩家昵称
    UPROPERTY()
    FString Nickname;

    // 玩家等级
    UPROPERTY()
    int32 PlayerLevel;

    // 登录时间（Unix 时间戳）
    UPROPERTY()
    int64 LoginTime;

    // 会话过期时间（Unix 时间戳）
    UPROPERTY()
    int64 ExpiresAt;

    // 是否为访客会话
    UPROPERTY()
    bool bIsGuest;
};

//==============================================================================
// 会话登录回调代理
// 参数：Session - 登录后的用户会话信息
//==============================================================================
DECLARE_DYNAMIC_DELEGATE_OneParam(FSessionLoginCallback, const FUserSession&, Session);

//==============================================================================
// 会话服务
// 负责会话管理、验证、持久化等功能
//==============================================================================
UCLASS(Blueprintable)
class MAHJONGGAME_API USessionService : public UObject
{
    GENERATED_BODY()

public:
    // 构造函数
    USessionService();

    // 初始化会话服务
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void Initialize(const FString& InServerURL);

    // 使用 Token 登录
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void LoginWithToken(const FString& SessionToken, FSessionLoginCallback Callback);

    // 访客登录
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void LoginAsGuest(FSessionLoginCallback Callback);

    // 获取当前会话
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    FUserSession GetCurrentSession() const;

    // 检查会话是否有效
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    bool IsSessionValid() const;

    // 更新会话信息
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void UpdateSession(const FUserSession& Session);

    // 清除会话
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void ClearSession();

    // 保存会话到磁盘
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    bool SaveSessionToDisk();

    // 从磁盘加载会话
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    bool LoadSessionFromDisk();

    // 在服务器上验证会话
    UFUNCTION(BlueprintCallable, Category="Mahjong|Session")
    void ValidateSessionOnServer(const FString& Token, FSessionLoginCallback Callback);

protected:
    // 处理服务器验证响应
    void OnValidateResponseReceived(TSharedPtr<IHttpRequest> Request, bool bSuccess);

    // 解析会话响应
    void ParseSessionResponse(const FString& ResponseContent);

    // UPROPERTY 标记的成员
    UPROPERTY()
    FUserSession CurrentSession;

    // 服务器地址
    UPROPERTY()
    FString ServerURL;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;
};