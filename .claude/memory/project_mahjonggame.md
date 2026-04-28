---
name: project_mahjonggame
description: UE5.7 Mahjong game project with FastAPI backend - multiplayer online mahjong
type: project
---

# MahjongGame Project Context

**Project Path**: `e:/poject/feebooz/MahjongGame/MahjongGame`
**Engine**: Unreal Engine 5.7
**Backend**: FastAPI + SQLAlchemy + SQLite (path: `h:/backend`)

## Project Type
Unreal Engine 5 multiplayer online mahjong game (Guiyang Mahjong)

## Architecture
- **Presentation Layer**: Widgets, GameMode, PlayerController
- **Business Logic Layer**: MahjongAIController, MahjongGameRules
- **Data Layer**: NetworkManager, AccountService, AI Service
- **Backend Service**: FastAPI + SQLAlchemy + SQLite

## Key Technologies
- OnlineSubsystemSteam enabled
- AndroidFileServer enabled
- Targets: Game, Server, Android

## Known Issues (from Analysis_Report.md 2026-04-28)
1. **High Priority**: MahjongAIControllerTest needs UE5-compatible reimplementation
2. **Medium Priority**: FTimerManager* pointer lifecycle (NetworkManager.h:90)
3. **Medium Priority**: LoginWidget/LoginWidgetBase parallel classes causing confusion

## Recent Fixes (session 2026-04-28)
- WeChatAuthService.cpp FString::Format() fix
- MahjongAIController.h duplicate member variable fix
- MahjongGameWidget.h missing HandTiles/DiscardedTiles fix
- LoginGameModeBase.cpp and RoomGameModeBase.cpp ShowUIWidget type cast fixes

## Module Organization
- Core: 8 files
- Gameplay: 35+ files
- Account: 4 files
- Monitor: 6 files
- Utils: 2 files

## Design Patterns
- Singleton: MonitorManager
- Strategy: AIBrain (aggressive/defensive/balanced)
- Observer: Delegate/Callback