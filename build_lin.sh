#!/bin/bash
path_posix="$(pwd)"

config=$1
bitness=$2

path_project_build=$path_posix/build/$bitness/$config
path_project_bin=$path_posix/bin/$bitness/$config
path_project_lib=$path_posix/lib/$bitness/$config

echo "project root: "$path_posix
echo "project build: "$path_project_build
echo "project bin: "$path_project_bin
echo "project lib: "$path_project_lib

rm -rf "$path_project_build"
mkdir -p "$path_project_build"
cd "$path_project_build"

function show_build_failed
{
  echo "*********************************************"
  echo "BUILD FAILED ("$config $bitness")"
  echo "*********************************************"
}

cmake -G "Unix Makefiles" -DEXECUTABLE_OUTPUT_PATH="$path_project_bin" -DLIBRARY_OUTPUT_PATH="$path_project_lib" -DCMAKE_BUILD_TYPE="$config" -DBITNESS="$bitness" "$path_posix"
if [[ $? -ne 0 ]]; then
  show_build_failed
  exit 1
fi

cmake --build . --config $config
if [[ $? -ne 0 ]]; then
  show_build_failed
  exit 1
fi

cd $path_posix

echo "*********************************************"
echo "BUILD SUCCEEDED ("$config $bitness")"
echo "*********************************************"
