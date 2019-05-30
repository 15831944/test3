@echo on

set execfile_name=FeiQ.exe
set execfile_pid=test

set abs_path=D:\\Program Files\\·ÉÇï
set file_path=%abs_path%\\%execfile_name%

wmic process where "caption like '%execfile_name%'" get caption,commandline,ParentProcessId /value | find "ParentProcessId" >%execfile_pid%

pause