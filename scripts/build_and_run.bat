@echo off

:: Remove the build directory if it exists
if exist build (
    echo Removing existing build directory...
    rmdir /s /q build
)

:: Create a new build directory
echo Creating build directory...
mkdir build
cd build

echo Configuring the project with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

if errorlevel 1 (
    echo CMake configuration failed.
    cd ..
    exit /b 1
)

:: Build the project
echo Building the project...
cmake --build .
if errorlevel 1 (
    echo Build failed.
    cd ..
    exit /b 1
)

cd ..

echo Build and configuration completed successfully.

:: Run the executable (make sure to change this to your executable name)
echo Running the executable...
"build\Debug\main.exe"
if errorlevel 1 (
    echo Execution failed.
    exit /b 1
)

echo Execution completed successfully.
