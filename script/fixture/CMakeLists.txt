cmake_minimum_required(VERSION 2.8)

project(snow_fixture)

set(TEMP_DIRECTORY "${CMAKE_SOURCE_DIR}/temp")
set(EXECUTABLE_OUTPUT_PATH "${TEMP_DIRECTORY}")
set(LIBRARY_OUTPUT_PATH "${TEMP_DIRECTORY}")

include("${CMAKE_SOURCE_DIR}/../ensure_output_path.cmake")

## configure poco (set static without suffix)
set(POCO_PATH "${CMAKE_SOURCE_DIR}/../../third_party/poco")
set(FILE_CONTENT)
file(READ "${POCO_PATH}/CMakeLists.txt" FILE_CONTENT)
set(FILE_CONTENT_TMP)

string(REGEX REPLACE "option\\(POCO_STATIC.*\\\"Set to OFF\\|ON \\(default is OFF\\) to control build of POCO as STATIC library\\\" OFF\\)"
  "option(POCO_STATIC \"Set to OFF|ON (default is OFF) to control build of POCO as STATIC library\" ON)" FILE_CONTENT_TMP "${FILE_CONTENT}")

string(REGEX REPLACE "option\\(POCO_MT.*\\\"Set to OFF\\|ON \\(default is OFF\\) to control build of POCO as /MT instead of /MD\\\" OFF\\)"
  "option(POCO_STATIC \"Set to OFF|ON (default is OFF) to control build of POCO as as /MT instead of /MD\" ON)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")

string(REGEX REPLACE "set\\(CMAKE_LIBRARY_OUTPUT_DIRECTORY \\$\\{CMAKE_BINARY_DIR\\}/lib\\)" " " FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
string(REGEX REPLACE "set\\(CMAKE_ARCHIVE_OUTPUT_DIRECTORY \\$\\{CMAKE_BINARY_DIR\\}/lib\\)" " " FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
string(REGEX REPLACE "set\\(CMAKE_RUNTIME_OUTPUT_DIRECTORY \\$\\{CMAKE_BINARY_DIR\\}/bin\\)" " " FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")

string(REGEX REPLACE "option\\(ENABLE_DATA_MYSQL \\\"Enable Data MySQL\\\" ON\\)" "option(ENABLE_DATA_MYSQL \"Enable Data MySQL\" OFF)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
string(REGEX REPLACE "option\\(ENABLE_DATA_ODBC \\\"Enable Data ODBC\\\" ON\\)" "option(ENABLE_DATA_ODBC \"Enable Data ODBC\" OFF)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
string(REGEX REPLACE "option\\(ENABLE_MONGODB \\\"Enable MongoDB\\\" ON\\)" "option(ENABLE_MONGODB \"Enable MongoDB\" OFF)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
string(REGEX REPLACE "option\\(ENABLE_PAGECOMPILER \\\"Enable PageCompiler\\\" ON\\)" "option(ENABLE_PAGECOMPILER \"Enable PageCompiler\" OFF)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
string(REGEX REPLACE "option\\(ENABLE_PAGECOMPILER_FILE2PAGE \\\"Enable File2Page\\\" ON\\)" "option(ENABLE_PAGECOMPILER_FILE2PAGE \"Enable File2Page\" OFF)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")

file(WRITE "${POCO_PATH}/CMakeLists.txt" "${FILE_CONTENT_TMP}")

file(READ "${POCO_PATH}/cmake/DefinePlatformSpecifc.cmake" FILE_CONTENT)
string(REGEX REPLACE "set\\(CMAKE_DEBUG_POSTFIX \\\"\\$\\{STATIC_POSTFIX\\}d" "set(CMAKE_DEBUG_POSTFIX \"\${STATIC_POSTFIX}" FILE_CONTENT_TMP "${FILE_CONTENT}")
string(REGEX REPLACE "set\\(STATIC_POSTFIX \\\"mt\\\" CACHE STRING \\\"Set static library postfix\\\" FORCE\\)" "set(STATIC_POSTFIX \"\" CACHE STRING \"Set static library postfix\" FORCE)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
string(REGEX REPLACE "set\\(STATIC_POSTFIX \\\"md\\\" CACHE STRING \\\"Set static library postfix\\\" FORCE\\)" "set(STATIC_POSTFIX \"\" CACHE STRING \"Set static library postfix\" FORCE)" FILE_CONTENT_TMP "${FILE_CONTENT_TMP}")
file(WRITE "${POCO_PATH}/cmake/DefinePlatformSpecifc.cmake" "${FILE_CONTENT_TMP}")
