@echo off

set s1=%~dp0%
if exist "*.ncb"				(del /a /q /f "*.ncb")
if exist "*.suo"				(del /a /q /f "*.suo")

cd %s1%\test
if exist "*.aps"				(del /a /q /f "*.aps")
rem if exist "*.user"			(del /a /q /f "*.user")

cd %s1%\bin
if exist "test.exe"				(del /a /q /f "test.exe")
if exist "opencv*.dll"			(del /a /q /f "opencv*.dll")
if exist "lib*.dll"				(del /a /q /f "lib*.dll")
if exist "tbb*.dll"				(del /a /q /f "tbb*.dll")	
if exist "OpenCVRzt.dll"		(del /a /q /f "OpenCVRzt.dll")

cd %s1%\temp\Compile
if exist "test" 				(rmdir /Q /S "test")
if exist "OpenCVRzt" 			(rmdir /Q /S "OpenCVRzt")
if exist "UKeyVerifyLib" 		(rmdir /Q /S "UKeyVerifyLib")

cd %s1%\temp\Link
if exist "test" 				(rmdir /Q /S "test")
if exist "OpenCVRzt" 			(rmdir /Q /S "OpenCVRzt")
if exist "UKeyVerifyLib" 		(rmdir /Q /S "UKeyVerifyLib")