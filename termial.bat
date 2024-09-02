@echo off
setlocal
set "CURRENT_DIR=%~dp0"
start wt new-tab cmd /k "call \"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat\" && cd /d %CURRENT_DIR% && chcp 65001 && nvim ."
endlocal
