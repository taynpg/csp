@echo off
:: 获取批处理所在的当前目录
set "current_dir=%~dp0"

:: 定义目标目录（上级目录的 include 文件夹）
set "target_dir=%~dp0..\include"

:: 如果目标目录不存在，则创建
if not exist "%target_dir%" (
    mkdir "%target_dir%"
    echo 创建目标目录: %target_dir%
)
copy /y "%current_dir%box_yxbase.h" "%target_dir%"
echo 完成!
