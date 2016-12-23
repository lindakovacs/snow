cmake_minimum_required(VERSION 2.8)

project(snow)

include_directories(.)
include("script/ensure_output_path.cmake")

link_directories(${LIBRARY_OUTPUT_PATH})
message("link_directories = ${LIBRARY_OUTPUT_PATH}")
message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

if(WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(research/core)
## add_subdirectory(third_party/google/gtest)
## add_subdirectory(third_party/poco)
