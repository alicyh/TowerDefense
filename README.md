# Tower Defense Game - Cocos2d-x 4.0

一个基于 Cocos2d-x 4.0 的简单塔防游戏，包含完整的测试场景！

## 🎮 快速开始 - 测试场景

项目包含一个 **TestScene**，专门用于快速测试和展示游戏效果！

### 测试场景特性：

✅ 预设的弯曲路径（显示为棕色）
✅ 预设的3个防御塔（绿色、橙色、紫色）
✅ 自动生成敌人（每3秒）
✅ 点击屏幕任意位置可添加更多塔
✅ 网格背景帮助对齐
✅ 实时FPS显示

### 测试场景操作：
- 程序启动后会自动进入测试场景
- 点击屏幕会轮流放置3种塔
- 观察塔攻击敌人的效果

## 📁 项目文件结构

```
TowerDefense/
├── Classes/
│   ├── AppDelegate.cpp/h      # 应用入口
│   ├── TestScene.cpp/h        # ⭐ 测试场景（当前使用）
│   ├── GameScene.cpp/h        # 完整游戏场景
│   ├── Tower.cpp/h            # 防御塔类
│   ├── Enemy.cpp/h            # 敌人类
│   └── AppMacros.h
├── Resources/                 # 资源文件
├── proj.win32/               # Windows平台项目
├── proj.android/             # Android平台项目
├── CMakeLists.txt            # CMake构建配置
├── build.bat                 # ⭐ Windows快速构建脚本
└── README.md
```

## 🛠️ 构建和运行（Windows）

### 方法一：使用 CMake（推荐）

**前提条件：**
- Visual Studio 2019/2022
- CMake 3.6+
- Cocos2d-x 4.0 完整源码

**步骤：**

1. **首先构建 Cocos2d-x 引擎**
   ```bash
   cd e:\ProjCore\游戏\cocos2d-x-4.0
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A Win32
   # 用 Visual Studio 打开 build/Cocos2d-x.sln 并构建
   ```

2. **然后构建塔防游戏**
   ```bash
   cd e:\ProjCore\游戏\cocos2d-x-4.0\TowerDefense
   build.bat
   ```

3. **运行游戏**
   - 在 Visual Studio 中打开 `TowerDefense/build/TowerDefense.sln`
   - 选择 Release 或 Debug 配置
   - 按 F5 或点击"本地 Windows 调试器"

### 方法二：使用 cocos 命令行工具

如果你配置了 cocos 命令行工具：
```bash
cd TowerDefense
cocos run -p win32
```

## 🎯 游戏功能

### 防御塔类型

| 类型 | 颜色 | 射程 | 伤害 | 攻击速度 | 成本 | 特点 |
|------|------|------|------|----------|------|------|
| 塔1 | 绿色 | 150 | 20 | 1.0秒 | 50G | 平衡型 |
| 塔2 | 橙色 | 100 | 40 | 0.8秒 | 100G | 高伤害 |
| 塔3 | 紫色 | 200 | 15 | 1.5秒 | 75G | 远程型 |

### 敌人类型

| 类型 | 颜色 | 生命值 | 速度 | 奖励 | 特点 |
|------|------|--------|------|------|------|
| 敌1 | 红色 | 100 | 100 | 10G | 平衡型 |
| 敌2 | 蓝色 | 200 | 80 | 25G | 坦克型 |
| 敌3 | 黄色 | 50 | 150 | 5G | 快速型 |

## 🎮 完整游戏玩法 (GameScene)

如果想玩完整的塔防游戏，可以修改 `AppDelegate.cpp`，将 `TestScene` 改回 `GameScene`：

```cpp
// 在 AppDelegate.cpp 中
#include "GameScene.h"  // 改为这个
// #include "TestScene.h"

// ...

auto scene = GameScene::scene();  // 改为这个
// auto scene = TestScene::scene();
```

完整游戏包含：
- 金币系统
- 生命值系统
- 波次系统
- 塔选择菜单
- 游戏结束界面

## 🔧 开发者说明

### 切换场景

编辑 `AppDelegate.cpp` 来切换使用的场景：
- `TestScene::scene()` - 测试场景（预设内容，适合快速演示）
- `GameScene::scene()` - 完整游戏（需要购买塔，有胜负条件）

### 添加新功能

- 新塔类型：修改 `Tower.cpp` 中的 `initTower` 函数
- 新敌人类型：修改 `Enemy.cpp` 中的 `initEnemy` 函数
- 新路径：修改 `GameScene.cpp` 或 `TestScene.cpp` 中的 `initPath` 函数

## 📝 注意事项

1. 确保先成功构建 Cocos2d-x 引擎库
2. CMake 生成项目时选择 Win32 (x86) 架构
3. 资源文件需要放在正确的 Resources 目录下
4. 首次构建可能需要较长时间

## 📄 许可证

本项目使用与 Cocos2d-x 相同的许可证。
