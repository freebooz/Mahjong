#==============================================================================
# MahjongGame 音效生成脚本
# GenerateSounds.py
#
# 使用方法:
#   python GenerateSounds.py [输出目录]
#
# 生成以下音效文件 (WAV 格式):
#   UI 音效: ButtonClick, Confirm, Cancel, Success, Error
#   麻将音效: DrawTile, DiscardTile, Chow, Pong, Kong, Hu
#==============================================================================

import os
import struct
import math
import sys
import wave
import argparse

# 音频参数
SAMPLE_RATE = 44100
BIT_DEPTH = 16
CHANNELS = 1

# 音效持续时间（秒）
DURATIONS = {
    'ButtonClick': 0.1,
    'Confirm': 0.2,
    'Cancel': 0.15,
    'Success': 0.4,
    'Error': 0.3,
    'DrawTile': 0.15,
    'DiscardTile': 0.12,
    'Chow': 0.2,
    'Pong': 0.2,
    'Kong': 0.25,
    'Hu': 0.5,
}


def generate_sine_wave(frequency, duration, volume=0.5, fade_out=True):
    """生成正弦波音频数据"""
    num_samples = int(SAMPLE_RATE * duration)
    samples = []

    for i in range(num_samples):
        t = i / SAMPLE_RATE
        # 正弦波
        value = volume * math.sin(2 * math.pi * frequency * t)

        # 添加淡出效果
        if fade_out:
            fade_start = int(num_samples * 0.7)
            if i > fade_start:
                fade_ratio = (num_samples - i) / (num_samples - fade_start)
                value *= fade_ratio

        # 转换为 16 位整数
        sample = int(value * 32767)
        sample = max(-32768, min(32767, sample))
        samples.append(sample)

    return samples


def generate_click_sound(duration=0.1):
    """生成点击音效 - 短促的正弦波"""
    # 使用较高频率的正弦波
    return generate_sine_wave(800, duration, volume=0.4)


def generate_confirm_sound(duration=0.2):
    """生成确认音效 - 升调"""
    samples = []
    # 前半段较低频率
    samples.extend(generate_sine_wave(400, duration * 0.5, volume=0.4))
    # 后半段较高频率
    samples.extend(generate_sine_wave(600, duration * 0.5, volume=0.4))
    return samples


def generate_cancel_sound(duration=0.15):
    """生成取消音效 - 降调"""
    samples = []
    # 前半段较高频率
    samples.extend(generate_sine_wave(500, duration * 0.5, volume=0.4))
    # 后半段较低频率
    samples.extend(generate_sine_wave(300, duration * 0.5, volume=0.4))
    return samples


def generate_success_sound(duration=0.4):
    """生成成功音效 - 两声升调"""
    samples = []
    samples.extend(generate_sine_wave(500, 0.15, volume=0.4))
    samples.extend([0] * int(SAMPLE_RATE * 0.05))  # 短暂停顿
    samples.extend(generate_sine_wave(700, 0.15, volume=0.4))
    return samples


def generate_error_sound(duration=0.3):
    """生成错误音效 - 下降的蜂鸣声"""
    samples = []
    # 频率从高到低
    num_samples = int(SAMPLE_RATE * duration)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        # 线性下降的频率
        frequency = 400 - (t / duration) * 150
        value = 0.3 * math.sin(2 * math.pi * frequency * t)
        sample = int(value * 32767)
        sample = max(-32768, min(32767, sample))
        samples.append(sample)
    return samples


def generate_draw_tile_sound(duration=0.15):
    """生成摸牌音效 - 轻柔的滑动声"""
    samples = []
    # 使用噪音和低频正弦波混合
    import random
    num_samples = int(SAMPLE_RATE * duration)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        # 噪音成分
        noise = random.uniform(-0.1, 0.1)
        # 低频正弦波
        sine = 0.2 * math.sin(2 * math.pi * 200 * t)
        # 包络
        envelope = 1.0 if i < num_samples * 0.8 else (num_samples - i) / (num_samples * 0.2)
        value = (noise + sine) * envelope * 0.5
        sample = int(value * 32767)
        sample = max(-32768, min(32767, sample))
        samples.append(sample)
    return samples


def generate_discard_tile_sound(duration=0.12):
    """生成出牌音效 - 清脆的点击"""
    samples = []
    # 高频短促的正弦波
    samples.extend(generate_sine_wave(1200, 0.03, volume=0.3))
    samples.extend([0] * int(SAMPLE_RATE * 0.02))
    samples.extend(generate_sine_wave(800, 0.05, volume=0.3))
    return samples


