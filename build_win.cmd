echo off

set path_current=%~dp0
set path_current=%path_current:~0,-1%
set path_posix=%path_current:\=/%

set config=%1
set bitness=%2
set path_project=%path_current%\build\%bitness%\%config%

rmdir %path_project% /s /q
mkdir %path_project%

if %bitness%==x86 set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio %vc_number%.0\VC\bin
if %bitness%==x86 call vcvars32.bat

if %bitness%==x64 set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio %vc_number%.0\VC\bin\amd64
if %bitness%==x64 call vcvars64.bat

set path_project_build=%path_posix%/build/%bitness%/%config%
set path_project_bin=%path_posix%/bin/%bitness%/%config%
set path_project_lib=%path_posix%/lib/%bitness%/%config%

echo project root: %path_posix%
echo project build: %path_project_build%
echo project bin: %path_project_bin%
echo project lib: %path_project_lib%

:: NOTE: use vs-generator for local builds
if %bitness%==x64 set generator="Visual Studio %vc_number% Win64"
if %bitness%==x86 set generator="Visual Studio %vc_number%"
:: set generator="NMake Makefiles"

pushd %path_project%
call cmake -G %generator% -DEXECUTABLE_OUTPUT_PATH=%path_project_bin% -DLIBRARY_OUTPUT_PATH=%path_project_lib% -DCMAKE_BUILD_TYPE=%config% %path_posix%
if errorlevel 1 goto fail
call cmake --build . --config %config%
if errorlevel 1 goto fail
popd

echo *********************************************
echo BUILD SUCCEEDED (%config% %bitness%)
echo *********************************************
goto end:

:fail
echo *********************************************
echo BUILD FAILED (%config% %bitness%)
echo *********************************************

:end
