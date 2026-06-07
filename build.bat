@echo off
echo ==========================================
echo Tower Defense - Build Script
echo ==========================================
echo.

if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

echo.
echo Running CMake...
cmake .. -G "Visual Studio 17 2022" -A Win32

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ==========================================
    echo CMake configuration completed successfully!
    echo ==========================================
    echo.
    echo Now you can open TowerDefense.sln in Visual Studio
    echo or run: cmake --build . --config Release
    echo.
) else (
    echo.
    echo ==========================================
    echo Error: CMake configuration failed!
    echo ==========================================
    echo.
    echo Trying with Visual Studio 16 2019...
    cmake .. -G "Visual Studio 16 2019" -A Win32
)

pause
