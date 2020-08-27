@echo off
REM "$Date: 2019-01-24 16:35:14 -0200 (qui, 24 jan 2019) $"
REM "$Author: chico $"
REM "$Revision: 124 $"
set OBJECT_=LogViewer
rem set PATH=%QT_PATH%;%QT_CREATOR_PATH%;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files\CollabNet\Subversion Client;C:\MinGW\bin;C:\MinGW\MSYS\1.0\bin;
rem call "C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"
rem qmake && jom -j2 -l distclean
mingw32-make -j4 -s distclean
del /s /q release
rmdir /s /q release
del /s /q debug
rmdir /s /q debug
del /s /q GeneratedFiles
rmdir /s /q GeneratedFiles
del object_script.%OBJECT_%
Z: && cd Z:\ && rmdir /s /q %OBJECT_%
C: