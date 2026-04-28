# 贵阳麻将 - 蓝图实现指南

## 概述

本文档定义贵阳捉鸡麻将游戏的蓝图类实现规范，包括UI组件、关卡配置和游戏流程。

---

## 一、蓝图类列表

### UI Widget 蓝图

| 蓝图类名 | 父类 | 路径 | 说明 |
|---------|------|------|------|
| WBP_Login | LoginWidgetBase | /Game/Mahjong/Blueprint/UI/ | 登录界面 |
| WBP_Lobby | LobbyWidgetBase | /Game/Mahjong/Blueprint/UI/ | 大厅界面 |
| WBP_Room | RoomWidgetBase | /Game/Mahjong/Blueprint/UI/ | 房间界面 |
| WBP_GameHUD | HUDWidgetBase | /Game/Mahjong/Blueprint/UI/ | 游戏HUD |
| WBP_GameWidget | MahjongGameWidget | /Game/Mahjong/Blueprint/UI/ | 游戏主界面 |
| WBP_RoomListEntry | UserWidget | /Game/Mahjong/Blueprint/UI/ | 房间列表项 |
| WBP_Settings | SettingsWidgetBase | /Game/Mahjong/Blueprint/UI/ | 设置界面 |

### 游戏模式蓝图

| 蓝图类名 | 父类 | 路径 | 说明 |
|---------|------|------|------|
| BP_LoginGameMode | GameModeBase | /Game/Mahjong/Blueprint/Modes/ | 登录模式 |
| BP_LobbyGameMode | GameModeBase | /Game/Mahjong/Blueprint/Modes/ | 大厅模式 |
| BP_RoomGameMode | GameModeBase | /Game/Mahjong/Blueprint/Modes/ | 房间模式 |
| BP_GuiyangGameMode | AMahjongGameMode | /Game/Mahjong/Blueprint/Modes/ | 贵阳游戏模式 |

### 玩家控制器蓝图

| 蓝图类名 | 父类 | 路径 | 说明 |
|---------|------|------|------|
| BP_LoginPlayerController | PlayerController | /Game/Mahjong/Blueprint/Game/ | 登录控制器 |
| BP_LobbyPlayerController | PlayerController | /Game/Mahjong/Blueprint/Game/ | 大厅控制器 |
| BP_RoomPlayerController | PlayerController | /Game/Mahjong/Blueprint/Game/ | 房间控制器 |
| BP_GuiyangPlayerController | AMahjongPlayerController | /Game/Mahjong/Blueprint/Game/ | 贵阳游戏控制器 |

### 玩家状态蓝图

| 蓝图类名 | 父类 | 路径 | 说明 |
|---------|------|------|------|
| BP_GuiyangPlayerState | AMahjongPlayerStateBase | /Game/Mahjong/Blueprint/PlayerState/ | 贵阳玩家状态 |

### 游戏状态蓝图

| 蓝图类名 | 父类 | 路径 | 说明 |
|---------|------|------|------|
| BP_GuiyangGameState | AMahjongGameStateBase | /Game/Mahjong/Blueprint/GameState/ | 贵阳游戏状态 |

### 规则蓝图

| 蓝图类名 | 父类 | 路径 | 说明 |
|---------|------|------|------|
| BP_GuiyangGameRules | UBP_GuiyangGameRules | /Game/Mahjong/Blueprint/Rules/ | 贵阳规则 |

---

## 二、游戏流程

```
[Login] --> [Lobby] --> [Room] --> [MahjongTable]
    |           |          |
    v           v          v
LoginGameMode LobbyGameMode RoomGameMode GuiyangGameMode
```

### 2.1 登录流程 (Login Level)

```
Login Level
├── GameMode: BP_LoginGameMode
├── PlayerController: BP_LoginPlayerController
├── HUD Widget: WBP_Login
└── WidgetTree:
    └── RootCanvas
        ├── BackgroundImage
        ├── LoginPanel (VerticalBox)
        │   ├── TitleText
        │   ├── UsernameInput (EditableTextBox)
        │   ├── PasswordInput (EditableTextBox, Password)
        │   ├── ErrorText
        │   └── ButtonBox (HorizontalBox)
        │       ├── LoginButton
        │       └── RegisterButton
        └── VersionText
```

**事件流程:**
1. 用户输入用户名密码
2. 点击登录按钮
3. 调用 `LoginPlayerName(Username)` 在 GameInstance
4. 打开 Lobby 关卡

### 2.2 大厅流程 (Lobby Level)

