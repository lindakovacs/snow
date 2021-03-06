### The following function generates the file with build version numbers.

function(generate_version_file file_name version_major version_minor version_patch build_num)
  if("${build_num}" STREQUAL "")
    set(build_num "1140")
  endif()
  if(NOT EXISTS "${file_name}")
    file(WRITE ${file_name} "#pragma once\n\n#include <core/macro.h>\n\n#define BUILD_VERSION_MAJOR ${version_major}\n#define BUILD_VERSION_MINOR ${version_minor}\n#define BUILD_VERSION_PATCH ${version_patch}\n#define BUILD_NUMBER ${build_num}\n#define BUILD_VERSION_STR CORE_CONVERT_TO_STR(BUILD_VERSION_MAJOR) \".\" CORE_CONVERT_TO_STR(BUILD_VERSION_MINOR) \".\" CORE_CONVERT_TO_STR(BUILD_VERSION_PATCH) \".\" CORE_CONVERT_TO_STR(BUILD_NUMBER)
      \n#ifdef __GNUC__\n#pragma GCC push_options\n#pragma GCC optimize(\"O0\")\n#define BUILD_VERSION_STR_WITH_PREFIX \"build_version=\" BUILD_VERSION_STR\nstatic const volatile char* BuildVersion = BUILD_VERSION_STR_WITH_PREFIX;\n#pragma GCC pop_options\n#endif\n")
  endif()
endfunction(generate_version_file)
