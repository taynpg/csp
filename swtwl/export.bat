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

:: ������ǰĿ¼�µ����� .h �ļ������Ƶ�Ŀ��Ŀ¼
for %%f in (*.h) do (
    echo ���ڸ��� %%f �� %target_dir%
    copy /y "%%f" "%target_dir%"
)

echo ���!
