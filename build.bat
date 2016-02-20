@echo off 
if not defined DevEnvDir (
    call "c:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/vcvarsall.bat"
)

cl %1.cpp gdi32.lib user32.lib Kernel32.lib


IF ERRORLEVEL 1 goto DIE

IF "%2"=="-r" ( start %1.exe )

:DIE

popd