@echo off
rem build.bat
rem
rem Build the Chk example with Microsoft cl.exe. Run from a Developer Command
rem Prompt for Visual Studio. MinGW/MSYS2/Git Bash users can use build.sh.

if not exist build mkdir build
cl /nologo /W4 /O2 /D_CRT_SECURE_NO_WARNINGS /I..\Lib main.c ..\Lib\demoAbcInOut.c /Fe:build\demoAbcDevice.exe
