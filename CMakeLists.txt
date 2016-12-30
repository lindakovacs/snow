cmake_minimum_required(VERSION 2.8)

project(snow)

include_directories(.
  ./research
  ./third_party/google/gtest/googlemock/include
  ./third_party/google/gtest/googletest/include
)

include("script/ensure_output_path.cmake")
include("script/ensure_staic_linkage.cmake")
include("script/generate_version_file.cmake")
include("script/set_compilation_flags.cmake")

link_directories(${LIBRARY_OUTPUT_PATH})
message("link_directories = ${LIBRARY_OUTPUT_PATH}")

### be sure to use a function to generate a certain version (file, major, minor, patch, build)
generate_version_file("${CMAKE_SOURCE_DIR}/build/version.h" "1" "1" "2" "$ENV{BUILD_NUMBER}")

add_subdirectory(core)
add_subdirectory(research/core_test)
## add_subdirectory(research/gtest)
## add_subdirectory(third_party/google/gtest)
## add_subdirectory(third_party/poco)
