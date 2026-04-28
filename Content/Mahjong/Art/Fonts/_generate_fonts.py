# -*- coding: utf-8 -*-
"""
麻将游戏字体资源说明
Mahjong Font Resources

使用方法:
    python _generate_font_preview.py
    将在 Fonts/ 目录生成预览图 preview.png
"""

import os
import sys

# 检查是否有PIL, 如果没有则跳过图片生成只显示文字信息
try:
    from PIL import Image, ImageDraw, ImageFont
    HAS_PIL = True
except ImportError:
    HAS_PIL = False

# 字体目录
FONT_DIR = os.path.dirname(os.path.abspath(__file__))

# 麻将字符集
MAHJONG_CHARACTERS = {
    "Characters (萬)": ["一萬", "二萬", "三萬", "四萬", "五萬", "六萬", "七萬", "八萬", "九萬"],
    "Bamboos (筒)": ["一筒", "二筒", "三筒", "四筒", "五筒", "六筒", "七筒", "八筒", "九筒"],
    "Circles (條)": ["一条", "二条", "三条", "四条", "五条", "六条", "七条", "八条", "九条"],
    "Winds (风)": ["东风", "南风", "西风", "北风"],
    "Dragons (字)": ["红中", "发财", "白板"],
    "Flowers (花)": ["梅", "兰", "菊", "竹"],
}

# 特殊牌
SPECIAL_TILES = ["鸡", "金鸡", "赖子"]


def get_available_fonts():
    """获取可用字体文件"""
    fonts = []
    for f in os.listdir(FONT_DIR):
        if f.lower().endswith(('.ttf', '.otf', '.ttc')):
            fonts.append(f)
    return fonts


def generate_preview_image():
    """生成字体预览图"""
    if not HAS_PIL:
        print("警告: PIL未安装, 无法生成预览图")
        print("安装命令: pip install pillow")
        return

    fonts = get_available_fonts()
    if not fonts:
        print("未找到字体文件")
        return

    # 创建大图
    img_width = 1200
    img_height = 800
    img = Image.new('RGB', (img_width, img_height), color=(30, 30, 40))
    draw = ImageDraw.Draw(img)

    y_offset = 30
    for font_file in fonts:
        font_path = os.path.join(FONT_DIR, font_file)
        try:
            font = ImageFont.truetype(font_path, 32)
            large_font = ImageFont.truetype(font_path, 40)
        except Exception as e:
            print(f"无法加载字体 {font_file}: {e}")
            continue

        # 字体名称
        draw.text((30, y_offset), f"字体: {font_file}", font=font, fill=(255, 200, 100))
        y_offset += 50

        # 显示麻将字符
        for suit, chars in MAHJONG_CHARACTERS.items():
            draw.text((30, y_offset), f"{suit}:", font=font, fill=(200, 200, 200))
            x_offset = 150
            for char in chars:
                draw.text((x_offset, y_offset), char, font=large_font, fill=(255, 255, 255))
                x_offset += 70
            y_offset += 55

        y_offset += 20

    # 保存预览图
    preview_path = os.path.join(FONT_DIR, "preview.png")
    img.save(preview_path)
    print(f"预览图已生成: {preview_path}")


if __name__ == "__main__":
    print("=" * 60)
    print("麻将游戏字体资源")
    print("=" * 60)

    fonts = get_available_fonts()
    if not fonts:
        print("错误: Fonts目录下未找到字体文件!")
        print("请将.ttf/.otf字体文件放入此目录")
        sys.exit(1)

    print(f"\n找到 {len(fonts)} 个字体文件:")
    for f in fonts:
        size = os.path.getsize(os.path.join(FONT_DIR, f))
        size_mb = size / (1024 * 1024)
        print(f"  - {f} ({size_mb:.1f} MB)")

    print("\n麻将字符支持:")
    for suit, chars in MAHJONG_CHARACTERS.items():
        print(f"  {suit}: {''.join(chars)}")

    print("\n特殊牌:")
    print(f"  {''.join(SPECIAL_TILES)}")

    print("\n" + "=" * 60)
    generate_preview_image()
    print("=" * 60)
    print("\n字体文件已导入到项目中:")
    print("  Content/Mahjong/Art/Fonts/")
    print("\n在UE中使用:")
    print("  1. 打开UE编辑器")
    print("  2. 在Content Browser中导航到 Content/Mahjong/Art/Fonts/")
    print("  3. 右键点击字体文件 -> Asset Actions -> Create Font Instance")
    print("  4. 设置Font Family为对应中文字体(如simkai))")
    print("=" * 60)
