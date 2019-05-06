@echo off

set s1=%~dp0%
if exist "*.ncb"				(del /a /q /f "*.ncb")
if exist "*.suo"				(del /a /q /f "*.suo")
if exist "*.db"					(del /a /q /f "*.db")
if exist "ipch" 				(rmdir /Q /S "ipch")

cd %s1%\test
if exist "*.aps"				(del /a /q /f "*.aps")
if exist "*.user"				(del /a /q /f "*.user")		

cd %s1%\bin
if exist "test.exe"				(del /a /q /f "test.exe")
if exist "*.dll"				(del /a /q /f "*.dll")

cd %s1%\temp\Compile
if exist "test" 				(rmdir /Q /S "test")

cd %s1%\temp\Link
if exist "test" 				(rmdir /Q /S "test")

cd %s1%\.vs
if exist "Test Project14"		(rmdir /Q /S "Test Project14")