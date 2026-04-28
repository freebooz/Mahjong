@echo off
chcp 65001 >nul
echo ============================================================================
echo                    MahjongGame 音效生成脚本
echo ============================================================================
echo.
echo 本脚本需要 Python 3.x 环境
echo.
echo 如果你没有安装 Python，请:
echo   1. 访问 https://www.python.org/downloads/
echo   2. 下载并安装 Python 3.x
echo   3. 安装时勾选 "Add Python to PATH"
echo   4. 重新运行本脚本
echo.
echo 如果已安装 Python，请确保在系统 PATH 中
echo.

:: 检查 Python 是否可用
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 Python，请先安装 Python
    echo 按任意键退出...
    pause >nul
    exit /b 1
)

echo [信息] 找到 Python 环境
echo.

:: 获取脚本所在目录
set SCRIPT_DIR=%~dp0
set OUTPUT_DIR=%SCRIPT_DIR%..

:: 运行 Python 脚本
echo [执行] 生成音效文件...
echo.
python "%SCRIPT_DIR%GenerateSounds.py" "%OUTPUT_DIR%"

echo.
echo [完成] 按任意键退出...
pause >nul
