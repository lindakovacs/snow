#!/bin/bash

./download_lin.sh
if [[ $? -ne 0 ]]; then
  exit 1
fi

./build_lin.sh debug x64
if [[ $? -ne 0 ]]; then
  exit 1
fi

./build_lin.sh debug x86
if [[ $? -ne 0 ]]; then
  exit 1
fi

./build_lin.sh release x64
if [[ $? -ne 0 ]]; then
  exit 1
fi

./build_lin.sh release x86
if [[ $? -ne 0 ]]; then
  exit 1
fi

echo "*********************************************"
echo "BUILD MODELS SUCCEEDED"
echo "*********************************************"
