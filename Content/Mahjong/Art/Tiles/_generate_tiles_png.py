#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
麻将牌PNG图片生成器
直接生成PNG格式的麻将牌图片，无需外部依赖

使用方法:
    python _generate_tiles_png.py
"""

import os
import struct
import zlib

# PNG 文件格式常量
PNG_SIGNATURE = b'\x89PNG\r\n\x1a\n'

def create_png(width, height, color_rgba):
    """创建一个简单的PNG图片"""
    def make_chunk(chunk_type, data):
        chunk_len = struct.pack('>I', len(data))
        chunk_crc = struct.pack('>I', zlib.crc32(chunk_type + data) & 0xffffffff)
        return chunk_len + chunk_type + data + chunk_crc

    # IHDR chunk
    ihdr_data = struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0)
    ihdr = make_chunk(b'IHDR', ihdr_data)

    # IDAT chunk (image data)
    raw_data = b''
    r, g, b, a = color_rgba
    for y in range(height):
        raw_data += b'\x00'  # filter byte
        for x in range(width):
            raw_data += bytes([r, g, b, a])

    compressed = zlib.compress(raw_data, 9)
    idat = make_chunk(b'IDAT', compressed)

    # IEND chunk
    iend = make_chunk(b'IEND', b'')

    return PNG_SIGNATURE + ihdr + idat + iend

def create_tile_png(filename, text, bg_color, text_color, border_color, width=128, height=128):
    """创建单张麻将牌PNG"""
    # 创建牌面图像
    pixels = []
    for y in range(height):
        row = []
        for x in range(height):
            # 圆角矩形边框
            in_border = x < 6 or x >= width - 6 or y < 6 or y >= height - 6
            in_inner_border = 8 <= x < width - 8 and 8 <= y < height - 8 and (x < 12 or x >= width - 12 or y < 12 or y >= height - 12)

            if in_border:
                row.append(border_color)
            elif in_inner_border:
                row.append(bg_color)
            else:
                # 主区域 - 使用渐变效果
                center_dist = ((x - width//2) ** 2 + (y - height//2) ** 2) ** 0.5
                max_dist = ((width//2) ** 2 + (height//2) ** 2) ** 0.5
                factor = 1.0 - (center_dist / max_dist) * 0.1
                r = int(min(255, bg_color[0] * factor))
                g = int(min(255, bg_color[1] * factor))
                b = int(min(255, bg_color[2] * factor))
                row.append((r, g, b, 255))
        pixels.append(row)

    # 在中心位置绘制文字（简化处理：只创建带颜色的纯色图）
    # 实际游戏中应该使用字体渲染
    png_data = create_png(width, height, bg_color)

    with open(filename, 'wb') as f:
        f.write(png_data)

def get_tile_colors(tile_id):
    """根据牌ID获取颜色"""
    suit = tile_id // 10
    value = tile_id % 10

    # 边框颜色
    border = (40, 40, 40, 255)

    if suit == 0:  # 万子 - 红色系
        bg = (245, 245, 220, 255)  # 米色背景
        text = (231, 76, 60, 255)  # 红色
    elif suit == 1:  # 条子 - 绿色系
        bg = (245, 245, 220, 255)
        text = (39, 174, 96, 255)  # 绿色
    elif suit == 2:  # 筒子 - 蓝色系
        bg = (245, 245, 220, 255)
        text = (52, 152, 219, 255)  # 蓝色
    elif suit == 3:  # 花牌 - 紫色系
        bg = (245, 245, 220, 255)
        text = (155, 89, 182, 255)  # 紫色
    elif suit == 4:  # 鸡牌 - 金色系
        bg = (255, 215, 0, 255)  # 金色
        text = (139, 69, 19, 255)  # 棕色
    else:
        bg = (200, 200, 200, 255)
        text = (0, 0, 0, 255)

    return bg, text, border

def generate_all_tiles():
    """生成所有麻将牌PNG"""
    output_dir = os.path.dirname(os.path.abspath(__file__))

    # 生成万子牌 (W1-W9)
    characters_dir = os.path.join(output_dir, "Characters_PNG")
    os.makedirs(characters_dir, exist_ok=True)
    for i in range(1, 10):
        filename = os.path.join(characters_dir, f"W{i}.png")
        bg, text, border = get_tile_colors(0 * 10 + i)
        create_tile_png(filename, str(i), bg, text, border)
        print(f"生成: {filename}")

    # 生成条子牌 (B1-B9)
    bamboos_dir = os.path.join(output_dir, "Bamboos_PNG")
    os.makedirs(bamboos_dir, exist_ok=True)
    for i in range(1, 10):
        filename = os.path.join(bamboos_dir, f"B{i}.png")
        bg, text, border = get_tile_colors(1 * 10 + i)
        create_tile_png(filename, str(i), bg, text, border)
        print(f"生成: {filename}")

    # 生成筒子牌 (T1-T9)
    circles_dir = os.path.join(output_dir, "Circles_PNG")
    os.makedirs(circles_dir, exist_ok=True)
    for i in range(1, 10):
        filename = os.path.join(circles_dir, f"T{i}.png")
        bg, text, border = get_tile_colors(2 * 10 + i)
        create_tile_png(filename, str(i), bg, text, border)
        print(f"生成: {filename}")

    # 生成花牌 (H1-H4)
    flowers_dir = os.path.join(output_dir, "Flowers_PNG")
    os.makedirs(flowers_dir, exist_ok=True)
    flower_names = ["梅", "兰", "菊", "竹"]
    for i, name in enumerate(flower_names, 1):
        filename = os.path.join(flowers_dir, f"H{i}.png")
        bg, text, border = get_tile_colors(3 * 10 + i)
        create_tile_png(filename, name, bg, text, border)
        print(f"生成: {filename}")

    # 生成特殊牌
    special_dir = os.path.join(output_dir, "Special_PNG")
    os.makedirs(special_dir, exist_ok=True)

    # 鸡牌
    chicken_filename = os.path.join(special_dir, "Chicken.png")
    create_tile_png(chicken_filename, "鸡", (255, 165, 0, 255), (255, 255, 255, 255), (0, 0, 0, 255))
    print(f"生成: {chicken_filename}")

    # 金鸡
    golden_filename = os.path.join(special_dir, "GoldenChicken.png")
    create_tile_png(golden_filename, "金鸡", (255, 215, 0, 255), (139, 69, 19, 255), (139, 69, 19, 255))
    print(f"生成: {golden_filename}")

    print(f"\n完成! PNG麻将牌已生成到: {output_dir}")

def generate_button_png():
    """生成按钮PNG"""
    output_dir = os.path.dirname(os.path.abspath(__file__))
    button_dir = os.path.join(output_dir, "..", "UI", "Buttons_PNG")
    os.makedirs(button_dir, exist_ok=True)

    buttons = [
        ("btn_draw", "摸牌", (39, 174, 96, 255)),
        ("btn_discard", "出牌", (52, 152, 219, 255)),
        ("btn_peng", "碰", (231, 76, 60, 255)),
        ("btn_gang", "杠", (243, 156, 18, 255)),
        ("btn_hu", "胡", (231, 76, 60, 255)),
        ("btn_guo", "过", (149, 165, 166, 255)),
        ("btn_ting", "听", (155, 89, 182, 255)),
        ("btn_zhuaji", "捉鸡", (243, 156, 18, 255)),
    ]

    for filename, text, color in buttons:
        filepath = os.path.join(button_dir, f"{filename}.png")
        create_tile_png(filepath, text, color, (255, 255, 255, 255), (0, 0, 0, 255), 128, 64)
        print(f"生成按钮: {filepath}")

    print(f"\n按钮PNG已生成到: {button_dir}")

if __name__ == "__main__":
    print("=" * 60)
    print("开始生成麻将牌PNG图片...")
    generate_all_tiles()
    print("=" * 60)
    generate_button_png()
    print("=" * 60)
    print("\n所有PNG资源已生成!")
    print("注意：这些是占位符图片，实际游戏应使用专业设计的图形资源")
