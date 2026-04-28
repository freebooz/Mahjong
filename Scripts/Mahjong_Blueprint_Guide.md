# 贵阳捉鸡麻将 Blueprint 创建指南

## C++ 基类文件列表

### 枚举和结构体
```
Source/MahjongGame/Public/Game/MahjongEnums.h
  - ETileSuit (万/筒/条)
  - ETileValue (1-9)
  - EChickenType (幺鸡/翻牌鸡/冲锋鸡/责任鸡/金鸡)
  - EDouType (闷豆/转弯豆/点豆/抢杠)
  - EHandPattern (平胡/大对子/七对/龙七对/清一色等)
  - EMahjongAction (碰/杠/胡/自摸/过)
  - EGamePhase (等待/发牌/游戏中/摸牌/出牌等)
  - EPlayerState
  - 苏醒信息/回合结果结构体

Source/MahjongGame/Public/Game/MahjongTile.h
  - FMahjongTile (单张牌数据)
  - FMahjongHand (手牌数据)
  - FMahjongPlayerData (玩家数据)
  - FMahjongGameSettings (游戏设置)
```

### 核心游戏类
```
Source/MahjongGame/Public/Game/MahjongGameLogic.h/cpp
  - UMahjongGameLogic (游戏逻辑：洗牌/发牌/胡牌判断/番数计算)

Source/MahjongGame/Public/Game/MahjongGameState.h/cpp
  - AMahjongGameState (游戏状态管理)

Source/MahjongGame/Public/Game/MahjongPlayerState.h/cpp
  - AMahjongPlayerState (玩家状态管理)

Source/MahjongGame/Public/Game/MahjongPlayerController.h/cpp
  - AMahjongPlayerController (玩家控制器)
```

### UI 类
```
Source/MahjongGame/Public/UI/MahjongGameWidget.h/cpp
  - UMahjongGameWidget (游戏主界面)
```

## Blueprint 创建步骤

### 1. 创建 BP_MahjongGameWidget

1. Content Browser 右键 → Create Advanced Asset → Blueprints → Blueprint Class
2. 搜索 `MahjongGameWidget` 作为父类
3. 命名为 `BP_MahjongGameWidget`
4. 在 Designer 中添加以下 Widget 并绑定：

| 变量名 | 类型 | 说明 |
|--------|------|------|
| MainSwitcher | WidgetSwitcher | 界面切换器 |
| HandTilesPanel | Widget | 手牌面板 |
| HandTilesGrid | UniformGridPanel | 手牌网格 |
| DiscardPoolScroll | ScrollBox | 弃牌池滚动框 |
| DiscardPoolGrid | UniformGridPanel | 弃牌池网格 |
| DrawButton | Button | 摸牌按钮 |
| DiscardButton | Button | 出牌按钮 |
| PengButton | Button | 碰按钮 |
| GangButton | Button | 杠按钮 |
| HuButton | Button | 胡按钮 |
| GuoButton | Button | 过按钮 |
| TingButton | Button | 报听按钮 |
| TurnText | TextBlock | 回合指示文本 |
| TileCountText | TextBlock | 牌数文本 |
| WaitingText | TextBlock | 等待文本 |
| CountdownText | TextBlock | 倒计时文本 |
| FanPaiJiText | TextBlock | 翻牌鸡文本 |
| AlertImage | Image | 警告图片 |

### 2. 创建 BP_MahjongPlayerController

1. 创建 Blueprint 继承 `PlayerController`
2. 命名为 `BP_MahjongPlayerController`
3. 设置类默认值：

| 属性 | 值 |
|------|-----|
| GameWidgetClass | BP_MahjongGameWidget |
| HUDWidgetClass | BP_HUDWidget |

### 3. 创建 BP_MahjongGameState

1. 创建 Blueprint 继承 `GameStateBase`
2. 命名为 `BP_MahjongGameState`

### 4. 创建 BP_MahjongPlayerState

1. 创建 Blueprint 继承 `PlayerState`
2. 命名为 `BP_MahjongPlayerState`

