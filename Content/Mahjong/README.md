# 贵阳麻将游戏资源目录

本目录包含贵阳捉鸡麻将游戏所需的所有资源文件。

## 目录结构

```
Content/Mahjong/
├── Art/                      # 美术资源
│   ├── Tiles/              # 麻将牌图片
│   │   ├── Characters/     # 萬子 (1-9萬)
│   │   ├── Bamboos/        # 筒子 (1-9筒)
│   │   ├── Circles/        # 条子 (1-9条)
│   │   ├── Winds/          # 四风 (东南西北)
│   │   ├── Dragons/        # 三元 (中发白)
│   │   ├── Flowers/        # 花牌 (梅兰菊竹)
│   │   └── Special/        # 特殊牌 (鸡、赖子)
│   ├── Tilesets/           # 牌组集合
│   │   ├── Classic/        # 经典风格
│   │   ├── Modern/         # 现代风格
│   │   └── Guiyang/        # 贵阳风格
│   └── UI/                 # UI图素
│       ├── Buttons/        # 按钮
│       ├── Icons/          # 图标
│       ├── Backgrounds/    # 背景
│       └── Effects/        # 特效
│
├── Audio/                   # 音频资源
│   ├── SFX/               # 音效
│   │   ├── Tiles/         # 摸牌音效
│   │   ├── Actions/       # 动作音效
│   │   ├── Special/       # 特殊音效
│   │   └── UI/            # UI音效
│   └── BGM/              # 背景音乐
│
├── Fonts/                  # 字体资源
│   ├── Chinese/           # 中文字体
│   └── Numbers/           # 数字字体
│
└── Data/                   # 配置数据
    ├── TileValues.json     # 牌值配置
    ├── ScoringRules.json  # 计分规则
    └── AudioConfig.json   # 音效配置
```

## 资源配置

### TileValues.json
定义所有麻将牌的：
- 牌值 (TileID)
- 花色分类 (Suit)
- 资源路径 (AssetPath)
- 数量统计

### ScoringRules.json
定义贵阳麻将特有规则：
- 胡牌类型 (HuTypes)
- 豆计算 (DouTypes)
- 鸡牌规则 (ChickenRules)
- 番数倍数 (FanMultiplier)

### AudioConfig.json
定义所有音频：
- 音量设置
- 音效映射
- 背景音乐
- 衰减设置

## 资源获取

### 免费资源来源
| 类型 | 来源 |
|------|------|
| 麻将牌图片 | OpenGameArt.org, itch.io |
| 音效 | freesound.org |
| 中文字体 | 思源字体 (Source Han Sans) |

### 推荐规格
| 类型 | 规格 |
|------|------|
| 牌图片 | 128x128 或 256x256 PNG |
| 按钮 | 128x64 PNG |
| 背景 | 1920x1080 PNG |
| 音效 | WAV 44.1kHz 16bit 或 OGG |
| BGM | OGG 循环 |

## 贵阳麻将特有资源

### 鸡牌系统
- 幺鸡 (Chicken.png) - 基础鸡牌
- 冲锋鸡 (ChongFengJi.png) - 报听后打出的鸡
- 责任鸡 (ZeRenJi.png) - 点炮时打出的鸡
- 金鸡 (GoldenChicken.png) - 翻牌区金鸡

### 鸡牌音效
- chicken_call.wav - 翻到鸡的叫声
- golden_ji.wav - 金鸡特效音

## 命名规范

### 文件命名
```
{TileType}_{Value}.png
W1.png     # 一萬
D5.png     # 五筒
T9.png     # 九条
East.png   # 东风
```

### UE5资源导入
1. 将PNG图片导入 `Content/Mahjong/Art/Tiles/`
2. 设置纹理类型为 UI 或 Sprite
3. 创建 Paper2D Sprite 或 UMG Image
4. 音效导入为 Sound Cue

## 更新日志
- 2024-04-25: 初始创建目录结构
