# MahjongGame UI 蓝图创建指南

## 蓝图创建总览

| 层级 | Blueprint 名称 | 父类 C++ | 用途 |
|------|----------------|----------|------|
| **Layer 1: 通用层** |||
| Common | WBP_Base | UBaseWidget | 基础窗口 |
| Common | WBP_ConfirmDialog | UConfirmDialogWidget | 确认对话框 |
| Common | WBP_Toast | UToastWidget | 提示消息 |
| Common | WBP_Loading | ULoadingWidget | 加载等待 |
| Common | WBP_Settings | USettingsWidget | 设置面板 |
| Common | WBP_SoundButton | USoundButton | 带音效按钮 |
| **Layer 2: 麻将专用层** |||
| Mahjong | WBP_MahjongTile | UMahjongTileWidget | 单个麻将牌 |
| Mahjong | WBP_HandTiles | UHandTilesWidget | 手牌组件 |
| Mahjong | WBP_DiscardTiles | UDiscardTilesWidget | 弃牌区 |
| Mahjong | WBP_PlayerInfo | UPlayerInfoWidget | 玩家信息 |
| Mahjong | WBP_ActionPrompt | UActionPromptWidget | 动作提示 |
| Mahjong | WBP_Countdown | UCountdownTimerWidget | 倒计时 |
| Mahjong | WBP_Reconnection | UReconnectionWidget | 断线重连 |
| Mahjong | WBP_MahjongGame | UMahjongGameWidget | 麻将主界面 |
| Mahjong | WBP_Settlement | UGameSettlementWidget | 游戏结算 |
| Mahjong | WBP_GameRules | UGameRulesWidget | 游戏规则 |
| Mahjong | WBP_RoomPassword | URoomPasswordWidget | 房间密码 |
| **Layer 3: 区域扩展层** |||
| Regional | WBP_GuizhouGame | UGuizhouMahjongWidget | 贵阳麻将 |
| Regional | WBP_GuizhouSettlement | UGuizhouSettlementWidget | 贵阳结算 |
| Regional | WBP_SichuanGame | USichuanMahjongWidget | 四川麻将 |
| Regional | WBP_SichuanSettlement | USichuanSettlementWidget | 四川结算 |

---

## 创建步骤

### 步骤 1: 创建 Blueprint 资产

1. 打开 UE5 编辑器
2. Content Browser → 选择 `Content/UI/Blueprints/` 文件夹
3. 右键 → Blueprint Class
4. 在搜索框中输入父类名称（如 `UBaseWidget`）
5. 选择类 → 命名为 `WBP_Base`
6. 点击 Create Blueprint

### 步骤 2: 配置 Widget 控件

1. 双击打开 Blueprint
2. 在 Designer 标签页中，从 Palette 拖拽控件到 Viewport
3. 选中控件，在 Details 面板中配置属性
4. 使用 BindWidget 绑定 C++ 声明的控件

### 步骤 3: 绑定事件

1. 在 Designer 中选中按钮控件
2. Details 面板 → Events → OnClicked
3. 点击 "+" 添加事件
4. 在 Graph 中连接节点

---

## Layer 1: 通用层 Blueprint 详解

### WBP_Base (UBaseWidget)

**创建路径**: `Content/UI/Blueprints/Common/WBP_Base`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── BackgroundImage (Image) - 可选背景
└── ContentSlot (CanvasSlot)
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| BackgroundImage | Visibility | Collapsed (默认) |
| BackgroundImage | Is Variable | ✓ |
| RootCanvas | Is Variable | ✓ |

**事件**: 无

---

### WBP_ConfirmDialog (UConfirmDialogWidget)

**创建路径**: `Content/UI/Blueprints/Common/WBP_ConfirmDialog`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── BackgroundOverlay (CanvasPanel) - 半透明黑色背景
│   └── OnClicked → OnBackgroundClicked
└── DialogPanel (SizeBox)
    ├── TitleText (TextBlock) - "确认"
    ├── MessageText (TextBlock) - 消息内容
    └── ButtonRow (HorizontalBox)
        ├── ConfirmButton (Button)
        │   └── OnClicked → OnConfirmClicked
        └── CancelButton (Button)
            └── OnClicked → OnCancelClicked
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| BackgroundOverlay | Visibility | Visible |
| BackgroundOverlay | Color | #000000, Opacity: 0.5 |
| TitleText | Font Size | 24 |
| TitleText | Justification | Center |
| MessageText | Font Size | 18 |
| ConfirmButton | Style | Primary Button Style |
| CancelButton | Style | Secondary Button Style |

