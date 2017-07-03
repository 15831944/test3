@echo off

set s1=%~dp0%
if exist "*.ncb"		(del /a /q /f "*.ncb")
if exist "*.suo"		(del /a /q /f "*.suo")

cd %s1%test
if exist "*.aps"		(del /a /q /f "*.aps")
if exist "*.user"		(del /a /q /f "*.user")

cd %s1%bin
if exist "test.exe"		(del /a /q /f "test.exe")
if exist "opencv*.dll"	(del /a /q /f "opencv*.dll")
if exist "lib*.dll"		(del /a /q /f "lib*.dll")
if exist "tbb*.dll"		(del /a /q /f "tbb*.dll")	

cd %S2%\temp\Compile
if exist "test" 		(rmdir /Q /S test)

cd %S2%\temp\Link
if exist "test" 		(rmdir /Q /S test)