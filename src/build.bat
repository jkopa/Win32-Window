@echo off

IF NOT "%clset%"=="64" call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

set opts =-Zi
set libs =User32.lib

mkdir ..\build
pushd ..\build
cl -Zi D:\Dev\Win32-Border\src\win32_custom_window.c %libs%
popd