**事件绑定**:
- ConfirmButton.OnClicked → OnConfirmClicked
- CancelButton.OnClicked → OnCancelClicked
- BackgroundOverlay.OnClicked → OnBackgroundClicked

---

### WBP_Toast (UToastWidget)

**创建路径**: `Content/UI/Blueprints/Common/WBP_Toast`

**控件结构**:
```
ToastContainer (CanvasPanel)
└── ToastItem (SizeBox)
    ├── ToastBackground (Image) - 背景纹理
    └── ContentRow (HorizontalBox)
        ├── TypeIcon (Image) - 类型图标
        └── MessageText (TextBlock)
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| ToastItem | Width Override | 300 |
| ToastItem | Height Override | 60 |
| ToastBackground | Brush | 圆角矩形纹理 |
| TypeIcon | Size | 32x32 |
| MessageText | Font Size | 16 |

---

### WBP_Loading (ULoadingWidget)

**创建路径**: `Content/UI/Blueprints/Common/WBP_Loading`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── BackgroundOverlay (Image) - 半透明黑色
├── LoadingIcon (Image) - 旋转加载图标
│   └── Animation: Rotation (360° loop, 1s)
└── LoadingText (TextBlock)
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| BackgroundOverlay | Color | #000000, Opacity: 0.7 |
| LoadingIcon | Brush | 圆形旋转图标 |
| LoadingIcon | Size | 64x64 |
| LoadingText | Font Size | 20 |
| LoadingText | Justification | Center |

**动画**: LoadingIcon Rotation 0° → 360°, 1s, Loop

---

### WBP_Settings (USettingsWidget)

**创建路径**: `Content/UI/Blueprints/Common/WBP_Settings`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── Panel_Background (SizeBox)
│   └── SettingsPanel (VerticalBox)
│       ├── Header (HorizontalBox)
│       │   ├── TitleText (TextBlock) - "设置"
│       │   └── CloseButton (Button)
│       │       └── OnClicked → OnCloseClicked
│       ├── Divider
│       ├── MusicVolumeRow (HorizontalBox)
│       │   ├── Label (TextBlock) - "音乐音量"
│       │   └── MusicVolumeSlider (Slider)
│       │       └── OnValueChanged → OnMusicVolumeChanged
│       ├── SFXVolumeRow (HorizontalBox)
│       │   ├── Label (TextBlock) - "音效音量"
│       │   └── SFXVolumeSlider (Slider)
│       │       └── OnValueChanged → OnSFXVolumeChanged
│       ├── MusicMuteCheckBox (CheckBox)
│       │   └── OnCheckStateChanged → OnMusicMuteChanged
│       ├── SFXMuteCheckBox (CheckBox)
│       │   └── OnCheckStateChanged → OnSFXMuteChanged
│       ├── VibrationCheckBox (CheckBox)
│       │   └── OnCheckStateChanged → OnVibrationChanged
│       ├── Spacer
│       ├── AccountButton (Button)
│       │   └── OnClicked → OnAccountClicked
│       └── AboutButton (Button)
│           └── OnClicked → OnAboutClicked
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| Panel_Background | Width Override | 400 |
| Panel_Background | Height Override | 600 |
| TitleText | Font Size | 28 |
| Slider | Min Value | 0 |
| Slider | Max Value | 1 |
| Slider | Step Size | 0.1 |

---

### WBP_SoundButton (USoundButton)

**创建路径**: `Content/UI/Blueprints/Common/WBP_SoundButton`

**控件结构**:
```
Button (Button)
├── BackgroundImage (Image) - 按钮背景
├── ButtonText (TextBlock) - 按钮文字
└── ButtonIcon (Image) - 可选图标
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| Button | Is Variable | ✓ |
| Button | Style | 按钮样式表 |
| BackgroundImage | Is Variable | ✓ |
| ButtonText | Is Variable | ✓ |
| ButtonText | Justification | Center |
| ButtonIcon | Is Variable | ✓ |

**事件**: Button.OnClicked → HandleClick

---

## Layer 2: 麻将专用层 Blueprint 详解

### WBP_MahjongTile (UMahjongTileWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_MahjongTile`

