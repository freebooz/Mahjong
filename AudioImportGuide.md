# UE5 音效资源导入指南

## 音效资源目录结构

```
Content/
├── Audio/
│   ├── UI/                          # UI 音效
│   │   ├── ButtonClick_Cue.uasset
│   │   ├── Confirm_Cue.uasset
│   │   ├── Cancel_Cue.uasset
│   │   ├── Success_Cue.uasset
│   │   └── Error_Cue.uasset
│   └── Mahjong/                     # 麻将游戏音效
│       ├── DrawTile_Cue.uasset
│       ├── DiscardTile_Cue.uasset
│       ├── Chow_Cue.uasset
│       ├── Pong_Cue.uasset
│       ├── Kong_Cue.uasset
│       └── Hu_Cue.uasset
```

---

## 导入步骤

### 步骤 1：创建音效目录

1. 打开 UE5 编辑器
2. Content Browser → 右键 → New Folder → 命名为 `Audio`
3. 在 Audio 文件夹内创建 `UI` 和 `Mahjong` 子文件夹

### 步骤 2：导入音效文件

将下载的 `.wav` 或 `.mp3` 文件放入对应目录后：

1. 在 Content Browser 中选择 `Content/Audio/UI/`
2. 右键 → Import to /Game/Audio/UI/
3. 选择音效文件（如 `button_click.wav`）
4. UE5 自动创建对应的 Sound Cue 资源

### 步骤 3：重命名为标准名称

| 原文件名 | 标准名称 | 用途 |
|----------|----------|------|
| button_click.wav | ButtonClick_Cue | 按钮点击 |
| confirm.wav | Confirm_Cue | 确认 |
| cancel.wav | Cancel_Cue | 取消 |
| success.wav | Success_Cue | 成功 |
| error.wav | Error_Cue | 错误 |
| draw_tile.wav | DrawTile_Cue | 摸牌 |
| discard_tile.wav | DiscardTile_Cue | 出牌 |
| chow.wav | Chow_Cue | 吃牌 |
| pong.wav | Pong_Cue | 碰牌 |
| kong.wav | Kong_Cue | 杠牌 |
| hu.wav | Hu_Cue | 胡牌 |

---

## 音效 Cue 属性配置

### 通用配置 (所有音效)

| 属性 | 推荐值 | 说明 |
|------|--------|------|
| Volume | 1.0 | 音量 |
| Pitch | 1.0 | 音调 |
| bOverrideAttenuation | true | 覆盖衰减 |
| Attenuation - Distance | 500 | 最大距离 |

### UI 音效配置

```csharp
// Volume: 0.8 - 1.0
// Pitch: 1.0
// Duration: < 1秒
// 衰减: 关闭或短距离
```

### 麻将音效配置

```csharp
// Volume: 1.0
// Pitch: 1.0
// Duration: 0.3 - 0.8秒
// 衰减: 500-1000 (房间内可听)
```

---

## Sound Cue 资源模板

### ButtonClick_Cue 配置

```
Sound Cue Editor:
├── Wave Player (加载 button_click.wav)
│   └── Volume: 0.9
└── Output
```

### Mahjong 音效配置

```
Sound Cue Editor:
├── Wave Player (加载对应 .wav)
│   └── Volume: 1.0
│   └── Pitch: 1.0
└── Output
```

---

## 代码引用路径

音效加载路径（在 UISoundManager.cpp 中配置）：

```cpp
// UI 音效路径
"SoundCue'/Game/Audio/UI/ButtonClick_Cue.ButtonClick_Cue'"
"SoundCue'/Game/Audio/UI/Confirm_Cue.Confirm_Cue'"
"SoundCue'/Game/Audio/UI/Cancel_Cue.Cancel_Cue'"
"SoundCue'/Game/Audio/UI/Success_Cue.Success_Cue'"
"SoundCue'/Game/Audio/UI/Error_Cue.Error_Cue'"

// 麻将音效路径
"SoundCue'/Game/Audio/Mahjong/DrawTile_Cue.DrawTile_Cue'"
"SoundCue'/Game/Audio/Mahjong/DiscardTile_Cue.DiscardTile_Cue'"
"SoundCue'/Game/Audio/Mahjong/Chow_Cue.Chow_Cue'"
"SoundCue'/Game/Audio/Mahjong/Pong_Cue.Pong_Cue'"
"SoundCue'/Game/Audio/Mahjong/Kong_Cue.Kong_Cue'"
"SoundCue'/Game/Audio/Mahjong/Hu_Cue.Hu_Cue'"
```

---

## 免费音效资源推荐

### 网站

| 网站 | 链接 | 说明 |
|------|------|------|
| Freesound | https://freesound.org | 免费音效，需注册 |
| Pixabay Sounds | https://pixabay.com/sound-effects/ | 免费音效 |
| Zapsplat | https://zapsplat.com | 免费音效 |
| BBC Sound Effects | https://sound-effects.bbcrewind.co.uk | BBC 免费音效库 |

### 推荐搜索关键词

| 音效 | 英文关键词 |
|------|-----------|
| 按钮点击 | button click, ui click |
| 确认 | confirm, positive |
| 取消 | cancel, negative |
| 成功 | success, achievement |
| 错误 | error, fail, buzz |
| 摸牌 | card draw, tile slide |
| 出牌 | card place, tile drop |
| 麻将 | mahjong, domino |

---

## 临时替代方案（无音效文件时）

如果暂时没有音效文件，可使用 UE5 内置语音：

### 使用 UE5 Starter Content 音效

1. Window → Project Settings → Audio
2. Enable "Include Starter Content"
3. 使用 StarterContent/Audio 中的：
   - `Default__Click_Cue` - 按钮点击
   - `Default__explosion_Cue` - 爆炸/确认
   - `Default__Targeted_Cue` - 目标音

### 修改代码使用内置语音

```cpp
// UISoundManager.cpp - 临时方案
void UUISoundManager::PlayButtonClick(UObject* WorldContextObject)
{
    if (!WorldContextObject) return;

    // 使用 UE 内置语音（临时）
    USoundBase* Sound = LoadSoundFromPath(
        "SoundCue'/Game/FirstPerson/Audio/FirstPersonCompileClick_Cue.FirstPersonCompileClick_Cue'"
    );
    if (Sound)
        UGameplayStatics::PlaySound2D(WorldContextObject, Sound);
}
```

---

## 验证清单

导入完成后，请确认：

- [ ] 所有 Sound Cue 文件已创建
- [ ] Sound Cue 可在 Content Browser 中预览播放
- [ ] 路径与 UISoundManager.cpp 中的路径一致
- [ ] Blueprint 中可拖拽 Sound Cue 到 Widget

---

## 常见问题

### Q: 音效没有声音？
**A:** 检查 Sound Cue 的 Volume 是否为 0，attenuation 设置是否正确

### Q: 路径找不到？
**A:** 确认 Sound Cue 的 Asset Path，格式应为 `SoundCue'/Game/Audio/...`

### Q: 音效延迟？
**A:** UI 音效使用 `PlaySound2D` 应无延迟，检查是否误用了 3D 声音播放函数