def generate_chow_sound(duration=0.2):
    """生成吃牌音效 - 两声短促"""
    samples = []
    samples.extend(generate_sine_wave(600, 0.08, volume=0.35))
    samples.extend([0] * int(SAMPLE_RATE * 0.04)
    samples.extend(generate_sine_wave(600, 0.08, volume=0.35))
    return samples


def generate_pong_sound(duration=0.2):
    """生成碰牌音效 - 两声有力"""
    samples = []
    samples.extend(generate_sine_wave(400, 0.1, volume=0.4))
    samples.extend([0] * int(SAMPLE_RATE * 0.03)
    samples.extend(generate_sine_wave(400, 0.1, volume=0.4))
    return samples


def generate_kong_sound(duration=0.25):
    """生成杠牌音效 - 三声有力"""
    samples = []
    samples.extend(generate_sine_wave(350, 0.08, volume=0.45))
    samples.extend([0] * int(SAMPLE_RATE * 0.03))
    samples.extend(generate_sine_wave(350, 0.08, volume=0.45))
    samples.extend([0] * int(SAMPLE_RATE * 0.03))
    samples.extend(generate_sine_wave(350, 0.08, volume=0.45))
    return samples


def generate_hu_sound(duration=0.5):
    """生成胡牌音效 - 胜利音乐"""
    samples = []
    # 升调序列
    frequencies = [400, 500, 600, 800, 1000]
    for freq in frequencies:
        samples.extend(generate_sine_wave(freq, 0.1, volume=0.4))
        samples.extend([0] * int(SAMPLE_RATE * 0.02))
    return samples


def save_wav_file(filename, samples):
    """保存为 WAV 文件"""
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(CHANNELS)
        wav_file.setsampwidth(BIT_DEPTH // 8)
        wav_file.setframerate(SAMPLE_RATE)

        for sample in samples:
            wav_file.writeframes(struct.pack('<h', sample))

    print(f"  已生成: {filename}")


def create_sound(sound_name, generator_func, duration):
    """创建单个音效文件"""
    samples = generator_func(duration)
    return samples


def main():
    # 解析命令行参数
    parser = argparse.ArgumentParser(description='MahjongGame 音效生成脚本')
    parser.add_argument('output_dir', nargs='?', default='.',
                        help='输出目录路径')
    args = parser.parse_args()

    output_dir = args.output_dir

    # 创建输出目录
    ui_dir = os.path.join(output_dir, 'Audio', 'UI')
    mahjong_dir = os.path.join(output_dir, 'Audio', 'Mahjong')

    os.makedirs(ui_dir, exist_ok=True)
    os.makedirs(mahjong_dir, exist_ok=True)

    print("=" * 60)
    print("MahjongGame 音效生成器")
    print("=" * 60)
    print(f"\n输出目录: {output_dir}")
    print("\n生成 UI 音效...")

    # 生成 UI 音效
    ui_sounds = {
        'ButtonClick': (generate_click_sound, 0.1),
        'Confirm': (generate_confirm_sound, 0.2),
        'Cancel': (generate_cancel_sound, 0.15),
        'Success': (generate_success_sound, 0.4),
        'Error': (generate_error_sound, 0.3),
    }

    for name, (func, duration) in ui_sounds.items():
        filepath = os.path.join(ui_dir, f'{name}.wav')
        samples = func(duration)
        save_wav_file(filepath, samples)

    print("\n生成麻将游戏音效...")

    # 生成麻将音效
    mahjong_sounds = {
        'DrawTile': (generate_draw_tile_sound, 0.15),
        'DiscardTile': (generate_discard_tile_sound, 0.12),
        'Chow': (generate_chow_sound, 0.2),
        'Pong': (generate_pong_sound, 0.2),
        'Kong': (generate_kong_sound, 0.25),
        'Hu': (generate_hu_sound, 0.5),
    }

    for name, (func, duration) in mahjong_sounds.items():
        filepath = os.path.join(mahjong_dir, f'{name}.wav')
        samples = func(duration)
        save_wav_file(filepath, samples)

    print("\n" + "=" * 60)
    print("音效生成完成!")
    print("=" * 60)
    print(f"\n请将 Audio 文件夹中的 .wav 文件导入 UE5:")
    print(f"  1. 打开 UE5 编辑器")
    print(f"  2. 将 Audio 文件夹拖入 Content Browser")
    print(f"  3. UE5 会自动创建 Sound Cue 资源")
    print(f"  4. 重命名 Cue 资源为 {name}_Cue 格式")
    print("\n示例:")
    print(f"  ButtonClick.wav → ButtonClick_Cue")
    print(f"  DrawTile.wav → DrawTile_Cue")
    print("\n代码中的引用路径:")
    print(f"  SoundCue'/Game/Audio/UI/ButtonClick_Cue.ButtonClick_Cue'")
    print(f"  SoundCue'/Game/Audio/Mahjong/DrawTile_Cue.DrawTile_Cue'")


if __name__ == '__main__':
    main()
