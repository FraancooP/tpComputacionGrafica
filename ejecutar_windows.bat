@echo off
setlocal

cd /d "%~dp0"

rem Buscar las herramientas de Visual Studio.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" goto sin_visual

set "VSROOT="

for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSROOT=%%i"

if not defined VSROOT goto sin_visual

rem Preparar el compilador de 64 bits.
call "%VSROOT%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64

if errorlevel 1 goto error

rem Agregar el CMake incluido con Visual Studio, si existe.
if exist "%VSROOT%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" set "PATH=%VSROOT%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;%PATH%"

where cmake >nul 2>nul
if errorlevel 1 goto sin_cmake

rem Configurar y compilar.
cmake -S . -B build-windows -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 goto error

cmake --build build-windows
if errorlevel 1 goto error

rem Ejecutar solo si la compilacion termino correctamente.
"build-windows\bin\cgyav.exe"
if errorlevel 1 goto error

exit /b 0

:sin_visual
echo No se encontraron las herramientas de C++ de Visual Studio.
exit /b 1

:sin_cmake
echo No se encontro CMake.
exit /b 1

:error
echo La operacion fallo. Revisa los mensajes anteriores.
exit /b 1