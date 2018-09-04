@echo off

:: Configure build variables.
set BUILD_CONFIGURATION=Debug
set BUILD_ARCH=x64
set BUILD_DIR=msvc15-%BUILD_ARCH%-%BUILD_CONFIGURATION%
set BUILD_GENERATOR="Visual Studio"

set VS_VERSION=15
set VS_YEAR=2017
set VS_ARCH=Win64

echo Creating a build directory...

pushd ..

if not exist build mkdir build
pushd build

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%

echo [CMake] Generating the project...
cmake ^
	-G "Visual Studio %VS_VERSION% %VS_YEAR% %VS_ARCH%" ^
	../..

echo Project has been generated successfully!
popd
popd
popd
goto :eof

:error
echo [ERROR] Failed to generate project.
popd
popd
popd
exit /b 1