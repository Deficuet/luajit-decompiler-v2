if not exist "./build" mkdir "./build"
del /s /f /q .\build\*

if "%2" == "32" (
    set TARGET=x86
) else (
    set TARGET=amd64
)

set VS_DEV_CMD_ROOT=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools

if "%1"=="dynamic" (
"%VS_DEV_CMD_ROOT%\VsDevCmd.bat" -arch=%TARGET% && ^
cl /std:c++20 /J /O2 /LD /EHsc /Fo.\build\ /Fe:.\build\ljd%2.dll .\*.cpp .\ast\*.cpp .\bytecode\*.cpp .\lua\*.cpp
) else if "%1"=="static" (
"%VS_DEV_CMD_ROOT%\VsDevCmd.bat" -arch=%TARGET% && ^
cl /std:c++20 /J /O2 /EHsc /c /Fo.\build\ .\*.cpp .\ast\*.cpp .\bytecode\*.cpp .\lua\*.cpp && ^
ar rcs .\build\ljd%2.a .\build\*.obj
)
