$content = '{
	"FileVersion": 3,
	"EngineAssociation": "{3F529181-4ABC-2F3F-ABE3-50A8681A1248}",
	"Category": "",
	"Description": "Guiyang Mahjong - Multiplayer Online Mahjong Game",
	"Modules": [
		{
			"Name": "MahjongGame",
			"Type": "Runtime",
			"LoadingPhase": "Default",
			"AdditionalDependencies": [
				"Engine",
				"CoreUObject",
				"UMG",
				"Networking",
				"Sockets"
			]
		}
	],
	"Plugins": [
		{
			"Name": "ModelingToolsEditorMode",
			"Enabled": true,
			"TargetAllowList": [
				"Editor"
			]
		},
		{
			"Name": "OnlineSubsystem",
			"Enabled": true
		},
		{
			"Name": "OnlineSubsystemSteam",
			"Enabled": true
		},
		{
			"Name": "AndroidFileServer",
			"Enabled": true
		}
	],
	"Targets": [
		{
			"Name": "MahjongGame",
			"Type": "Game",
			"LoadingPhase": "Default"
		},
		{
			"Name": "MahjongGameServer",
			"Type": "Server",
			"LoadingPhase": "Default",
			"bUsesSteam": true
		},
		{
			"Name": "MahjongGameAndroid",
			"Type": "Game",
			"LoadingPhase": "Default"
		}
	]
}'
[System.IO.File]::WriteAllText('h:\MahjongGame\MahjongGame.uproject', $content, [System.Text.Encoding]::UTF8)