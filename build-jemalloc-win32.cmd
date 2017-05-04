@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
if errorlevel 1 goto ex1
md build
cd build

:checkout
git clone https://github.com/jemalloc/jemalloc.git

:build
cd jemalloc
if errorlevel 1 goto ex1
sh ./autogen.sh CC=cl --enable-lazy-lock=no
if errorlevel 1 goto ex1
msbuild msvc\jemalloc_vc2015.sln /p:Configuration=Debug-static /p:Platform=x86
if errorlevel 1 goto ex1
msbuild msvc\jemalloc_vc2015.sln /p:Configuration=Release-static /p:Platform=x86
if errorlevel 1 goto ex1
cd ..

:install
@xcopy /y jemalloc\msvc\Win32\Debug-static\jemalloc-*.* ..\jemalloc\lib\
if errorlevel 1 goto ex1
@xcopy /y jemalloc\msvc\Win32\Release-static\jemalloc-*.* ..\jemalloc\lib\
if errorlevel 1 goto ex1
xcopy /y jemalloc\include\jemalloc\*.h ..\jemalloc\include\jemalloc\
if errorlevel 1 goto ex1
xcopy /s /y jemalloc\include\msvc_compat ..\jemalloc\include\msvc_compat\
if errorlevel 1 goto ex1

echo SUCCESS!
goto eof
:ex1
echo FAILED!
:eof
pause