**控件结构**:
```
TileButton (Button)
├── TileImage (Image) - 牌面图案
├── SelectedOverlay (Image) - 选中高亮（初始隐藏）
└── DisabledOverlay (Image) - 禁用遮罩（初始隐藏）
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| TileButton | Size | 60x80 |
| TileButton | Is Variable | ✓ |
| TileImage | Is Variable | ✓ |
| TileImage | Brush | 麻将牌纹理 |
| SelectedOverlay | Visibility | Collapsed |
| SelectedOverlay | Brush | 黄色高亮 |
| DisabledOverlay | Visibility | Collapsed |
| DisabledOverlay | Color | #000000, Opacity: 0.5 |

**事件**: TileButton.OnClicked → HandleClick

---

### WBP_HandTiles (UHandTilesWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_HandTiles`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── HandTilesContainer (Horizontal Box)
│   └── [动态生成 WBP_MahjongTile 实例]
├── TileCountText (TextBlock) - "手牌: 14"
└── SortButton (Button)
    └── OnClicked → SortTiles
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| HandTilesContainer | Slot Padding | 2 |
| HandTilesContainer | Alignment | Center |
| TileCountText | Font Size | 14 |
| SortButton | Size | 80x30 |

---

### WBP_DiscardTiles (UDiscardTilesWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_DiscardTiles`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── HeaderRow (HorizontalBox)
│   ├── TitleText (TextBlock) - "弃牌区"
│   └── DiscardCountText (TextBlock) - "24"
└── DiscardTilesContainer (Wrap Box)
    └── [动态生成 WBP_MahjongTile 实例]
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| DiscardTilesContainer | Inner Slot Padding | 2 |
| DiscardTilesContainer | Max Per Line | 10 |
| TitleText | Font Size | 16 |
| DiscardCountText | Font Size | 14 |

---

### WBP_PlayerInfo (UPlayerInfoWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_PlayerInfo`

**控件结构**:
```
PlayerPanel (CanvasPanel)
├── AvatarImage (Image) - 头像
├── NicknameText (TextBlock) - 昵称
├── LevelText (TextBlock) - "Lv.10"
├── ScoreText (TextBlock) - "1000分"
├── TurnIndicator (Image) - 回合指示器（初始隐藏）
├── ReadyText (TextBlock) - "准备"（初始隐藏）
├── OfflineOverlay (Image) - 离线遮罩（初始隐藏）
└── BankerIcon (Image) - 庄家标记（初始隐藏）
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| PlayerPanel | Size | 120x150 |
| AvatarImage | Size | 80x80 |
| AvatarImage | Brush | 圆形头像纹理 |
| NicknameText | Font Size | 14 |
| LevelText | Font Size | 12 |
| ScoreText | Font Size | 16 |
| TurnIndicator | Visibility | Collapsed |
| ReadyText | Visibility | Collapsed |
| OfflineOverlay | Visibility | Collapsed |
| BankerIcon | Visibility | Collapsed |

---

### WBP_ActionPrompt (UActionPromptWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_ActionPrompt`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── TitleText (TextBlock) - "请选择动作"
├── ActionButtonsContainer (HorizontalBox)
│   ├── ChowButton (Button) - "吃"
│   ├── PongButton (Button) - "碰"
│   ├── KongButton (Button) - "杠"
│   ├── HuButton (Button) - "胡"
│   ├── DrawButton (Button) - "摸牌"
│   ├── DiscardButton (Button) - "出牌"
│   └── SkipButton (Button) - "过"
└── CountdownText (TextBlock) - "10"
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| RootCanvas | Visibility | Collapsed (默认) |
| TitleText | Font Size | 24 |
| ActionButtonsContainer | Alignment | Center |
| 每个按钮 | Size | 80x60 |
| 每个按钮 | Style | 动作按钮样式 |

**事件绑定**:
- ChowButton.OnClicked → OnChowClicked
- PongButton.OnClicked → OnPongClicked
- KongButton.OnClicked → OnKongClicked
- HuButton.OnClicked → OnHuClicked
- DrawButton.OnClicked → OnDrawClicked
- DiscardButton.OnClicked → OnDiscardClicked
- SkipButton.OnClicked → OnSkipClicked

---

### WBP_Countdown (UCountdownTimerWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_Countdown`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── CountdownText (TextBlock) - "15"
└── TimerProgressBar (ProgressBar)
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| RootCanvas | Visibility | Collapsed (默认) |
| CountdownText | Font Size | 72 |
| CountdownText | Justification | Center |
| TimerProgressBar | Size | 200x10 |
| TimerProgressBar | Fill Color | 绿色→黄色→红色 |

