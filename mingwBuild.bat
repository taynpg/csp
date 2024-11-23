@echo off
setlocal
cd /d %~dp0
set CC=g++
set CFLAGS=-std=c++14 -O2 -DNDEBUG -finput-charset=utf-8 -fexec-charset=gbk
set INCLUDES=-I src -I calendar -I qimen -I swtwl -I yixue -I zhcn
set LDFLAGS=
set TARGET=build\release\csp.exe
set OBJ_DIR=build\release
set SRC_DIRS=src calendar qimen swtwl yixue zhcn
if exist %OBJ_DIR% rmdir /s /q %OBJ_DIR%
mkdir %OBJ_DIR%
for %%d in (%SRC_DIRS%) do (
    for %%f in (%%d\*.cpp) do (
        echo Compiling %%f ...
        %CC% %CFLAGS% %INCLUDES% -c %%f -o %OBJ_DIR%\%%~nxf.o
    )
)
echo Linking executable ...
%CC% %LDFLAGS% -o %TARGET% %OBJ_DIR%\*.o
echo Compile done: %TARGET%

echo Cleaning up object files ...
del /q %OBJ_DIR%\*.o
echo Build and cleanup completed.
endlocal
