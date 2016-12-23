echo off
:: [DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL]

:: call download_win.cmd
:: if errorlevel 1 goto fail

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