---

### WBP_Reconnection (UReconnectionWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_Reconnection`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── BackgroundOverlay (Image) - 半透明黑色
├── ContentPanel (VerticalBox)
│   ├── StatusIcon (Image) - 状态图标
│   ├── LoadingIcon (Image) - 旋转图标
│   ├── StatusTitleText (TextBlock) - "正在重连..."
│   ├── StatusDetailText (TextBlock) - "第 3 次重连中"
│   ├── ProgressText (TextBlock) - "3 / 5"
│   ├── ReconnectProgressBar (ProgressBar)
│   ├── RetryButton (Button)
│   └── ReturnToLobbyButton (Button)
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| BackgroundOverlay | Color | #000000, Opacity: 0.8 |
| LoadingIcon | Animation | Rotation Loop |

---

### WBP_MahjongGame (UMahjongGameWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_MahjongGame`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── TableBackgroundImage (Image) - 牌桌背景纹理
├── HeaderBar (HorizontalBox)
│   ├── RoomIDText (TextBlock) - "房间 #123"
│   ├── RoundText (TextBlock) - "4/8 局"
│   └── ButtonRow (HorizontalBox)
│       ├── MenuButton (Button)
│       ├── RulesButton (Button)
│       └── LeaveButton (Button)
├── Player3Info (WBP_PlayerInfo) - 对面玩家
├── CenterArea (CanvasPanel)
│   ├── Player2Info (WBP_PlayerInfo) - 右边玩家
│   ├── DiscardTiles (WBP_DiscardTiles)
│   └── Player4Info (WBP_PlayerInfo) - 左边玩家
├── TurnCountdown (WBP_Countdown)
├── ActionPrompt (WBP_ActionPrompt)
├── Player1Info (WBP_PlayerInfo) - 自己
└── SelfHandTiles (WBP_HandTiles)
```

**布局说明**:
```
        [Player3 - 北]
              ↑
[Player4 - 西] ←  [牌桌中央]  → [Player2 - 东]
              ↓
        [Player1 - 南/自己]
              ↓
        [手牌区域]
