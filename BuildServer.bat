@echo off
chcp 65001 >nul
echo ============================================================================
echo                    MahjongGame 服务器构建脚本
echo ============================================================================
echo.
echo 本脚本用于编译独立服务器版本
echo.

:: 检查是否以管理员权限运行（某些编译操作需要）
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [提示] 建议以管理员权限运行以避免权限问题
)

:: 获取脚本所在目录
set SCRIPT_DIR=%~dp0
set PROJECT_DIR=%SCRIPT_DIR%
set PROJECT_NAME=MahjongGame

:: 检查 UE5 安装
echo [检查] 查找 UE5 安装目录...

:: 常见 UE5 安装路径
set UE_PATHS=^
    "C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat",^
    "C:\Program Files\Epic Games\UE_5.2\Engine\Build\BatchFiles\Build.bat",^
    "C:\Program Files\Epic Games\UE_5.1\Engine\Build\BatchFiles\Build.bat",^
    "D:\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat",^
    "D:\Epic Games\UE_5.2\Engine\Build\BatchFiles\Build.bat",^
    "%PROGRAMFILES%\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat",^
    "%PROGRAMFILES%\Epic Games\UE_5.2\Engine\Build\BatchFiles\Build.bat"

set UE_BUILD_BAT=

for %%p in (%UE_PATHS%) do (
    if exist %%p (
        set "UE_BUILD_BAT=%%p"
        goto :found_ue
    )
)

:found_ue
if "%UE_BUILD_BAT%"=="" (
    echo [错误] 未找到 UE5 安装
    echo 请确保已安装 UE5.1、UE5.2 或 UE5.3
    echo 可以从 Epic Games Launcher 下载安装
    echo.
    echo 按任意键退出...
    pause >nul
    exit /b 1
)

echo [成功] 找到 UE5: %UE_BUILD_BAT%
echo.

:: 设置输出目录
set OUTPUT_DIR=%PROJECT_DIR%Binaries\Win64

:: 创建输出目录
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

echo ============================================================================
echo 选项:
echo   1. 编译 Windows 服务器 (Development)
echo   2. 编译 Windows 服务器 (Shipping)
echo   3. 编译 Linux 服务器 (Cross-Compile)
echo   4. 清理并重新编译 (Development)
echo   0. 退出
echo ============================================================================
echo.

set /p CHOICE=请选择 (0-4):

if "%CHOICE%"=="1" goto :build_dev
if "%CHOICE%"=="2" goto :build_shipping
if "%CHOICE%"=="3" goto :build_linux
if "%CHOICE%"=="4" goto :build_clean
if "%CHOICE%"=="0" goto :exit

echo [错误] 无效选项
goto :exit

:build_dev
echo.
echo [编译] Windows Development 服务器...
echo.
call "%UE_BUILD_BAT%" "%PROJECT_NAME%.uproject" Win64 Server Development -NoEngineSubscription -WaitMutex -FromMsBuild
goto :done

:build_shipping
echo.
echo [编译] Windows Shipping 服务器...
echo.
call "%UE_BUILD_BAT%" "%PROJECT_NAME%.uproject" Win64 Server Shipping -NoEngineSubscription -WaitMutex -FromMsBuild
goto :done

:build_linux
echo.
echo [编译] Linux 服务器 (需要安装 Linux 构建工具)...
echo.

:: 检查 Linux 交叉编译工具
where clang >nul 2>&1
if %errorlevel% neq 0 (
    echo [警告] 未找到 clang 编译器
    echo Linux 交叉编译需要安装:
    echo   1. Visual Studio 2022 + Linux C++ workload
    echo   2. Or UE5 Linux 交叉编译工具链
    echo.
    echo 建议使用 Windows 服务器进行测试
    echo.
)

set /p CONFIRM=是否继续? (y/n):
if /i not "%CONFIRM%"=="y" goto :exit

call "%UE_BUILD_BAT%" "%PROJECT_NAME%.uproject" Linux Server Shipping -NoEngineSubscription -WaitMutex -FromMsBuild
goto :done

:build_clean
echo.
echo [清理] 清理中间文件...
echo.
if exist "%PROJECT_DIR%Intermediate" rmdir /s /q "%PROJECT_DIR%Intermediate\Build"
if exist "%PROJECT_DIR%Binaries" rmdir /s /q "%PROJECT_DIR%Binaries"
echo [完成] 清理完成
echo.
echo [重新编译] Windows Development 服务器...
echo.
call "%UE_BUILD_BAT%" "%PROJECT_NAME%.uproject" Win64 Server Development -NoEngineSubscription -WaitMutex -FromMsBuild
goto :done

:done
echo.
echo ============================================================================
echo 编译完成
echo ============================================================================
echo.
echo 输出目录: %OUTPUT_DIR%
echo.
echo 服务器运行方法:
echo   Binaries\Win64\MahjongGameServer.exe -log
echo.
echo 按任意键退出...
pause >nul
exit /b 0

:exit
echo.
echo 再见!
exit /b 0
