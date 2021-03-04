@echo off

::Step 1: checks if build directories are there and if not makes them
::
::Step 2: set options and flags as variables
::
::Step 3: pushd into build folder and compile the program, then move the exe into the bin folder


::Step 1
IF NOT EXIST build mkdir build

::Step 2
set optdgb=-g -O0
set optdgb0=-g -O0 -ftime-report
set optrls=
set libs=-luser32.lib -lgdi32.lib -lkernel32.lib

::Step 3
pushd build
clang %optdgb% ..\src\win32_custom_window.cpp -o custom_window.exe %libs%
::MOVE /Y custom_window.exe ..\bin
popd