#!/bin/bash
path_posix="$(pwd)"/script/fixture
path_project=$path_posix/temp

echo "project root: "$path_posix
echo "project path: "$path_project

rm -rf "$path_project"
mkdir -p "$path_project"
cd "$path_project"

cmake -G "Unix Makefiles" -DEXECUTABLE_OUTPUT_PATH="$path_project" -DLIBRARY_OUTPUT_PATH="$path_project" "$path_posix"
if [[ $? -ne 0 ]]; then
  exit 1
fi

cmake --build .
if [[ $? -ne 0 ]]; then
  exit 1
fi

cd $path_posix
rm -rf "$path_project"