```

---

### WBP_Settlement (UGameSettlementWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_Settlement`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── BackgroundOverlay (Image)
├── SettlementPanel (SizeBox)
│   └── ContentBox (VerticalBox)
│       ├── TitleText (TextBlock) - "结算"
│       ├── WinnerSection (HorizontalBox)
│       │   ├── WinnerNameText (TextBlock)
│       │   ├── HuTypeText (TextBlock)
│       │   └── FanCountText (TextBlock)
│       ├── PlayerListBox (VerticalBox)
│       │   └── [动态生成结算条目]
│       ├── Divider
│       └── ButtonRow (HorizontalBox)
│           ├── ShareButton (Button)
│           ├── PlayAgainButton (Button)
│           └── ReturnToLobbyButton (Button)
```

---

### WBP_GameRules (UGameRulesWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_GameRules`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── BackgroundOverlay (Image)
├── RulesPanel (SizeBox)
│   └── ContentBox (VerticalBox)
│       ├── Header (HorizontalBox)
│       │   ├── TitleText (TextBlock) - "游戏规则"
│       │   └── CloseButton (Button)
│       ├── TabBar (HorizontalBox)
│       │   ├── BasicRulesButton (Button)
│       │   ├── FanRulesButton (Button)
│       │   ├── SpecialRulesButton (Button)
│       │   └── ScoreRulesButton (Button)
│       └── RulesScrollBox (ScrollBox)
│           └── RulesContentBox (VerticalBox)
```

---

### WBP_RoomPassword (URoomPasswordWidget)

**创建路径**: `Content/UI/Blueprints/Mahjong/WBP_RoomPassword`

**控件结构**:
```
RootCanvas (CanvasPanel)
├── BackgroundOverlay (Image)
├── PasswordPanel (SizeBox)
│   └── ContentBox (VerticalBox)
│       ├── RoomNameText (TextBlock) - "请输入房间密码"
│       ├── HintText (TextBlock) - "密码为6位数字"
│       ├── ErrorText (TextBlock) - 错误消息（红色）
│       ├── PasswordDisplayText (TextBlock) - "******"
│       ├── NumberPad (GridPanel)
│       │   ├── 1, 2, 3
│       │   ├── 4, 5, 6
│       │   ├── 7, 8, 9
│       │   ├── Clear, 0, Delete
│       └── ButtonRow (HorizontalBox)
│           ├── ConfirmButton (Button)
│           └── CancelButton (Button)
```

**属性配置**:
| 控件 | 属性 | 值 |
|------|------|-----|
| NumberPad | Column Fill | 3 列均分 |
| NumberPad | Row Fill | 4 行均分 |
| NumberButton | Size | 60x60 |
| PasswordDisplayText | Font Size | 36 |
| ErrorText | Color | #FF0000 |

---

## Layer 3: 区域扩展层 Blueprint 详解

### WBP_GuizhouGame (UGuizhouMahjongWidget)

**创建路径**: `Content/UI/Blueprints/Regional/WBP_GuizhouGame`

**基于**: WBP_MahjongGame

**额外控件**:
```
├── ChickenPromptPanel (CanvasPanel)
│   ├── ChickenPromptText (TextBlock) - "捉鸡！"
│   ├── CatchChickenButton (Button)
│   └── NoCatchChickenButton (Button)
└── XiaMaText (TextBlock) - "夏麻: +10"
```

---

### WBP_SichuanGame (USichuanMahjongWidget)

**创建路径**: `Content/UI/Blueprints/Regional/WBP_SichuanGame`

**基于**: WBP_MahjongGame

**额外控件**:
```
├── BaoJiaoPromptPanel (CanvasPanel)
│   ├── CalledTilePromptText (TextBlock) - "报叫！"
│   ├── BaoJiaoButton (Button)
│   └── NoBaoJiaoButton (Button)
└── WindRainBonusText (TextBlock) - "刮风下雨: +5"
```

---

### WBP_GuizhouSettlement (UGuizhouSettlementWidget)

**创建路径**: `Content/UI/Blueprints/Regional/WBP_GuizhouSettlement`

**基于**: WBP_Settlement

**额外控件**:
```
├── TotalChickenBonusText (TextBlock) - "捉鸡: +3"
└── TotalXiaMaBonusText (TextBlock) - "夏麻: +10"
```

---

### WBP_SichuanSettlement (USichuanSettlementWidget)

**创建路径**: `Content/UI/Blueprints/Regional/WBP_SichuanSettlement`

**基于**: WBP_Settlement

**额外控件**:
```
├── WindRainTotalText (TextBlock) - "刮风下雨: +15"
└── BloodBattleText (TextBlock) - "血战场倍数: x2"
```

---

## Blueprint 创建检查清单

### 通用检查项
- [ ] 选择正确的父类 C++
- [ ] 所有 BindWidget 控件已绑定
- [ ] 按钮事件已绑定到 Graph
- [ ] 控件 Is Variable 已勾选
- [ ] 布局 Alignment 正确

### 控件属性检查
- [ ] Size 已正确设置
- [ ] Visibility 初始状态正确
- [ ] Font Size 合适
- [ ] Color/Brush 已配置

### 事件检查
- [ ] OnClicked 事件已连接
- [ ] OnValueChanged 事件已连接（如有 Slider）
- [ ] OnCheckStateChanged 事件已连接（如有 CheckBox）

---

## 文件路径映射

```
Content/
└── UI/
    └── Blueprints/
        ├── Common/
        │   ├── WBP_Base.uasset
        │   ├── WBP_ConfirmDialog.uasset
        │   ├── WBP_Toast.uasset
        │   ├── WBP_Loading.uasset
        │   ├── WBP_Settings.uasset
        │   └── WBP_SoundButton.uasset
        ├── Mahjong/
        │   ├── WBP_MahjongTile.uasset
        │   ├── WBP_HandTiles.uasset
        │   ├── WBP_DiscardTiles.uasset
        │   ├── WBP_PlayerInfo.uasset
        │   ├── WBP_ActionPrompt.uasset
        │   ├── WBP_Countdown.uasset
        │   ├── WBP_Reconnection.uasset
        │   ├── WBP_MahjongGame.uasset
        │   ├── WBP_Settlement.uasset
        │   ├── WBP_GameRules.uasset
        │   └── WBP_RoomPassword.uasset
        └── Regional/
            ├── WBP_GuizhouGame.uasset
            ├── WBP_GuizhouSettlement.uasset
            ├── WBP_SichuanGame.uasset
            └── WBP_SichuanSettlement.uasset
```