## UI 交互流程

### 游戏主界面布局 (BP_MahjongGameWidget)

```
┌─────────────────────────────────────────────────┐
│  [庄家信息]           牌数: XXX      [设置按钮] │
├─────────────────────────────────────────────────┤
│                                                 │
│              弃牌池区域                          │
│         ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐                    │
│         └─┘ └─┘ └─┘ └─┘ └─┘                    │
│                                                 │
├─────────────────────────────────────────────────┤
│   [玩家2]        [玩家3]        [玩家4]          │
│                                                 │
├─────────────────────────────────────────────────┤
│               你的回合: 第X圈                    │
│                                                 │
│   ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ │
│   │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │
│   └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ │
│          [摸牌] [出牌]                          │
│   [碰] [杠] [胡] [过] [报听]                     │
└─────────────────────────────────────────────────┘
```

## 游戏流程状态机

```
┌──────────┐
│ Waiting  │ ← 游戏等待开始
└────┬─────┘
     │ StartGame
     ▼
┌──────────┐
│   Deal   │ ← 发牌阶段
└────┬─────┘
     │ DealCards
     ▼
┌──────────┐
│ Playing  │ ← 游戏进行中
└────┬─────┘
     │
     ├──→ [Draw] DrawTile ←─────────────────┐
     │    │                                │
     │    ▼                                │
     │ ┌──────────┐                       │
     │ │ Discard  │ ← 出牌               │
     │ └────┬─────┘                       │
     │      │                              │
     │      ▼                              │
     │ ┌──────────────┐                   │
     └──│ChowPengGang │ ← 碰杠阶段        │
        └──────────────┘                   │
                  │                        │
                  ▼                        │
              [Hu/自摸]                   │
                  │                        │
                  ▼                        │
           ┌──────────┐                   │
           │ GameOver │ ← 游戏结束       │
           └──────────┘                   │
```

## 关键交互逻辑

### 1. 摸牌流程
1. 点击"摸牌"按钮
2. 服务器发牌
3. 手牌更新
4. 显示"出牌"按钮
5. 选择要出的牌
6. 点击"出牌"

### 2. 碰牌流程
1. 别人出牌
2. 检查是否可以碰（手牌有2张相同）
3. 显示"碰"按钮
4. 点击后手牌更新
5. 进入出牌阶段

### 3. 杠牌流程
1. 摸牌后检查是否可以暗杠
2. 或碰牌后可以补杠
3. 别人出牌可以点杠
4. 点击"杠"按钮
5. 计算豆的番数

### 4. 胡牌流程
1. 满足胡牌条件时显示"胡"按钮
2. 点击胡牌
3. 结算分数
4. 显示结果

## 按钮显示逻辑

| 条件 | 显示按钮 |
|------|----------|
| 轮到自己且未摸牌 | [摸牌] |
| 已摸牌 | [出牌] |
| 可碰时 | [碰] |
| 可杠时 | [杠] |
| 可胡时 | [胡] |
| 总是显示 | [过] |
| 未报听且听牌 | [报听] |

## 结算公式

```
胡牌者得分 = 基础分 × (1 + 牌型番 + 豆番 + 鸡番) × 庄家倍数 × 自摸倍数
```

| 项目 | 说明 |
|------|------|
| 基础分 | 游戏设置中的 BaseScore |
| 牌型番 | 平胡1, 大对子4, 七对4, 龙七对8, 清一色8... |
| 豆番 | 闷豆1, 转弯豆2, 点豆3 |
| 鸡番 | 每只鸡1番 |
| 庄家倍数 | 庄家胡牌×2, 被闲家胡×2 |
| 自摸倍数 | 自摸×2, 点炮×1 |

## 文件位置汇总

```
Content/
├── UI/
│   ├── Blueprints/
│   │   ├── BP_MahjongGameWidget.uasset
│   │   ├── BP_MahjongPlayerController.uasset
│   │   ├── BP_MahjongGameState.uasset
│   │   └── BP_MahjongPlayerState.uasset
│   └── ...
└── ...
```