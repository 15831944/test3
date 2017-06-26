@echo off

del test.exe /f /q
del opencv*.dll /f /q
del lib*.dll /f /q
del tbb*.dll /f /q

set S1=%cd%

cd..
set S2=%cd%

cd %S2%\temp\Compile
if exist "test" (rmdir /Q /S test)

cd %S2%\temp\Link
if exist "test" (rmdir /Q /S test)