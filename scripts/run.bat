@echo off

echo Build and configuration completed successfully.

:: Run the executable (make sure to change this to your executable name)
echo Running the executable...
"build\Debug\main.exe"
if errorlevel 1 (
    echo Execution failed.
    exit /b 1
)

echo Execution completed successfully.
