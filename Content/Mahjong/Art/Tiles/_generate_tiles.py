#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
麻将牌SVG生成器
生成所有麻将牌的SVG占位符图片

使用方法:
    python _generate_tiles.py
"""

import os

# 配置
OUTPUT_DIR = os.path.dirname(os.path.abspath(__file__))

# 花色配置 - 贵阳麻将只用万子、条子、筒子，不使用风牌和箭牌
SUITS = {
    "Characters": {
        "dir": "Characters",
        "name": "萬",
        "color": "#E74C3C",
        "values": ["一", "二", "三", "四", "五", "六", "七", "八", "九"],
        "prefix": "W"
    },
    "Bamboos": {
        "dir": "Bamboos",
        "name": "条",
        "color": "#27AE60",
        "values": ["一", "二", "三", "四", "五", "六", "七", "八", "九"],
        "prefix": "B"
    },
    "Circles": {
        "dir": "Circles",
        "name": "筒",
        "color": "#3498DB",
        "values": ["一", "二", "三", "四", "五", "六", "七", "八", "九"],
        "prefix": "T"
    },
    "Flowers": {
        "dir": "Flowers",
        "name": "花",
        "color": "#9B59B6",
        "values": ["梅", "兰", "菊", "竹"],
        "prefix": "H"
    }
}

def generate_tile_svg(value_text, color, filename, secondary_text="", special_color=None):
    """生成单张麻将牌SVG"""
    text_color = special_color if special_color else color

    svg = f'''<!-- {value_text} 麻将牌 -->
<svg xmlns="http://www.w3.org/2000/svg" width="128" height="128" viewBox="0 0 128 128">
  <!-- 背景 -->
  <rect width="128" height="128" fill="#F5F5DC" stroke="{color}" stroke-width="6" rx="8"/>

  <!-- 内边框 -->
  <rect x="8" y="8" width="112" height="112" fill="none" stroke="{color}" stroke-width="2" rx="4"/>

  <!-- 主文字 -->
  <text x="64" y="75" font-family="Arial, sans-serif" font-size="56" font-weight="bold" fill="{text_color}" text-anchor="middle">{value_text}</text>

  <!-- 副文字(如果有) -->
  {"<text x='64' y='115' font-family='Arial, sans-serif' font-size='20' fill='{color}' text-anchor='middle'>{secondary_text}</text>" if secondary_text else ""}
</svg>'''

    return svg

def generate_all_tiles():
    """生成所有麻将牌SVG"""
    for suit_key, suit_config in SUITS.items():
        suit_dir = os.path.join(OUTPUT_DIR, suit_config["dir"])
        os.makedirs(suit_dir, exist_ok=True)

        print(f"生成 {suit_config['name']} 牌...")

        for i, value_text in enumerate(suit_config["values"], 1):
            filename = os.path.join(suit_dir, f"{suit_config['prefix']}{i}.svg")
            svg = generate_tile_svg(value_text, suit_config["color"], filename, suit_config["name"])

            with open(filename, 'w', encoding='utf-8') as f:
                f.write(svg)

    # 生成特殊牌
    special_dir = os.path.join(OUTPUT_DIR, "Special")
    os.makedirs(special_dir, exist_ok=True)

    # 鸡牌
    chicken_svg = '''<!-- 鸡牌 SVG占位符 -->
<svg xmlns="http://www.w3.org/2000/svg" width="128" height="128" viewBox="0 0 128 128">
  <rect width="128" height="128" fill="#F39C12" stroke="#000000" stroke-width="6" rx="8"/>
  <text x="64" y="80" font-family="Arial, sans-serif" font-size="56" font-weight="bold" fill="#FFFFFF" text-anchor="middle">鸡</text>
  <text x="64" y="115" font-family="Arial, sans-serif" font-size="18" fill="#FFFFFF" text-anchor="middle">CHICKEN</text>
</svg>'''
    with open(os.path.join(special_dir, "Chicken.svg"), 'w', encoding='utf-8') as f:
        f.write(chicken_svg)

    # 金鸡
    golden_svg = '''<!-- 金鸡 SVG占位符 -->
<svg xmlns="http://www.w3.org/2000/svg" width="128" height="128" viewBox="0 0 128 128">
  <defs>
    <linearGradient id="goldGradient" x1="0%" y1="0%" x2="100%" y2="100%">
      <stop offset="0%" style="stop-color:#FFD700"/>
      <stop offset="50%" style="stop-color:#FFA500"/>
      <stop offset="100%" style="stop-color:#FFD700"/>
    </linearGradient>
  </defs>
  <rect width="128" height="128" fill="url(#goldGradient)" stroke="#8B4513" stroke-width="6" rx="8"/>
  <text x="64" y="80" font-family="Arial, sans-serif" font-size="56" font-weight="bold" fill="#8B4513" text-anchor="middle">金鸡</text>
  <text x="64" y="115" font-family="Arial, sans-serif" font-size="18" fill="#8B4513" text-anchor="middle">GOLD</text>
</svg>'''
    with open(os.path.join(special_dir, "GoldenChicken.svg"), 'w', encoding='utf-8') as f:
        f.write(golden_svg)

    # 赖子/百搭
    joker_svg = '''<!-- 赖子 SVG占位符 -->
<svg xmlns="http://www.w3.org/2000/svg" width="128" height="128" viewBox="0 0 128 128">
  <defs>
    <linearGradient id="jokerGradient" x1="0%" y1="0%" x2="100%" y2="100%">
      <stop offset="0%" style="stop-color:#9B59B6"/>
      <stop offset="100%" style="stop-color:#8E44AD"/>
    </linearGradient>
  </defs>
  <rect width="128" height="128" fill="url(#jokerGradient)" stroke="#000000" stroke-width="6" rx="8"/>
  <text x="64" y="80" font-family="Arial, sans-serif" font-size="56" font-weight="bold" fill="#FFFFFF" text-anchor="middle">赖</text>
  <text x="64" y="115" font-family="Arial, sans-serif" font-size="18" fill="#FFFFFF" text-anchor="middle">JOKER</text>
</svg>'''
    with open(os.path.join(special_dir, "Joker.svg"), 'w', encoding='utf-8') as f:
        f.write(joker_svg)

    print("完成! 所有SVG麻将牌已生成。")
    print(f"输出目录: {OUTPUT_DIR}")

def generate_ui_svgs():
    """生成UI元素的SVG占位符"""
    ui_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    # 按钮
    button_dir = os.path.join(ui_dir, "UI", "Buttons")
    os.makedirs(button_dir, exist_ok=True)

    buttons = [
        ("btn_draw", "DRAW", "#27AE60"),
        ("btn_discard", "DISCARD", "#3498DB"),
        ("btn_peng", "PENG", "#E74C3C"),
        ("btn_gang", "GANG", "#F39C12"),
        ("btn_hu", "HU", "#E74C3C"),
        ("btn_guo", "GUO", "#95A5A6"),
        ("btn_ting", "TING", "#9B59B6"),
        ("btn_zhuaji", "ZHUAJI", "#F39C12")
    ]

    for filename, text, color in buttons:
        svg = f'''<!-- {text} 按钮 -->
<svg xmlns="http://www.w3.org/2000/svg" width="128" height="64" viewBox="0 0 128 64">
  <defs>
    <linearGradient id="{filename}_gradient" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" style="stop-color:{color}"/>
      <stop offset="100%" style="stop-color:{color}" style="stop-opacity:0.8"/>
    </linearGradient>
  </defs>
  <rect width="128" height="64" fill="url(#{filename}_gradient)" stroke="#000000" stroke-width="2" rx="8"/>
  <text x="64" y="40" font-family="Arial, sans-serif" font-size="24" font-weight="bold" fill="#FFFFFF" text-anchor="middle">{text}</text>
</svg>'''
        with open(os.path.join(button_dir, f"{filename}.svg"), 'w', encoding='utf-8') as f:
            f.write(svg)

    print(f"UI按钮SVG已生成: {button_dir}")

if __name__ == "__main__":
    generate_all_tiles()
    generate_ui_svgs()
    print("\n所有占位符SVG资源已生成!")
    print("可以使用Inkscape或ImageMagick转换为PNG:")
    print("  inkscape -w 128 -h 128 tile.svg -o tile.png")
    print("  or")
    print("  convert tile.svg -resize 128x128 tile.png")
