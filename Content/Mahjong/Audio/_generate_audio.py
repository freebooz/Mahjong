#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
麻将游戏音效生成脚本
使用白噪声和正弦波生成占位符音效

使用方法:
    python _generate_audio.py

依赖:
    pip install numpy scipy

注意:
    这只是生成占位符音效用于测试，实际游戏应使用专业录制的音效
"""

import os
import struct
import wave
import math
import random

try:
    import numpy as np
    from scipy.io import wavfile
    HAS_AUDIO_LIBS = True
except ImportError:
    HAS_AUDIO_LIBS = False

# 配置
OUTPUT_DIR = os.path.dirname(os.path.abspath(__file__))
SAMPLE_RATE = 44100

def generate_click_sound(filename, duration=0.15, frequency=800):
    """生成点击音效"""
    if not HAS_AUDIO_LIBS:
        create_empty_wav(filename, duration)
        return

    t = np.linspace(0, duration, int(SAMPLE_RATE * duration), False)
    # 添加频率衰减
    envelope = np.exp(-t * 20)
    signal = np.sin(2 * np.pi * frequency * t) * envelope

    # 添加一些噪声
    noise = np.random.normal(0, 0.05, len(signal))
    signal = signal + noise

    # 归一化
    signal = np.int16(signal * 32767 * 0.8)
    wavfile.write(filename, SAMPLE_RATE, signal)

def generate_impact_sound(filename, duration=0.3, base_freq=150):
    """生成撞击音效"""
    if not HAS_AUDIO_LIBS:
        create_empty_wav(filename, duration)
        return

    t = np.linspace(0, duration, int(SAMPLE_RATE * duration), False)
    # 冲击声 - 低频为主
    envelope = np.exp(-t * 15)
    signal = np.sin(2 * np.pi * base_freq * t) * envelope

    # 添加泛音
    for harmonic in [2, 3, 4]:
        signal += np.sin(2 * np.pi * base_freq * harmonic * t) * envelope / harmonic

    # 添加噪声
    noise = np.random.normal(0, 0.1, len(signal))
    signal = signal + noise

    signal = np.int16(signal * 32767 * 0.8)
    wavfile.write(filename, SAMPLE_RATE, signal)

def generate_celebration_sound(filename, duration=1.5):
    """生成庆祝音效（胡牌）"""
    if not HAS_AUDIO_LIBS:
        create_empty_wav(filename, duration)
        return

    t = np.linspace(0, duration, int(SAMPLE_RATE * duration), False)

    # 上升音调
    freq_start = 400
    freq_end = 800
    frequency = freq_start + (freq_end - freq_start) * (t / duration)

    envelope = np.ones_like(t)
    envelope[:int(len(t)*0.1)] = np.linspace(0, 1, int(len(t)*0.1))
    envelope[-int(len(t)*0.3):] = np.linspace(1, 0, int(len(t)*0.3))

    signal = np.sin(2 * np.pi * frequency * t) * envelope

    # 添加和弦
    signal += np.sin(2 * np.pi * frequency * 1.25 * t) * envelope * 0.5  # 大三度
    signal += np.sin(2 * np.pi * frequency * 1.5 * t) * envelope * 0.3   # 纯五度

    signal = np.int16(signal * 32767 * 0.6)
    wavfile.write(filename, SAMPLE_RATE, signal)

def generate_button_sound(filename, duration=0.1):
    """生成按钮音效"""
    if not HAS_AUDIO_LIBS:
        create_empty_wav(filename, duration)
        return

    t = np.linspace(0, duration, int(SAMPLE_RATE * duration), False)
    envelope = np.exp(-t * 30)

    # 高频点击
    signal = np.sin(2 * np.pi * 1200 * t) * envelope
    signal += np.sin(2 * np.pi * 1800 * t) * envelope * 0.5

    signal = np.int16(signal * 32767 * 0.7)
    wavfile.write(filename, SAMPLE_RATE, signal)

def generate_bgm_placeholder(filename, duration=10):
    """生成BGM占位符（简单的循环音调）"""
    if not HAS_AUDIO_LIBS:
        create_empty_wav(filename, duration)
        return

    # 简单的循环背景音
    t = np.linspace(0, duration, int(SAMPLE_RATE * duration), False)

    # 低频持续音
    signal = np.sin(2 * np.pi * 100 * t) * 0.3

    # 添加一些变化
    signal += np.sin(2 * np.pi * 150 * t + np.sin(t * 0.5) * 0.5) * 0.2

    # 柔和的包络
    envelope = np.ones_like(t)
    fade_samples = int(SAMPLE_RATE * 0.5)
    envelope[:fade_samples] = np.linspace(0, 1, fade_samples)
    envelope[-fade_samples:] = np.linspace(1, 0, fade_samples)

    signal = signal * envelope
    signal = np.int16(signal * 32767 * 0.5)
    wavfile.write(filename, SAMPLE_RATE, signal)

def create_empty_wav(filename, duration):
    """创建空白WAV文件作为占位符"""
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(SAMPLE_RATE)
        num_samples = int(SAMPLE_RATE * duration)
        # 写入静音数据
        for _ in range(num_samples):
            wav_file.writeframes(struct.pack('h', 0))

def generate_all_audio():
    """生成所有占位符音效"""
    print("开始生成占位符音效...")

    # SFX/Tiles
    tile_dir = os.path.join(OUTPUT_DIR, "SFX", "Tiles")
    os.makedirs(tile_dir, exist_ok=True)

    for i in range(1, 4):
        filename = os.path.join(tile_dir, f"tile_draw_{i}.wav")
        generate_click_sound(filename, duration=0.15, frequency=600+i*100)
        print(f"生成: {filename}")

    # SFX/Actions
    actions_dir = os.path.join(OUTPUT_DIR, "SFX", "Actions")
    os.makedirs(actions_dir, exist_ok=True)

    actions = [
        ("action_peng.wav", 0.3, 200),
        ("action_gang.wav", 0.4, 150),
        ("action_chow.wav", 0.25, 400),
        ("action_discard.wav", 0.2, 300),
        ("action_ting.wav", 0.5, 600),
    ]

    for name, dur, freq in actions:
        filename = os.path.join(actions_dir, name)
        generate_impact_sound(filename, duration=dur, base_freq=freq)
        print(f"生成: {filename}")

    # 胡牌音效
    filename = os.path.join(actions_dir, "action_hu.wav")
    generate_celebration_sound(filename)
    print(f"生成: {filename}")

    # SFX/UI
    ui_dir = os.path.join(OUTPUT_DIR, "SFX", "UI")
    os.makedirs(ui_dir, exist_ok=True)

    ui_sounds = [
        ("btn_click.wav", 0.1),
        ("btn_hover.wav", 0.08),
        ("btn_open.wav", 0.3),
        ("btn_close.wav", 0.25),
        ("notification.wav", 0.5),
        ("error.wav", 0.4),
        ("success.wav", 0.6),
    ]

    for name, dur in ui_sounds:
        filename = os.path.join(ui_dir, name)
        generate_button_sound(filename, duration=dur)
        print(f"生成: {filename}")

    # BGM
    bgm_dir = os.path.join(OUTPUT_DIR, "BGM")
    os.makedirs(bgm_dir, exist_ok=True)

    bgm_files = ["bgm_lobby.wav", "bgm_game.wav", "bgm_guiyang.wav", "bgm_login.wav"]
    for name in bgm_files:
        filename = os.path.join(bgm_dir, name)
        generate_bgm_placeholder(filename, duration=10)
        print(f"生成: {filename}")

    print("\n所有占位符音效已生成!")
    print(f"输出目录: {OUTPUT_DIR}")
    print("\n注意: 这些只是占位符音效，实际游戏应使用专业录制的音效")

if __name__ == "__main__":
    if not HAS_AUDIO_LIBS:
        print("警告: numpy和scipy未安装，将创建空白WAV占位符")
        print("安装命令: pip install numpy scipy")

    print(f"输出目录: {OUTPUT_DIR}")
    print("生成音效到子目录...")

    generate_all_audio()
