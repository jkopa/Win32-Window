@echo off

IF NOT EXIST build mkdir build
IF NOT "%clset%"=="64" call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

set opt=-Zi
set libs=user32.lib gdi32.lib
set src=D:\Dev\Win32-Border\src\

pushd build
cl %opt% ..\src\win32_custom_window.c %libs% -Fecustom_window
popd