@echo off

rem ����debug������Ϣ

set s1=%~dp0%
set dir_path=%s1%bin
set run_path=%s1%bin\7za.exe

set target_file=""
set target_path=%dir_path%

set output_name=%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%%time:~6,2%
set output_dirname=%date:~0,4%%date:~5,2%%date:~8,2%

set output_dir=%dir_path%\Backup
set output_file=%output_dir%\%output_name%

rem �ж�7za.exeѹ�������Ƿ����;
if exist %run_path% goto run1
	mshta vbscript:msgbox("�޷��ҵ���ִ�г���, ����!",64,"����!")(window.close)
	exit
:run1

rem �жϱ���Ŀ¼Backup�Ƿ����;
if exist %output_dir% goto run2
	md %output_dir%
:run2

rem �жϱ���Ŀ¼�µ�����Ŀ¼�Ƿ����;
if exist %output_dir%\%output_dirname% goto run3
	md %output_dir%\%output_dirname%
:run3

rem �жϱ���Ŀ¼�µ���ʱĿ¼�Ƿ����;
if exist %output_dir%\%output_dirname%\%output_name% goto run4
	md %output_dir%\%output_dirname%\%output_name%
:run4

rem ö��ָ��Ŀ¼
for /f "delims=" %%a in ('dir /b/a-d/s %dir_path%\*') do (
	ECHO %%~a
)

