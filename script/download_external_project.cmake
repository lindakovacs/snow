include(ExternalProject)

function(download_external_project name repository target)
  ExternalProject_Add(${name}
    GIT_REPOSITORY    ${repository}
    GIT_TAG           master
    SOURCE_DIR        "${target}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
    TIMEOUT           90
  )
endfunction(download_external_project)
