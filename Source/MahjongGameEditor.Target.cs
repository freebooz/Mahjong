// Copyright Epic Games, Inc. All Rights Reserved.

// ============ 贵阳麻将游戏 - 编辑器目标构建配置 ============
// 本文件定义了编辑器目标的构建设置，用于在 Unreal Editor 中运行和调试游戏。
// 编辑器构建包含完整的开发工具和运行时功能。

using UnrealBuildTool;
using System.Collections.Generic;

public class MahjongGameEditorTarget : TargetRules
{
	public MahjongGameEditorTarget( TargetInfo Target) : base(Target)
	{
		// ============ 基础配置 ============
		// 指定此目标为编辑器类型
		// 编辑器构建包含完整的引擎编辑功能、蓝图可视化工具和运行时调试功能
		Type = TargetType.Editor;

		// 使用最新版本的构建设置
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		// 使用最新的包含顺序版本
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		// 添加游戏主模块
		ExtraModuleNames.Add("MahjongGame");

		// ============ 编辑器专属配置 ============

		// 不作为插件构建 - 这是主游戏项目，不是插件
		bBuildPluginEngine = false;

		// 需要编辑器功能 - 编辑器构建包含完整的编辑工具链
		bBuildRequiresEditorFeatures = true;

		// 不包含在功能层中 - 功能层用于模块化组织，此项目不需要
		bIncludeInFeatureLayer = false;

		// ============ 编辑器 UI 框架配置 ============

		// 使用 Slate UI 框架 - Unreal Editor 的 UI 系统
		// Slate 用于编辑器界面、窗口布局和控件渲染
		bUsesSlate = true;

		// 支持编辑器工具 - 允许使用编辑器实用程序和自定义编辑器功能
		bSupportsEditorUtilities = true;

		// ============ 编辑器构建优化 ============

		// 编辑器构建启用详细日志，便于调试
		// 日志对于开发过程非常重要，不需要在编辑器构建中禁用
		bUseLoggingInShipping = true;

		// 编辑器构建保留运行时检查，帮助发现代码问题
		bUseChecksInShipping = true;

		// 允许热重载 - 支持在编辑器运行时重新加载 C++ 代码
		bAllowHotReload = true;

		// 启用实时Coding - 支持在编辑器中编辑代码并立即看到效果
		bBuildDeveloperTools = true;
	}
}
