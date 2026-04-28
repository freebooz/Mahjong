# 贵阳捉鸡麻将 开发模式启动脚本 (PowerShell)
# 先启动服务器端，然后启动2个客户端

param(
    [string]$UEPath = "D:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe",
    [string]$ProjectPath = "H:\MahjongGame\MahjongGame.uproject"
)

Write-Host "============================================" -ForegroundColor Cyan
Write-Host "贵阳捉鸡麻将 - 开发模式启动" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan

# 服务器参数 - 窗口模式，日志输出
$ServerArgs = @(
    "`"$ProjectPath`"",
    "-server",
    "-log",
    "-Windowed",
    "-ResX=1280",
    "-ResY=720",
    "-NoSound"
)

# 客户端1参数
$Client1Args = @(
    "`"$ProjectPath`"",
    "-game",
    "-log",
    "-Windowed",
    "-ResX=960",
    "-ResY=640",
    "-WinX=100",
    "-WinY=100"
)

# 客户端2参数
$Client2Args = @(
    "`"$ProjectPath`"",
    "-game",
    "-log",
    "-Windowed",
    "-ResX=960",
    "-ResY=640",
    "-WinX=1100",
    "-WinY=100"
)

Write-Host ""
Write-Host "[1/3] 启动服务器模式..." -ForegroundColor Yellow

$ServerProcess = Start-Process -FilePath $UEPath -ArgumentList $ServerArgs -PassThru -WindowStyle Normal

Write-Host "服务器进程ID: $($ServerProcess.Id)" -ForegroundColor Green
Write-Host "等待服务器启动..." -ForegroundColor Gray
Start-Sleep -Seconds 5

Write-Host ""
Write-Host "[2/3] 启动客户端 1 ..." -ForegroundColor Yellow

$Client1Process = Start-Process -FilePath $UEPath -ArgumentList $Client1Args -PassThru -WindowStyle Normal

Write-Host "客户端1进程ID: $($Client1Process.Id)" -ForegroundColor Green
Write-Host "等待客户端1启动..." -ForegroundColor Gray
Start-Sleep -Seconds 3

Write-Host ""
Write-Host "[3/3] 启动客户端 2 ..." -ForegroundColor Yellow

$Client2Process = Start-Process -FilePath $UEPath -ArgumentList $Client2Args -PassThru -WindowStyle Normal

Write-Host "客户端2进程ID: $($Client2Process.Id)" -ForegroundColor Green

Write-Host ""
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "启动完成！" -ForegroundColor Green
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "- 服务器窗口: MahjongServer (PID: $($ServerProcess.Id))" -ForegroundColor White
Write-Host "- 客户端1窗口: MahjongClient1 (PID: $($Client1Process.Id))" -ForegroundColor White
Write-Host "- 客户端2窗口: MahjongClient2 (PID: $($Client2Process.Id))" -ForegroundColor White
Write-Host ""
Write-Host "按任意键退出此脚本（进程将继续运行）..." -ForegroundColor Gray
Read-Host