@echo off

rem 测试debug调试信息

set s1=%~dp0%
set dir_path=%s1%bin
set run_path=%s1%bin\7za.exe

set target_file=""
set target_path=%dir_path%

set output_name=%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%%time:~6,2%
set output_dirname=%date:~0,4%%date:~5,2%%date:~8,2%

set output_dir=%dir_path%\Backup
set output_file=%output_dir%\%output_name%

rem 判断7za.exe压缩程序是否存在;
if exist %run_path% goto run1
	mshta vbscript:msgbox("无法找到可执行程序, 请检查!",64,"警告!")(window.close)
	exit
:run1

rem 判断备份目录Backup是否存在;
if exist %output_dir% goto run2
	md %output_dir%
:run2

rem 判断备份目录下的日期目录是否存在;
if exist %output_dir%\%output_dirname% goto run3
	md %output_dir%\%output_dirname%
:run3

rem 判断备份目录下的临时目录是否存在;
if exist %output_dir%\%output_dirname%\%output_name% goto run4
	md %output_dir%\%output_dirname%\%output_name%
:run4

rem 枚举指定目录
for /f "delims=" %%a in ('dir /b/a-d/s %dir_path%\*') do (
	ECHO %%~a
)

