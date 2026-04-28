// Copyright Epic Games, Inc. All Rights Reserved.

// ============ 贵阳麻将游戏 - 服务器目标构建配置 ============
// 本文件定义了独立服务器目标的构建设置，用于构建可以在 Linux 服务器上运行的专用服务器程序。
// 独立服务器不包含渲染功能，专门用于处理游戏逻辑和玩家连接。

using UnrealBuildTool;
using System.Collections.Generic;

public class MahjongGameServerTarget : TargetRules
{
	public MahjongGameServerTarget(TargetInfo Target) : base(Target)
	{
		// ============ 基础配置 ============
		// 指定此目标为独立服务器类型
		// 服务器构建会排除所有渲染相关的模块，减小可执行文件体积
		Type = TargetType.Server;

		// 使用最新版本的构建设置，确保使用最新的编译器和优化选项
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		// 使用最新的包含顺序版本，避免因引擎头文件顺序变化导致的编译错误
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		// 添加游戏主模块，服务器需要包含所有游戏逻辑
		ExtraModuleNames.Add("MahjongGame");

		// ============ 服务器专属优化配置 ============

		// Linux 服务器交叉编译支持
		// 用于 AWS、Google Cloud、阿里云等 Linux 服务器环境
		// Linux 是最常用的服务器操作系统，提供更好的稳定性和性能
		if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			// 标记为 CEServer 构建，启用服务器特定的优化
			bIsBuildingForCEServer = true;
		}

		// 服务器构建优化：不包含编辑器内容
		// 服务器不需要编辑器功能，禁用可以减少二进制体积
		bBuildInsideEditor = false;

		// ============ 网络和复制优化 ============
		// 以下选项禁用不必要的网络功能，进一步减小服务器体积和内存占用

		// 禁用推送模型功能 - 服务器不需要主动推送数据给客户端
		bWithPushModel = false;

		// 禁用网络推送模型驱动 - 减少网络开销
		bWithNetPushModelDrivers = false;

		// ============ 正式环境优化 ============
		// 正式环境（Shipping）构建配置，禁用调试和日志功能以提升性能

		// 禁用运行时检查（如断言验证），减少 CPU 开销
		// 仅在开发环境保留检查功能
		bUseChecksInShipping = false;

		// 禁用日志输出，避免文件 I/O 带来的性能损失
		// 服务器日志应通过专用日志系统处理
		bUseLoggingInShipping = false;

		// 设置为正式发布版本构建，优化二进制文件
		Build = UnrealTargetBuild.Shipping;

		// ============ 服务器配置 ============
		// 禁用不必要的模块以减小服务器体积

		// 服务器不需要 Steam 子系统（Steam 用于玩家匹配，服务器使用自定义认证）
		// 这不是禁用 Steam 功能，而是服务器不需要 Steam 的服务器端库
		bUsesSteam = false;
	}
}
