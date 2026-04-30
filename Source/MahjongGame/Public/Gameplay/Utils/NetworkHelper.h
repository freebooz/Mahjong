//==============================================================================
// 麻将游戏 - 网络工具模板
// NetworkHelper.h
//
// 提供网络相关代码的模板和工具函数
// 支持：世界获取、定时器管理、Socket 创建等通用模式
//==============================================================================

#pragma once
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

//==============================================================================
// 宏定义 - 简化网络代码
//==============================================================================

// 获取世界对象的简化宏
#define GET_WORLD_TIMER_MANAGER(Object) \
    ([](UObject* InObject) -> FTimerManager* { \
        UWorld* World = GEngine->GetWorldFromContextObject(InObject, EGetWorldErrorMode::ReturnNull); \
        return World ? &World->GetTimerManager() : nullptr; \
    }(Object))

//==============================================================================
// 网络工具类
// 提供静态模板方法简化常见网络操作
//==============================================================================
class MAHJONGGAME_API FNetworkHelper
{
public:
    //============================================================================
    // 世界获取模板
    //============================================================================

    // 从任意 UObject 获取 World 指针（模板版本）
    template<typename T>
    static UWorld* GetWorldFromObject(T* Object)
    {
        if (!Object)
        {
            return nullptr;
        }
        return Object->GetWorld();
    }

    // 从 UWorld* 获取（直接返回）
    static UWorld* GetWorldFromObject(UWorld* World)
    {
        return World;
    }

    //============================================================================
    // 定时器管理模板
    //============================================================================

    // 设置延迟定时器（成员函数版本）
    template<typename T, typename RetType, typename... ArgTypes>
    static bool SetTimer(UObject* Object, RetType (T::*MemberFunction)(ArgTypes...), float Delay, bool bLooping, FTimerHandle& OutHandle, ArgTypes... Args)
    {
        UWorld* World = GEngine->GetWorldFromContextObject(Object, EGetWorldErrorMode::ReturnNull);
        if (!World)
        {
            UE_LOG(LogTemp, Warning, TEXT("[FNetworkHelper] 无法获取 World 对象"));
            return false;
        }

        FTimerDelegate Delegate = FTimerDelegate::CreateUObject(Object, MemberFunction, Args...);
        World->GetTimerManager().SetTimer(OutHandle, Delegate, Delay, bLooping);
        return true;
    }

    // 清除定时器
    template<typename T>
    static bool ClearTimer(UObject* Object, FTimerHandle& Handle)
    {
        UWorld* World = GEngine->GetWorldFromContextObject(Object, EGetWorldErrorMode::ReturnNull);
        if (!World)
        {
            return false;
        }

        World->GetTimerManager().ClearTimer(Handle);
        return true;
    }

    // 清除所有定时器
    template<typename T>
    static bool ClearAllTimers(UObject* Object)
    {
        UWorld* World = GEngine->GetWorldFromContextObject(Object, EGetWorldErrorMode::ReturnNull);
        if (!World)
        {
            return false;
        }

        World->GetTimerManager().ClearAllTimersForObject(Object);
        return true;
    }

    //============================================================================
    // Socket 创建模板
    //============================================================================

    // 创建 TCP Socket
    static FSocket* CreateTCPSocket(ISocketSubsystem* SocketSubsystem)
    {
        if (!SocketSubsystem)
        {
            SocketSubsystem = ISocketSubsystem::Get();
        }

        if (!SocketSubsystem)
        {
            UE_LOG(LogTemp, Error, TEXT("[FNetworkHelper] 无法获取 Socket 子系统"));
            return nullptr;
        }

        FSocket* Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("MahjongTCPClient"));
        if (Socket)
        {
            Socket->SetNonBlocking();
        }

        return Socket;
    }

    // 连接到地址
    static bool Connect(FSocket* Socket, const FString& Host, int32 Port)
    {
        if (!Socket)
        {
            return false;
        }

        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
        if (!SocketSubsystem)
        {
            return false;
        }

        TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();
        Addr->SetIp(*Host, Addr->GetPort(Port) == 0 ? Port : Addr->GetPort());
        Addr->SetPort(Port);

        return Socket->Connect(*Addr);
    }

    // 关闭连接
    static void CloseConnection(FSocket*& Socket)
    {
        if (Socket)
        {
            Socket->Close();
            ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
            if (SocketSubsystem)
            {
                SocketSubsystem->DestroySocket(Socket);
            }
            Socket = nullptr;
        }
    }

    //============================================================================
    // 错误处理模板
    //============================================================================

    // 获取 Socket 最后错误
    static FString GetSocketError()
    {
        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
        if (SocketSubsystem)
        {
            return SocketSubsystem->GetSocketError();
        }
        return TEXT("Unknown socket error");
    }

    // 获取最后错误代码
    static int32 GetLastErrorCode()
    {
        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
        if (SocketSubsystem)
        {
            return SocketSubsystem->GetLastErrorCode();
        }
        return -1;
    }
};

//==============================================================================
// 模板帮助类的特化版本（用于成员函数绑定）
//==============================================================================

// 定时器委托创建辅助类
template<typename T, typename RetType, typename... ArgTypes>
class FTimerDelegateHelper
{
public:
    // 创建定时器委托（成员函数版本）
    static FTimerDelegate CreateDelegate(T* Object, RetType (T::*MemberFunction)(ArgTypes...), ArgTypes... Args)
    {
        return FTimerDelegate::CreateUObject(Object, MemberFunction, Args...);
    }
};
