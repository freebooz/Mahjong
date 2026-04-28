# 贵阳麻将游戏资源文件

本目录包含游戏开发所需的所有资源文件和相关配置。

## 快速开始

### 1. 生成占位符资源

#### 图片资源（SVG格式）
```bash
cd Content/Mahjong/Art/Tiles
python _generate_tiles.py
```

#### 音效资源（WAV格式）
```bash
cd Content/Mahjong/Audio
python _generate_audio.py
```

### 2. 在UE5中导入资源

1. 在UE5编辑器中，打开 **Content Browser**
2. 右键点击 `Content/Mahjong` 文件夹
3. 选择 **Import** 或将文件拖入

### 3. 配置引用

参考 `Data/UE5AssetConfig.json` 中的资源路径配置

---

## 资源目录结构

```
Content/Mahjong/
├── Art/
│   ├── Tiles/                      # 麻将牌图片
│   │   ├── Characters/            # 萬子 SVG/PNG
│   │   ├── Bamboos/               # 筒子 SVG/PNG
│   │   ├── Circles/              # 条子 SVG/PNG
│   │   ├── Winds/                # 四风 SVG/PNG
│   │   ├── Dragons/             # 三元 SVG/PNG
│   │   ├── Flowers/             # 花牌 SVG/PNG
│   │   ├── Special/             # 特殊牌 SVG/PNG
│   │   ├── _generate_tiles.py    # SVG生成脚本
│   │   └── _placeholders/        # 占位符PNG
│   ├── Tilesets/                 # 牌组集合
│   └── UI/                       # UI元素
│       ├── Buttons/
│       ├── Icons/
│       ├── Backgrounds/
│       └── Effects/
├── Audio/
│   ├── SFX/                      # 音效
│   │   ├── Tiles/
│   │   ├── Actions/
│   │   ├── Special/
│   │   └── UI/
│   ├── BGM/                     # 背景音乐
│   └── _generate_audio.py       # 音效生成脚本
├── Data/                         # 配置数据
│   ├── TileValues.json          # 牌值配置
│   ├── TileVisualConfig.json   # 视觉配置
│   ├── ScoringRules.json       # 计分规则
│   ├── AudioConfig.json        # 音效配置
│   ├── AudioPlaceholder.json    # 音效占位符规格
│   ├── AssetList.json          # 资源清单
│   └── UE5AssetConfig.json     # UE5资源配置
├── Fonts/                       # 字体资源
└── README_Resources.md          # 本文件
```

---

## 数据配置文件

### TileValues.json
贵阳麻将144张牌的完整定义：
- 萬子、筒子、条子各36张（4×9）
- 东南西北各4张
- 中发白各4张
- 花牌4张
- 鸡牌2张

### ScoringRules.json
贵阳麻将特有规则配置：
- 胡牌类型（大对子、七对、龙七对、清一色等）
- 豆计算（闷豆、转弯豆、点豆）
- 鸡牌规则（幺鸡、翻牌鸡、冲锋鸡、责任鸡、金鸡）
- 番数计算

### AudioConfig.json
音效系统配置：
- 音量分组（SFX、BGM、UI）
- 音效文件路径映射
- 音调随机范围
- 衰减设置

### TileVisualConfig.json
程序化生成牌面视觉配置：
- 纹理尺寸
- 颜色配置
- 字体设置

---

## 生成脚本

### _generate_tiles.py
生成SVG格式的麻将牌图片：
- 自动创建所有花色的牌
- 支持自定义颜色和样式
- 生成的SVG可转换为PNG

### _generate_audio.py
生成占位符WAV音效：
- 需要Python + numpy + scipy
- 生成测试用音效
- 实际项目应使用专业录音

---

## UE5资源路径

参考 `Data/UE5AssetConfig.json`：

| 资源类型 | UE5路径 |
|---------|---------|
| 麻将牌纹理 | /Game/Mahjong/Art/Tiles/ |
| 音效 | /Game/Mahjong/Audio/SFX/ |
| BGM | /Game/Mahjong/Audio/BGM/ |
| 游戏规则 | /Game/Mahjong/Blueprint/Rules/ |
| UI组件 | /Game/Mahjong/Blueprint/UI/ |

---

## 推荐免费资源网站

### 音效
- [Freesound](https://freesound.org) - 搜索 "mahjong", "dice", "click"
- [Zapsplat](https://zapsplat.com) - 游戏音效

### 音乐
- [Free Music Archive](https://freemusicarchive.org)
- [Incompetech](https://incompetech.com)
- [Bensound](https://www.bensound.com) (需署名)

### 图片
- [OpenGameArt](https://opengameart.org) - 搜索 "mahjong"
- [itch.io](https://itch.io) - 游戏素材

### 字体
- [思源字体](https://github.com/adobe-fonts/source-han-sans) - 思源黑体
- [Noto Fonts](https://fonts.google.com/noto) - Google开源字体

---

## 资源规格表

| 类型 | 规格 | 格式 |
|------|------|------|
| 麻将牌 | 128×128 或 256×256 | PNG (32-bit ARGB) |
| 按钮 | 128×64 | PNG (带透明通道) |
| 图标 | 32×32 或 64×64 | PNG (带透明通道) |
| 背景 | 1920×1080 | PNG 或 DDS |
| 音效 | 44.1kHz, 16bit | WAV 或 OGG |
| BGM | 44.1kHz, 16bit, Stereo | OGG (循环) |

---

## 更新日志

| 日期 | 版本 | 更新内容 |
|------|------|---------|
| 2024-04-25 | 1.0 | 初始创建目录结构和配置文件 |
