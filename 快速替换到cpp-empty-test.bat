@echo off
echo ==========================================
echo 快速替换 - 将塔防测试场景复制到 cpp-empty-test
echo ==========================================
echo.

set SOURCE_DIR=%~dp0Classes
set DEST_DIR=%~dp0..\tests\cpp-empty-test\Classes

echo 源目录: %SOURCE_DIR%
echo 目标目录: %DEST_DIR%
echo.

if not exist "%DEST_DIR%" (
    echo 错误: 目标目录不存在！
    pause
    exit /b 1
)

echo 正在复制文件...
copy /Y "%SOURCE_DIR%\Enemy.h" "%DEST_DIR%\"
copy /Y "%SOURCE_DIR%\Enemy.cpp" "%DEST_DIR%\"
copy /Y "%SOURCE_DIR%\Tower.h" "%DEST_DIR%\"
copy /Y "%SOURCE_DIR%\Tower.cpp" "%DEST_DIR%\"
copy /Y "%SOURCE_DIR%\TestScene.h" "%DEST_DIR%\"
copy /Y "%SOURCE_DIR%\TestScene.cpp" "%DEST_DIR%\"
copy /Y "%SOURCE_DIR%\AppDelegate.cpp" "%DEST_DIR%\"
copy /Y "%SOURCE_DIR%\AppDelegate.h" "%DEST_DIR%\"
echo.

echo ==========================================
echo 替换完成！
echo ==========================================
echo.
echo 现在请按以下步骤操作：
echo 1. 打开 e:\ProjCore\游戏\cocos2d-x-4.0\tests\cpp-empty-test\
echo 2. 用你通常运行 cpp-empty-test 的方法运行项目
echo 3. 你会看到塔防测试场景！
echo.
echo 测试场景特性：
echo - 预设路径和3个塔
echo - 自动生成敌人
echo - 点击添加更多塔
echo.
pause
