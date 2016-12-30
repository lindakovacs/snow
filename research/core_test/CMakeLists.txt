cmake_minimum_required(VERSION 2.8)

project(research_core_test)

file(GLOB SOURCE_FILES *.cpp)

if(WIN32)
  add_executable(${PROJECT_NAME} ${SOURCE_FILES} description.rc)
else()
  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif()

target_link_libraries(
  ${PROJECT_NAME}
  snow_core
)
