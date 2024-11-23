@echo off
setlocal
cd /d %~dp0
set CC=cl
set LK=link
set CFLAGS=/EHsc /std:c++14 /W4 /MD /O2 /DNDEBUG /source-charset:utf-8 /wd4101 /wd4244 /wd4267 /wd4456 /wd4189 /wd4100 /wd4701 /wd4018
set INCLUDES=/I "src" /I "calendar" /I "qimen" /I "swtwl" /I "yixue" /I "zhcn"
set LDFLAGS=
set TARGET=build\release\csp.exe
set OBJ_DIR=build\release
set SRC_DIRS=src calendar qimen swtwl yixue zhcn
if exist %OBJ_DIR% rmdir /s /q %OBJ_DIR%
mkdir %OBJ_DIR%
for %%d in (%SRC_DIRS%) do (
    for %%f in (%%d\*.cpp) do (
        echo compile %%f ...
        %CC% %CFLAGS% %INCLUDES% /c %%f /Fo%OBJ_DIR%\%%~nxf.obj
    )
)
echo linking exe ...
%LK% %LDFLAGS% /OUT:%TARGET% %OBJ_DIR%\*.obj
echo compile done: %TARGET%
del /q %OBJ_DIR%\*.obj
endlocal