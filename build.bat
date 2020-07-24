@echo off

set opts =-FR
set code =%cd ../win32_custom_window.cpp%
set cpp_file =win32_custom_window.cpp
set libs =User32.lib

IF NOT "%clset%"=="64" call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
pushd
mkdir build
cd build
cl %opts% %code% %libs%
popd