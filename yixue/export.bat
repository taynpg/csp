@echo off
:: ��ȡ���������ڵĵ�ǰĿ¼
set "current_dir=%~dp0"

:: ����Ŀ��Ŀ¼���ϼ�Ŀ¼�� include �ļ��У�
set "target_dir=%~dp0..\include"

:: ���Ŀ��Ŀ¼�����ڣ��򴴽�
if not exist "%target_dir%" (
    mkdir "%target_dir%"
    echo ����Ŀ��Ŀ¼: %target_dir%
)
copy /y "%current_dir%box_yxbase.h" "%target_dir%"
echo ���!