```
Lobby Level
├── GameMode: BP_LobbyGameMode
├── PlayerController: BP_LobbyPlayerController
├── HUD Widget: WBP_Lobby
└── WidgetTree:
    └── RootCanvas
        ├── BackgroundImage
        ├── TopBar
        │   ├── PlayerInfoBox (PlayerName, Coins)
        │   ├── SettingsButton
        │   └── LogoutButton
        └── MainContent
            ├── TitleText
            ├── RoomListSection
            │   ├── RoomListHeader (Title, RefreshButton)
            │   └── RoomListView (ListView, item: WBP_RoomListEntry)
            └── ButtonBox
                ├── CreateRoomButton
                ├── JoinRoomButton
                └── QuickStartButton
```

**事件流程:**
1. 进入大厅，自动刷新房间列表
2. 点击房间项调用 `OnJoinRoomClicked(RoomID)`
3. 点击创建房间调用 `OnCreateRoomClicked()`
4. 点击快速开始直接进入 Room 关卡

### 2.3 房间流程 (Room Level)

```
Room Level
├── GameMode: BP_RoomGameMode
├── PlayerController: BP_RoomPlayerController
├── HUD Widget: WBP_Room
└── WidgetTree:
    └── RootCanvas
        ├── TableBackground
        ├── TopBar (RoomInfo, PlayerCount, LeaveButton)
        ├── PlayerSeats (4个玩家位置)
        ├── GameArea
        │   ├── DiscardPool (WrapBox)
        │   ├── DrawButton
        │   └── DiscardButton
        ├── ActionButtons (Peng, Gang, Hu, Guo)
        └── StatusText
```

**事件流程:**
1. 等待玩家加入（房主可开始）
2. 房主点击开始游戏 -> 打开 MahjongTable 关卡
3. 非房主等待，收到服务器切换关卡通知

### 2.4 游戏流程 (MahjongTable Level)

```
MahjongTable Level
├── GameMode: BP_GuiyangGameMode
├── PlayerController: BP_GuiyangPlayerController
├── HUD Widget: WBP_GameHUD
├── Game Widget: WBP_GameWidget
└── WidgetSwitcher (MainSwitcher):
    ├── WaitingPanel
    ├── PlayingPanel
    └── ResultPanel

PlayingPanel WidgetTree:
└── CanvasPanel
    ├── TopHUD (TurnInfo, TileCount, FanPaiJi)
    ├── PlayerArea
    │   ├── OpponentHands (3个对手)
    │   ├── SidePlayers (左右两侧)
    │   └── SelfHand (自己的手牌)
    ├── DiscardPoolArea
    ├── ActionButtons (Draw, Discard, Peng, Gang, Hu, Guo, Ting)
    └── AlertImage
```

---

## 三、蓝图创建步骤

### 3.1 创建UI Widget蓝图

1. **创建蓝图类**
   - 右键 Content Browser
   - 选择 `Blueprint Class`
   - 搜索父类（如 `LoginWidgetBase`）
   - 命名为 `WBP_Login`
   - 保存到 `/Game/Mahjong/Blueprint/UI/`

2. **设置Widget Tree**
   - 打开蓝图编辑器
   - 在 Designer 面板中构建UI层次结构
   - 使用 `BindWidget` 绑定可交互组件

3. **实现Event Graph**
   - 切换到 Graph 面板
   - 实现事件处理逻辑
   - 连接UI事件到游戏逻辑

### 3.2 创建GameMode蓝图

1. **创建蓝图类**
   - 选择父类 `GameModeBase` 或 `AMahjongGameMode`
   - 命名为 `BP_GuiyangGameMode`
   - 保存到 `/Game/Mahjong/Blueprint/Modes/`

2. **设置类默认值**
   ```
   Players Count: 4
   HUD Widget Class: WBP_GameHUD
   Game Widget Class: WBP_GameWidget
   Game Rules Class: BP_GuiyangGameRules
   ```

3. **实现游戏逻辑**
   - 实现 `StartGame()` - 开始发牌
   - 实现 `EndGame()` - 结算
   - 实现 `CheckGameOver()` - 检查结束条件

### 3.3 创建PlayerController蓝图

1. **创建蓝图类**
   - 选择父类 `PlayerController` 或 `AMahjongPlayerController`
   - 命名为 `BP_GuiyangPlayerController`
   - 保存到 `/Game/Mahjong/Blueprint/Game/`

2. **实现RPC函数**
   ```
   Server_DrawTile()     - 摸牌
   Server_DiscardTile() - 出牌
   Server_Peng()         - 碰牌
   Server_Gang()         - 杠牌
   Server_Hu()           - 胡牌
   ```

### 3.4 创建PlayerState蓝图

