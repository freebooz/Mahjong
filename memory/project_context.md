# MahjongGame 项目上下文

## 项目基本信息
- **项目路径**: E:\poject\feebooz\MahjongGame\MahjongGame
- **引擎**: UE5.7.1 source build (D:\UnrealEngine-5.7.1-release)
- **模块**: MahjongGame

## 三层架构（最终结构）

```
Source/MahjongGame/
├── Core/                      # 游戏通用基类
│   ├── Interfaces/           # 接口
│   ├── Types/                # 类型定义
│   ├── GameState/            # 游戏状态基类
│   ├── PlayerState/          # 玩家状态基类
│   └── UI/Common/            # 通用UI组件
│
├── Gameplay/                  # 麻将游戏通用
│   ├── Account/              # 账号服务
│   ├── Monitor/              # 监控服务
│   ├── Network/              # 网络管理
│   ├── Utils/                # 工具类
│   ├── Actor/                # Actor实体
│   ├── AI/                   # AI系统
│   ├── Components/           # 组件
│   ├── Game/                 # 游戏控制器
│   ├── Lobby/               # 大厅
│   ├── Login/               # 登录
│   ├── Modes/               # 游戏模式
│   ├── Room/                # 房间
│   ├── Rules/               # 规则
│   └── UI/Mahjong/          # 麻将UI
│
└── Variants/                  # 地域玩法
    ├── Guiyang/Rules/       # 贵阳麻将规则
    └── UI/Regional/         # 地域UI
```

## 关键修复记录

### UE5.7 兼容性问题
1. **Delegate绑定**: 必须使用 `BindUObject` 而非 `CreateUObject`
2. **ConstructorHelpers**: UE5.7 中 TObjectPtr 不兼容，已移除
3. **ModuleRules属性**: bUseUnityBuild/bUseLLVMThinLTO 等在 UE5.7 不存在

### 资产注册表错误
- 服务器启动时报错 `Failed to load premade asset registry`
- 需要通过 UE 编辑器 `File → Cook Content → Windows Server` 修复

## 近期提交
- `5ea05c6` - 完成三层架构重组
- `11db5da` - 删除废弃测试文件
- `b5ed819` - 初始三层目录结构

## 构建命令
```bash
# 编译服务器
Build.bat Server Development Win64

# 服务器启动
Binaries/Win64/MahjongGameServer.exe -log
```

## 待处理
- 资产烹饪（需要手动通过UE编辑器执行）
