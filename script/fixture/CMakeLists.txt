cmake_minimum_required(VERSION 2.8)

project(snow_fixture)

set(TEMP_DIRECTORY "${CMAKE_SOURCE_DIR}/temp")
set(EXECUTABLE_OUTPUT_PATH "${TEMP_DIRECTORY}")
set(LIBRARY_OUTPUT_PATH "${TEMP_DIRECTORY}")

include("${CMAKE_SOURCE_DIR}/../ensure_output_path.cmake")

include("${CMAKE_SOURCE_DIR}/../download_external_project.cmake")
download_external_project("gtest" "https://github.com/google/googletest.git" "${CMAKE_SOURCE_DIR}/../../third_party/google/gtest")
download_external_project("poco" "https://github.com/pocoproject/poco.git" "${CMAKE_SOURCE_DIR}/../../third_party/poco")
