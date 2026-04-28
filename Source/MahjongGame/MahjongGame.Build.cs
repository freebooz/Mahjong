// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MahjongGame : ModuleRules
{
	public MahjongGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Core dependencies - always needed
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"Networking",
			"Sockets",
			"HTTP",
			"Json",
			"JsonUtilities",
			"AIModule"
		});

		// Client-only dependencies (excluded for server builds)
		if (Target.Type != TargetType.Server)
		{
			PublicDependencyModuleNames.AddRange(new string[] {
				"InputCore",
				"EnhancedInput",
				"AudioMixer",
				"UMG",
				"OnlineSubsystemSteam"
			});
		}
		else
		{
			// Server-only minimal dependencies
			PublicDependencyModuleNames.AddRange(new string[] {
				"OnlineSubsystem",
				"OnlineSubsystemNull"
			});
			PublicDefinitions.Add("MAHJONG_SERVER_BUILD=1");
		}

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MahjongGame/Public"
		});

		// Disable PCH for faster iteration on server builds
		if (Target.Type == TargetType.Server)
		{
			PCHUsage = PCHUsageMode.NoPCHs;
			bEnableExceptions = false;
			bUseRTTI = false;
		}
	}
}
