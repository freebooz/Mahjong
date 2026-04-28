@echo off
REM 贵阳捉鸡麻将 开发模式启动脚本
REM 先启动服务器，然后启动2个客户端

echo ============================================
echo 贵阳捉鸡麻将 - 开发模式启动
echo ============================================

set UE_PATH=D:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe
set PROJECT_PATH=H:\MahjongGame\MahjongGame.uproject

echo.
echo [1/3] 启动服务器模式...
start "MahjongServer" "%UE_PATH%" "%PROJECT_PATH%" -server -log -Windowed -ResX=1280 -ResY=720

echo 等待服务器启动...
timeout /t 5 /nobreak > nul

echo.
echo [2/3] 启动客户端 1 ...
start "MahjongClient1" "%UE_PATH%" "%PROJECT_PATH%" -game -log -Windowed -ResX=960 -ResY=640 -player

echo 等待客户端1启动...
timeout /t 3 /nobreak > nul

echo.
echo [3/3] 启动客户端 2 ...
start "MahjongClient2" "%UE_PATH%" "%PROJECT_PATH%" -game -log -Windowed -ResX=960 -ResY=640 -player

echo.
echo ============================================
echo 启动完成！
echo - 服务器窗口: MahjongServer
echo - 客户端1窗口: MahjongClient1
echo - 客户端2窗口: MahjongClient2
echo ============================================
pause
