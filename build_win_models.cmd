echo off

:: set vs generator
:: Visual Studio 14 2015 [arch]
:: Visual Studio 12 2013 [arch]
:: Visual Studio 11 2012 [arch]
set vc_number=12

:: set model
:: [DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL]

call download_win.cmd
if errorlevel 1 goto fail

call build_win.cmd debug x64
if errorlevel 1 goto fail

call build_win.cmd debug x86
if errorlevel 1 goto fail

call build_win.cmd release x64
if errorlevel 1 goto fail

call build_win.cmd release x86
if errorlevel 1 goto fail

echo *********************************************
echo BUILD MODELS SUCCEEDED
echo *********************************************
goto end:

:fail
echo *********************************************
echo BUILD MODELS FAILED
echo *********************************************

:end
