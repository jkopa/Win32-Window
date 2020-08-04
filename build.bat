@echo off

::Step 1: checks if build & bin directories are there and if not makes them
::
::Step 2: set vcvars64.bat so that cmd can access the compiler 
::
::Step 3: set options and flags as variables
::
::Step 4: pushd into build folder and compile the program, then move the exe into the bin folder


::Step 1
IF NOT EXIST build mkdir build
IF NOT EXIST bin mkdir bin

::Step 2
IF NOT "%clset%"=="64" call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

::Step 3
set addlib=/I %VULKAN_SDK%\Include
set opt=-Zi
set libs=user32.lib gdi32.lib kernel32.lib
set src=D:\Dev\Win32-Border\src\

::Step 4
pushd build
cl /I %VULKAN_SDK%\Include %opt% ..\src\win32_custom_window.c %libs% -Fecustom_window
MOVE /Y custom_window.exe ..\bin
popd