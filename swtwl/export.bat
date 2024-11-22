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

:: 遍历当前目录下的所有 .h 文件并复制到目标目录
for %%f in (*.h) do (
    echo 正在复制 %%f 到 %target_dir%
    copy /y "%%f" "%target_dir%"
)

echo 完成!
