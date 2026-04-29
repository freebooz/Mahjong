// Copyright Epic Games, Inc. All Rights Reserved.

// ============ 贵阳麻将游戏 - 模块构建配置 ============
// 本文件定义了 MahjongGame 模块的编译规则和依赖配置
// ModuleRules 控制模块如何被编译以及需要哪些依赖

using UnrealBuildTool;

public class MahjongGame : ModuleRules
{
	public MahjongGame(ReadOnlyTargetRules Target) : base(Target)
	{
		// ============ 预编译头（PCH）配置 ============
		// PCH（Precompiled Header）可以加速编译，将常用头文件预编译
		// UseExplicitOrSharedPCHs: 使用显式指定的 PCH 或共享 PCH
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// ============ 核心依赖模块 ============
		// 这些是所有目标类型（客户端、服务器、编辑器）都需要的基础模块
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",              // 核心运行时类型（FString、TArray 等）
			"CoreUObject",       // UObject 系统的基础类型
			"Engine",            // 游戏引擎核心（Actor、World、GameInstance 等）
			"Networking",        // 网络基础模块
			"Sockets",          // Socket 网络编程
			"HTTP",             // HTTP 客户端，用于 REST API 通信
			"Json",             // JSON 序列化/反序列化
			"JsonUtilities",    // JSON 工具扩展
			"AIModule"          // AI 导航和感知系统
		});

		// ============ 客户端专属依赖 ============
		// 仅在非服务器构建时添加（桌面、移动、主机平台）
		if (Target.Type != TargetType.Server)
		{
			PublicDependencyModuleNames.AddRange(new string[] {
				"InputCore",         // 输入系统（键盘、鼠标、手柄）
				"EnhancedInput",    // 增强输入系统，支持更多输入特性
				"AudioMixer",       // 音频混合器
				"UMG",              // Unreal Motion Graphics，UI 系统
				"OnlineSubsystemSteam"  // Steam 在线子系统（用于玩家匹配）
			});
		}
		else
		{
			// ============ 服务器专属依赖 ============
			// 服务器使用轻量化依赖，减少内存占用

			// OnlineSubsystem: 在线子系统接口
			// OnlineSubsystemNull: 空实现，用于服务器环境
			PublicDependencyModuleNames.AddRange(new string[] {
				"OnlineSubsystem",
				"OnlineSubsystemNull"
			});

			// 定义服务器构建标记，用于条件编译
			PublicDefinitions.Add("MAHJONG_SERVER_BUILD=1");
		}

		// ============ 私有依赖 ============
		// 私有依赖的模块不会暴露给其他模块
		// 此处为空，如有过渡依赖可在此添加
		PrivateDependencyModuleNames.AddRange(new string[] { });

		// ============ 公共包含路径 ============
		// 添加模块的 Public 目录到包含路径，允许 #include "MahjongGame/xxx.h"
		PublicIncludePaths.AddRange(new string[] {
			"MahjongGame/Public"
		});

		// ============ 服务器构建特殊优化 ============
		// 服务器构建针对体积和性能进行优化
		if (Target.Type == TargetType.Server)
		{
			// 禁用预编译头 - 服务器代码量较小，禁用 PCH 可以简化构建
			PCHUsage = PCHUsageMode.NoPCHs;

			// 禁用异常处理 - 减小二进制体积，提升性能
			// 服务器使用错误代码而非异常进行错误处理
			bEnableExceptions = false;

			// 禁用 RTTI（运行时类型信息）- 减小内存占用
			bUseRTTI = false;
		}

		// ============ 全局构建设置 ============

		// 使用最新构建设置版本
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		// ============ 平台特定优化 ============

		// Apple 平台优化 (iOS/tvOS)
		if (Target.Platform == UnrealTargetPlatform.IOS ||
			Target.Platform == UnrealTargetPlatform.TVOS)
		{
			// iOS 特殊配置
		}

		// Android 平台优化
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			// Android 特殊配置
		}

		// ============ 编辑器特定配置 ============

		if (Target.Type == TargetType.Editor)
		{
			// 编辑器构建需要 Slate UI
		}
	}
}
