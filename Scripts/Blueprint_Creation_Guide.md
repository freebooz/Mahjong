# Blueprint 创建指南 / Blueprint Creation Guide

## 在 Unreal Editor 中创建 Blueprint 类

### 1. 创建 HUD Blueprint

1. 打开 Unreal Editor
2. 在 Content Browser 中，导航到 `Content/UI/Blueprints`
3. 右键点击空白区域
4. 选择 **Create Basic Asset > Blueprint Class**
5. 在弹出的窗口中，搜索并选择 `HUDWidgetBase` 作为父类
6. 将新 Blueprint 命名为 `BP_HUDWidget`
7. 双击打开 Blueprint
8. 在 Designer 面板中，添加 UI 组件并绑定到变量

### 2. 创建其他 Game UI Blueprints

按照上述步骤，创建以下 Blueprints：

| Blueprint 名称 | 父类 | 用途 |
|---------------|------|------|
| BP_GameWidget | GameWidgetBase | 游戏主界面 |
| BP_SettingsWidget | SettingsWidgetBase | 设置界面 |
| BP_HUDWidget | HUDWidgetBase | HUD 显示 |

### 3. 创建 Widget Blueprints

| Blueprint 名称 | 父类 | 用途 |
|---------------|------|------|
| BP_LoginWidget | LoginWidgetBase | 登录界面 |
| BP_LobbyWidget | LobbyWidgetBase | 大厅界面 |
| BP_RoomWidget | RoomWidgetBase | 房间界面 |

## 手动创建步骤详解

### 创建 BP_HUDWidget

1. **打开项目**
   ```
   双击 MahjongGame.uproject 打开项目
   ```

2. **创建 Blueprint**
   - 在 Content Browser 右键
   - 选择 `Create Advanced Asset > Blueprints > Blueprint Class`
   - 搜索 `HUDWidgetBase`
   - 选择后点击 `Select`
   - 命名为 `BP_HUDWidget`

3. **打开 Designer**
   - 双击 BP_HUDWidget 打开
   - 点击 `Designer` 标签

4. **添加 UI 组件**
   - 从 Palette 拖入 **Canvas Panel** (命名: HUDPanel)
   - 在 Canvas 内添加:
     - **Progress Bar** (命名: TimerProgressBar)
     - **Text Block** (命名: TimerText)
     - **Text Block** (命名: TurnIndicatorText)
     - **Text Block** (命名: ActionPromptText)
     - **Text Block** (命名: TileCountText)
     - **Text Block** (命名: MyScoreText)
     - **Text Block** (命名: ConnectionStatusText)
     - **Button** x3 (命名: ResumeButton, SettingsButton, QuitButton)
     - **Canvas Panel** (命名: PauseMenuPanel)

5. **绑定 Widget**
   - 在 My Blueprint 面板中，选择 Variables
   - 点击 + 添加变量
   - 变量名称必须与 C++ 类中的 `BindWidget` 元数据匹配
   - 例如: `HUDPanel`, `TimerText`, `TimerProgressBar` 等

6. **编译保存**
   - 点击 Compile 按钮
   - 保存 Blueprint

## Blueprint 继承关系

```
C++ Base Classes (Source/)
├── LoginWidgetBase.h/cpp
├── LobbyWidgetBase.h/cpp
├── RoomWidgetBase.h/cpp
├── GameWidgetBase.h/cpp
├── SettingsWidgetBase.h/cpp
└── HUDWidgetBase.h/cpp
        │
        ▼
Content/UI/Blueprints/
├── BP_LoginWidget (inherits LoginWidgetBase)
├── BP_LobbyWidget (inherits LobbyWidgetBase)
├── BP_RoomWidget (inherits RoomWidgetBase)
├── BP_GameWidget (inherits GameWidgetBase)
├── BP_SettingsWidget (inherits SettingsWidgetBase)
└── BP_HUDWidget (inherits HUDWidgetBase)
```

## 使用 Python 脚本创建 (可选)

在 Editor 中执行 Python 脚本：

1. 打开 Editor
2. 选择 **Edit > Plugin > Console**
3. 输入: `py Scripts/CreateBlueprints.py`
4. 或在 **Window > Developer Tools > Python Editor** 中执行脚本

