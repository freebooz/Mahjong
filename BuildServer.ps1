#==============================================================================
# MahjongGame 服务器构建脚本 (PowerShell)
# BuildServer.ps1
#
# 使用方法:
#   1. 右键 -> Run with PowerShell
#   2. 或在 PowerShell 中执行: .\BuildServer.ps1
#
# 需要: UE5.1+ 已安装
#==============================================================================

param(
    [Parameter(Position=0)]
    [ValidateSet("Dev", "Ship", "Linux", "Clean", "Help")]
    [string]$Action = "Help"
)

$ErrorActionPreference = "Stop"
$PROJECT_NAME = "MahjongGame"
$PROJECT_DIR = Split-Path -Parent $MyInvocation.MyCommand.Path

# 颜色配置
function Write-ColorOutput {
    param([string]$Message, [string]$Color = "White")
    $colors = @{
        "Red" = [ConsoleColor]::Red
        "Green" = [ConsoleColor]::Green
        "Yellow" = [ConsoleColor]::Yellow
        "Cyan" = [ConsoleColor]::Cyan
        "White" = [ConsoleColor]::White
    }
    Write-Host $Message -ForegroundColor $colors[$Color]
}

# 查找 UE5 安装
function Find-UE5 {
    $uePaths = @(
        "C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat",
        "C:\Program Files\Epic Games\UE_5.2\Engine\Build\BatchFiles\Build.bat",
        "C:\Program Files\Epic Games\UE_5.1\Engine\Build\BatchFiles\Build.bat",
        "D:\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat",
        "D:\Epic Games\UE_5.2\Engine\Build\BatchFiles\Build.bat"
    )

    foreach ($path in $uePaths) {
        if (Test-Path $path) {
            return $path
        }
    }
    return $null
}

# 显示帮助
function Show-Help {
    Write-ColorOutput "========================================================================" "Cyan"
    Write-ColorOutput "                    MahjongGame 服务器构建脚本" "Cyan"
    Write-ColorOutput "========================================================================" "Cyan"
    Write-Host ""
    Write-ColorOutput "用法: .\BuildServer.ps1 [选项]" "White"
    Write-Host ""
    Write-ColorOutput "选项:" "Yellow"
    Write-Host "  Dev     - 编译 Windows Development 服务器"
    Write-Host "  Ship    - 编译 Windows Shipping 服务器"
    Write-Host "  Linux   - 编译 Linux 服务器 (需要交叉编译工具)"
    Write-Host "  Clean   - 清理并重新编译"
    Write-Host "  Help    - 显示此帮助信息"
    Write-Host ""
    Write-ColorOutput "示例:" "Yellow"
    Write-Host "  .\BuildServer.ps1 Dev     - 开发版本"
    Write-Host "  .\BuildServer.ps1 Ship    - 发布版本"
    Write-Host ""
}

# 清理函数
function Clear-Build {
    Write-ColorOutput "[清理] 删除中间文件..." "Yellow"

    $paths = @(
        "$PROJECT_DIR\Intermediate\Build",
        "$PROJECT_DIR\Binaries"
    )

    foreach ($path in $paths) {
        if (Test-Path $path) {
            Remove-Item -Path $path -Recurse -Force
            Write-ColorOutput "  已删除: $path" "White"
        }
    }

    Write-ColorOutput "[完成] 清理完成" "Green"
}

# 主函数
function Main {
    Write-ColorOutput "========================================================================" "Cyan"
    Write-ColorOutput "                    MahjongGame 服务器构建" "Cyan"
    Write-ColorOutput "========================================================================" "Cyan"
    Write-Host ""

    # 查找 UE5
    Write-ColorOutput "[检查] 查找 UE5 安装..." "Yellow"
    $ueBat = Find-UE5

    if (-not $ueBat) {
        Write-ColorOutput "[错误] 未找到 UE5 安装" "Red"
        Write-Host "请确保已安装 UE5.1、UE5.2 或 UE5.3"
        Write-Host "可以从 Epic Games Launcher 下载安装"
        return 1
    }

    Write-ColorOutput "[成功] 找到 UE5: $ueBat" "Green"
    Write-Host ""

    # 根据操作执行
    switch ($Action) {
        "Help" {
            Show-Help
            return 0
        }
        "Dev" {
            Write-ColorOutput "[编译] Windows Development 服务器" "Yellow"
            & $ueBat "$PROJECT_DIR\$PROJECT_NAME.uproject" Win64 Server Development -NoEngineSubscription -WaitMutex
        }
        "Ship" {
            Write-ColorOutput "[编译] Windows Shipping 服务器" "Yellow"
            & $ueBat "$PROJECT_DIR\$PROJECT_NAME.uproject" Win64 Server Shipping -NoEngineSubscription -WaitMutex
        }
        "Linux" {
            Write-ColorOutput "[编译] Linux 服务器 (交叉编译)" "Yellow"
            & $ueBat "$PROJECT_DIR\$PROJECT_NAME.uproject" Linux Server Shipping -NoEngineSubscription -WaitMutex
        }
        "Clean" {
            Clear-Build
            Write-Host ""
            Write-ColorOutput "[编译] Windows Development 服务器" "Yellow"
            & $ueBat "$PROJECT_DIR\$PROJECT_NAME.uproject" Win64 Server Development -NoEngineSubscription -WaitMutex
        }
    }

    Write-Host ""
    Write-ColorOutput "========================================================================" "Green"
    Write-ColorOutput "                         编译完成" "Green"
    Write-ColorOutput "========================================================================" "Green"
    Write-Host ""

    $outputPath = "$PROJECT_DIR\Binaries\Win64\$PROJECT_NAME\Binaries\Win64"
    if (Test-Path "$PROJECT_DIR\Binaries\Win64") {
        Write-ColorOutput "输出目录: $PROJECT_DIR\Binaries\Win64" "White"
        Get-ChildItem "$PROJECT_DIR\Binaries\Win64" -Filter "*.exe" -Recurse | ForEach-Object {
            Write-ColorOutput "  生成: $($_.FullName)" "White"
        }
    }

    Write-Host ""
    Write-Host "服务器运行方法:"
    Write-Host "  .\Binaries\Win64\$PROJECT_NAME\Binaries\Win64\$PROJECT_NAME-Server.exe -log"
    Write-Host ""

    return 0
}

# 执行
Main
