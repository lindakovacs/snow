cmake_minimum_required(VERSION 2.8)

project(snow_core)

file(GLOB SOURCE_FILES *.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
