cmake_minimum_required(VERSION 2.8)

project(snow_fixture)

set(TEMP_DIRECTORY "${CMAKE_SOURCE_DIR}/temp")
set(EXECUTABLE_OUTPUT_PATH "${TEMP_DIRECTORY}")
set(LIBRARY_OUTPUT_PATH "${TEMP_DIRECTORY}")

include("${CMAKE_SOURCE_DIR}/../ensure_output_path.cmake")

include("${CMAKE_SOURCE_DIR}/../download_external_project.cmake")
download_external_project("gtest" "https://github.com/google/googletest.git" "${CMAKE_SOURCE_DIR}/../../third_party/google/gtest")
download_external_project("poco" "https://github.com/pocoproject/poco.git" "${CMAKE_SOURCE_DIR}/../../third_party/poco")

## configure poco (set static without suffix)
set(POCO_PATH "${CMAKE_SOURCE_DIR}/../../third_party/poco")
set(file_content)
file(READ "${POCO_PATH}/CMakeLists.txt" file_content)
set(file_content_res)

string(REGEX REPLACE "option\\(POCO_STATIC.*\\\"Set to OFF\\|ON \\(default is OFF\\) to control build of POCO as STATIC library\\\" OFF\\)"
  "option(POCO_STATIC \"Set to OFF|ON (default is OFF) to control build of POCO as STATIC library\" ON)" file_content_res "${file_content}")

string(REGEX REPLACE "option\\(POCO_MT.*\\\"Set to OFF\\|ON \\(default is OFF\\) to control build of POCO as /MT instead of /MD\\\" OFF\\)"
  "option(POCO_STATIC \"Set to OFF|ON (default is OFF) to control build of POCO as as /MT instead of /MD\" ON)" file_content_res "${file_content_res}")

string(REGEX REPLACE "set\\(CMAKE_LIBRARY_OUTPUT_DIRECTORY \\$\\{CMAKE_BINARY_DIR\\}/lib\\)" " " file_content_res "${file_content_res}")
string(REGEX REPLACE "set\\(CMAKE_ARCHIVE_OUTPUT_DIRECTORY \\$\\{CMAKE_BINARY_DIR\\}/lib\\)" " " file_content_res "${file_content_res}")
string(REGEX REPLACE "set\\(CMAKE_RUNTIME_OUTPUT_DIRECTORY \\$\\{CMAKE_BINARY_DIR\\}/bin\\)" " " file_content_res "${file_content_res}")
file(WRITE "${POCO_PATH}/CMakeLists.txt" "${file_content_res}")

file(READ "${POCO_PATH}/cmake/DefinePlatformSpecifc.cmake" file_content)
string(REGEX REPLACE "set\\(CMAKE_DEBUG_POSTFIX \\\"\\$\\{STATIC_POSTFIX\\}d" "set(CMAKE_DEBUG_POSTFIX \"\${STATIC_POSTFIX}" file_content_res "${file_content}")
string(REGEX REPLACE "set\\(STATIC_POSTFIX \\\"mt\\\" CACHE STRING \\\"Set static library postfix\\\" FORCE\\)" "set(STATIC_POSTFIX \"\" CACHE STRING \"Set static library postfix\" FORCE)" file_content_res "${file_content_res}")
string(REGEX REPLACE "set\\(STATIC_POSTFIX \\\"md\\\" CACHE STRING \\\"Set static library postfix\\\" FORCE\\)" "set(STATIC_POSTFIX \"\" CACHE STRING \"Set static library postfix\" FORCE)" file_content_res "${file_content_res}")
file(WRITE "${POCO_PATH}/cmake/DefinePlatformSpecifc.cmake" "${file_content_res}")
