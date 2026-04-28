// Copyright Epic Games, Inc. All Rights Reserved.

// ============ 贵阳麻将游戏 - Android 目标构建配置 ============
// 本文件定义了 Android 移动平台的构建设置，用于构建可以在 Android 设备上运行的游戏客户端。

using UnrealBuildTool;
using System.Collections.Generic;

public class MahjongGameAndroidTarget : TargetRules
{
	public MahjongGameAndroidTarget(TargetInfo Target) : base(Target)
	{
		// ============ 基础配置 ============
		// 指定此目标为游戏类型（可执行程序）
		// Android 平台的游戏构建包含完整的游戏运行时
		Type = TargetType.Game;

		// 使用最新版本的构建设置
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		// 使用最新的包含顺序版本
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		// 添加游戏主模块
		ExtraModuleNames.Add("MahjongGame");

		// ============ Android 平台配置 ============

		// Android NDK r25b+ 优化配置

		// 使用 LLVM ThinLTO（Link Time Optimization）链接时优化
		// ThinLTO 可以显著减小 APK 体积并提升运行时性能
		// 适用于 Android NDK r25b 及以上版本
		bUseLLVMThinLTO = true;

		// 编译 AES 加密模块
		// 用于网络通信加密，保护玩家数据安全
		bCompileAES = true;

		// ============ Android 构建优化 ============

		// 启用统一构建模式
		// 统一构建将多个源文件合并编译，减少重复编译，提升构建速度
		bUseUnityBuild = true;

		// 自适应回溯生成
		// 根据代码变化自动调整回溯信息生成策略，优化编译时间
		bAdaptiveUnityBacktraceGeneration = true;

		// ============ Android 目标架构 ============

		// 指定 Android 目标架构为 arm64-v8a（64位 ARM）
		// 现代 Android 设备大多数都支持 64 位架构
		// 64 位架构提供更好的性能和更大的内存寻址空间
		AndroidPlatformArchitecture = "arm64-v8a";

		// ============ APK 体积优化 ============

		// 隐藏符号可见性
		// 只导出必要的符号，减少 APK 中的符号表大小
		bBuildWithHiddenSymbolVisibility = true;

		// 禁用异常处理子系统
		// 移动平台资源有限，禁用异常处理可以减小体积并提升性能
		// 注意：这意味着不能使用 C++ 异常，必须使用错误代码处理错误
		bUseExceptionSubsystem = false;

		// ============ 移动平台特定配置 ============

		// 禁用不必要的功能以减小 APK 体积

		// 移动平台不需要推送模型功能
		bWithPushModel = false;

		// 禁用网络推送模型驱动
		bWithNetPushModelDrivers = false;

		// 正式环境禁用检查，提升性能
		bUseChecksInShipping = false;

		// 正式环境禁用日志，减少资源消耗
		bUseLoggingInShipping = false;

		// 设置为发布版本构建
		Build = UnrealTargetBuild.Shipping;
	}
}