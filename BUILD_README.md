# MahjongGame 服务器构建指南

## 快速开始

### 方法 1: 使用批处理脚本 (推荐 Windows)

1. 双击 `BuildServer.bat`
2. 选择编译选项:
   - `1` - Windows Development 版本 (开发用)
   - `2` - Windows Shipping 版本 (发布用)
   - `3` - Linux 服务器 (交叉编译)
   - `4` - 清理并重新编译

### 方法 2: 使用 PowerShell

```powershell
# 开发版本
.\BuildServer.ps1 Dev

# 发布版本
.\BuildServer.ps1 Ship

# 清理并重新编译
.\BuildServer.ps1 Clean
```

### 方法 3: 使用 UE5 编辑器

1. 打开 UE5 编辑器
2. File -> Package Project -> Build Target -> Server
3. 选择 Windows 或 Linux

### 方法 4: 命令行直接编译

```bash
# 找到 UE5 安装目录
cd "C:\Program Files\Epic Games\UE_5.3"

# Development 版本
Engine\Build\BatchFiles\Build.bat MahjongGame.uproject Win64 Server Development -WaitMutex

# Shipping 版本
Engine\Build\BatchFiles\Build.bat MahjongGame.uproject Win64 Server Shipping -WaitMutex
```

---

## 编译选项说明

| 选项 | 用途 | 说明 |
|------|------|------|
| Development | 开发调试 | 包含调试符号，适合开发调试 |
| Shipping | 发布版本 | 优化后的发布版本，适合生产环境 |
| Linux | Linux 服务器 | 交叉编译适用于 Linux 服务器 |

---

## 输出文件

编译成功后，服务器可执行文件位于:

```
Binaries\
└── Win64\
    └── MahjongGameServer.exe    # Windows 服务器
    └── MahjongGame\            # Linux 服务器 (交叉编译)
        └── Binaries\
            └── Linux\
                └── MahjongGameServer
```

---

## 运行服务器

### Windows 服务器

```bash
# 进入输出目录
cd Binaries\Win64

# 运行服务器 (显示日志)
.\MahjongGameServer.exe -log

# 后台运行
.\MahjongGameServer.exe -server -unattended
```

### Linux 服务器

```bash
# 设置执行权限
chmod +x MahjongGameServer

# 运行服务器
./MahjongGameServer -log
```

---

## 服务器配置

### 配置文件位置

服务器默认读取以下配置:

- `Config/DefaultEngine.ini` - 引擎配置
- `Config/DefaultGame.ini` - 游戏配置

### 启动参数

| 参数 | 说明 |
|------|------|
| `-log` | 显示日志窗口 |
| `-server` | 服务器模式 |
| `-unattended` | 无头模式 (无GUI) |
| `-Port=7777` | 指定端口 (默认 7777) |

### 示例

```bash
# 基本启动
.\MahjongGameServer.exe -log

# 指定端口
.\MahjongGameServer.exe -log -Port=8888

# 无头模式
.\MahjongGameServer.exe -server -unattended
```

---

## 常见问题

### Q: 编译失败 "UBT error"

**A:** 确保使用正确版本的 UE5。检查 `MahjongGame.uproject` 中的 Engine Association。

### Q: 找不到 Build.bat

**A:** 确保 UE5 已正确安装，且路径在系统 PATH 中。

### Q: 链接器错误 "unresolved external"

**A:** 清理中间文件后重新编译:
```powershell
.\BuildServer.ps1 Clean
```

---

## 部署到 Linux 服务器

1. 在 Windows 上交叉编译 Linux 版本:
   ```powershell
   .\BuildServer.ps1 Linux
   ```

2. 使用 FTP/SCP 上传到 Linux 服务器:
   ```bash
   scp -r Binaries/Linux root@your-server:/path/to/mahjong/
   ```

3. 在 Linux 服务器上运行:
   ```bash
   chmod +x MahjongGameServer
   ./MahjongGameServer -log
   ```

---

## 网络端口

| 端口 | 用途 | 协议 |
|------|------|------|
| 7777 | 游戏流量 | UDP |
| 7778 | 端口 (备用) | UDP |
| 27015 | Query 端口 | UDP |

确保服务器防火墙允许这些端口的流量。
