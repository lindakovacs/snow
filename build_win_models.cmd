echo off

:: set vs generator (sure that VS installed on C:)
:: Visual Studio 14 2015 [arch]
:: Visual Studio 12 2013 [arch]
:: Visual Studio 11 2012 [arch]
set vc_number=12

:: set model
:: [DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL]

call launch_win.cmd download
if errorlevel 1 goto fail

call launch_win.cmd fixture
if errorlevel 1 goto fail

call build_win.cmd debug x64
if errorlevel 1 goto fail

:: call build_win.cmd debug x86
:: if errorlevel 1 goto fail

:: call build_win.cmd release x64
:: if errorlevel 1 goto fail

:: call build_win.cmd release x86
:: if errorlevel 1 goto fail

echo *********************************************
echo BUILD MODELS SUCCEEDED
echo *********************************************
goto end:

:fail
echo *********************************************
echo BUILD MODELS FAILED
echo *********************************************

:end
