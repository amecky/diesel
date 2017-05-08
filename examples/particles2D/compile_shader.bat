@echo off
rem THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
rem ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
rem THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
rem PARTICULAR PURPOSE.
rem
rem Copyright (c) Microsoft Corporation. All rights reserved.

setlocal
set error=0

rem set FX_PATH=C:\devtools\DirectX_SDK\Utilities\bin\x86
set FX_PATH=C:\devtools\DirectX\Utilities\bin\x86

rem Particles
call :CompileShader%1 Particles Particles vs VS_Main
call :CompileShader%1 Particles Particles ps PS_Main

echo.

if %error% == 0 (
    echo Shaders compiled ok
) else (
    echo There were shader compilation errors!
)

endlocal
exit /b

:CompileShader
set fxc=%FX_PATH%\fxc /nologo %1.hlsl /T%3_4_0 /Zi /Zpc /Qstrip_reflect /Qstrip_debug /E%4 /Fh%1_%4.h /Vn%2_%4
echo.
echo %fxc%
%fxc% || set error=1
exit /b
