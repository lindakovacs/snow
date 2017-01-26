cmake_minimum_required(VERSION 2.8)

project(snow_core)

include("../script/gather_source_files.cmake")

add_library(${PROJECT_NAME} ${SOURCE_FILES})