1. **创建蓝图类**
   - 选择父类 `AMahjongPlayerStateBase`
   - 命名为 `BP_GuiyangPlayerState`
   - 保存到 `/Game/Mahjong/Blueprint/PlayerState/`

2. **添加游戏特定属性**
   ```
   ChickenCount: int        - 鸡牌数量
   DouCount: int          - 豆数量
   TotalScore: int        - 总分数
   ```

---

## 四、资源配置

### 4.1 关卡资源

| 关卡名 | 类型 | GameMode | HUD |
|--------|------|----------|-----|
| Login | Primary | BP_LoginGameMode | WBP_Login |
| Lobby | Primary | BP_LobbyGameMode | WBP_Lobby |
| Room | Primary | BP_RoomGameMode | WBP_Room |
| MahjongTable | Gameplay | BP_GuiyangGameMode | WBP_GameHUD + WBP_GameWidget |

### 4.2 世界场景设置

在每个关卡的 `World Settings` 中配置:

```
GameMode Override: [对应GameMode]
Player Controller Class: [对应Controller]
Default Pawn Class: None
```

---

## 五、蓝图间通信

### 5.1 Widget -> PlayerController

```Blueprint
// WBP_GameWidget 中
Event DrawButtonClicked:
  -> Get PlayerController
  -> Cast to BP_GuiyangPlayerController
  -> Call Server_DrawTile()
```

### 5.2 PlayerController -> GameState

```Blueprint
// BP_GuiyangPlayerController 中
Event Server_DrawTile:
  -> Get GameState
  -> Cast to BP_GuiyangGameState
  -> Call DrawTile(PlayerIndex)
  -> Multicast_UpdateHandTiles()
```

### 5.3 GameState -> Widget

```Blueprint
// BP_GuiyangGameState 中
Event OnTileDrawn (Multicast):
  -> Get All Player Controllers
  -> For Each Controller:
    -> Get HUD Widget
    -> Call UpdateHandTiles(TileIDs)
```

---

## 六、网络同步

### 6.1 服务器权威

```
Client -> Server: RPC调用 (Server_xxx)
Server -> Client: Multicast或Client RPC
```

### 6.2 复制属性

```cpp
// 在 GameState 中使用 DOREPLIFETIME
DOREPLIFETIME(AGuiyangGameState, CurrentTurnPlayer);
DOREPLIFETIME(AGuiyangGameState, TilePoolCount);
DOREPLIFETIME(AGuiyangGameState, GamePhase);
```

---

## 七、常用参考

### 7.1 文件路径

| 资源类型 | Content路径 |
|---------|------------|
| UI Widget | /Game/Mahjong/Blueprint/UI/ |
| Game Modes | /Game/Mahjong/Blueprint/Modes/ |
| Player Controllers | /Game/Mahjong/Blueprint/Game/ |
| Player States | /Game/Mahjong/Blueprint/PlayerState/ |
| Game States | /Game/Mahjong/Blueprint/GameState/ |
| Rules | /Game/Mahjong/Blueprint/Rules/ |

### 7.2 配置文件

| 文件 | 说明 |
|------|------|
| LevelConfig.json | 关卡和游戏模式配置 |
| WBP_Login.json | 登录界面UI规范 |
| WBP_GameWidget.json | 游戏主界面UI规范 |

---

## 八、创建清单

### Phase 1: 基础UI
- [ ] 创建 WBP_Login
- [ ] 创建 WBP_Lobby
- [ ] 创建 WBP_Room
- [ ] 创建 WBP_RoomListEntry

### Phase 2: 游戏UI
- [ ] 创建 WBP_GameHUD
- [ ] 创建 WBP_GameWidget
- [ ] 创建 WBP_Settings

### Phase 3: 游戏模式
- [ ] 创建 BP_LoginGameMode
- [ ] 创建 BP_LobbyGameMode
- [ ] 创建 BP_RoomGameMode
- [ ] 创建 BP_GuiyangGameMode

### Phase 4: 控制器
- [ ] 创建 BP_LoginPlayerController
- [ ] 创建 BP_LobbyPlayerController
- [ ] 创建 BP_RoomPlayerController
- [ ] 创建 BP_GuiyangPlayerController

### Phase 5: 状态和规则
- [ ] 创建 BP_GuiyangPlayerState
- [ ] 创建 BP_GuiyangGameState
- [ ] 创建 BP_GuiyangGameRules

### Phase 6: 关卡
- [ ] 创建 Login 关卡
- [ ] 创建 Lobby 关卡
- [ ] 创建 Room 关卡
- [ ] 创建 MahjongTable 关卡
