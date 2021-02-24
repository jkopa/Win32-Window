IF NOT "%clset%"=="64" call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

pushd build
devenv custom_window.exe
popd
