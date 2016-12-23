cmake_minimum_required(VERSION 2.8)

project(snow)

include_directories(.)
include("script/ensure_output_path.cmake")

link_directories(${LIBRARY_OUTPUT_PATH})

message("link_directories = ${LIBRARY_OUTPUT_PATH}")
message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message("CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")

if(WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

message("CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
message("CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

set(FLAG_SUFF ${CMAKE_BUILD_TYPE})
string(TOUPPER ${FLAG_SUFF} FLAG_SUFF)
message("CMAKE_C_FLAGS_${FLAG_SUFF} = ${CMAKE_C_FLAGS_${FLAG_SUFF}}")
message("CMAKE_CXX_FLAGS_${FLAG_SUFF} = ${CMAKE_CXX_FLAGS_${FLAG_SUFF}}")

add_subdirectory(core)
## add_subdirectory(third_party/google/gtest)
## add_subdirectory(third_party/poco)