## 绑定 Widget 变量名对照表

### HUDWidgetBase BindWidget 列表

| 变量名 | 类型 | 说明 |
|--------|------|------|
| HUDPanel | CanvasPanel | HUD 主面板 |
| TimerText | TextBlock | 计时器文本 |
| TimerProgressBar | ProgressBar | 计时器进度条 |
| TurnIndicatorText | TextBlock | 回合指示器 |
| ActionPromptText | TextBlock | 动作提示 |
| TileCountText | TextBlock | 剩余牌数 |
| MyScoreText | TextBlock | 我的分数 |
| ConnectionStatusText | TextBlock | 连接状态 |
| PauseMenuPanel | CanvasPanel | 暂停菜单面板 |
| ResumeButton | Button | 继续按钮 |
| SettingsButton | Button | 设置按钮 |
| QuitButton | Button | 退出按钮 |
| PlayerInfoBox | HorizontalBox | 玩家信息框 |
| QuickActionsBox | VerticalBox | 快捷操作框 |
| NetworkIndicator | Image | 网络指示器 |

### SettingsWidgetBase BindWidget 列表

| 变量名 | 类型 | 说明 |
|--------|------|------|
| SettingsPanel | CanvasPanel | 设置面板 |
| CloseButton | Button | 关闭按钮 |
| ApplyButton | Button | 应用按钮 |
| CancelButton | Button | 取消按钮 |
| MasterVolumeSlider | Slider | 主音量 |
| MusicVolumeSlider | Slider | 音乐音量 |
| SFXVolumeSlider | Slider | 音效音量 |
| GraphicsQualityCombo | ComboBoxString | 画质选项 |
| ResolutionCombo | ComboBoxString | 分辨率选项 |
| FullscreenCheckBox | CheckBox | 全屏复选框 |
| PlayerNameTextBox | EditableTextBox | 玩家名称输入 |
| AutoReadyCheckBox | CheckBox | 自动准备复选框 |
| SettingsTitleText | TextBlock | 设置标题 |

### GameWidgetBase BindWidget 列表

| 变量名 | 类型 | 说明 |
|--------|------|------|
| RoomDialogPanel | CanvasPanel | 房间对话框 |
| CreateRoomButton | Button | 创建房间按钮 |
| JoinRoomButton | Button | 加入房间按钮 |
| LeaveButton | Button | 离开按钮 |
| RoomIDText | TextBlock | 房间号文本 |
| RoomNameText | TextBlock | 房间名称 |
| PlayerCountText | TextBlock | 玩家数量 |
| PlayerListBox | VerticalBox | 玩家列表 |
| HandTilesBox | HorizontalBox | 手牌框 |
| DrawTileButton | Button | 摸牌按钮 |
| DiscardButton | Button | 出牌按钮 |
| ChowButton | Button | 吃按钮 |
| PongButton | Button | 碰按钮 |
| KongButton | Button | 杠按钮 |
| HuButton | Button | 胡按钮 |
| ZhuaJiButton | Button | 抓鸡按钮 |
| TurnInfoText | TextBlock | 回合信息 |
| StatusText | TextBlock | 状态文本 |
| ErrorText | TextBlock | 错误文本 |
| DiscardPoolScroll | ScrollBox | 弃牌池滚动框 |
| DiscardPoolBox | HorizontalBox | 弃牌池框 |

## 完成后验证

1. 所有 Blueprint 编译无错误
2. Widget 正确绑定（Designer 中变量旁无警告图标）
3. 在地图中测试 UI 显示

## 文件位置

```
MahjongGame/
├── Source/MahjongGame/
│   ├── Public/UI/
│   │   ├── HUDWidgetBase.h
│   │   ├── SettingsWidgetBase.h
│   │   └── GameWidgetBase.h
│   └── Private/UI/
│       ├── HUDWidgetBase.cpp
│       ├── SettingsWidgetBase.cpp
│       └── GameWidgetBase.cpp
└── Content/
    └── UI/
        └── Blueprints/
            ├── BP_HUDWidget.uasset
            ├── BP_SettingsWidget.uasset
            └── BP_GameWidget.uasset
```