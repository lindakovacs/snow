echo off
set path_current=%~dp0
set path_current=%path_current:~0,-1%
set path_posix=%path_current:\=/%/script/download
set path_project=%path_current%\script\download\temp

rmdir %path_project% /s /q
mkdir %path_project%

echo project root: %path_posix%
echo project path: %path_project%

set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio %vc_number%.0\VC\bin
call vcvars32.bat

set path_project_bin=%path_posix%/temp
set path_project_lib=%path_posix%/temp

set generator="NMake Makefiles"

pushd %path_project%
cmake -G %generator% -DEXECUTABLE_OUTPUT_PATH=%path_project_bin% -DLIBRARY_OUTPUT_PATH=%path_project_lib% %path_posix%
cmake --build .
popd    

rmdir %path_project% /s /q
