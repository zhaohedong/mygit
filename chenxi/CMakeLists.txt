# add the executable
cmake_minimum_required (VERSION 3.0)
project(chenxi)

set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(${ROOT_PATH}/main)
add_subdirectory(${ROOT_PATH}/basic)
add_subdirectory(${ROOT_PATH}/application)

aux_source_directory(${ROOT_PATH}/main MAIN_SRC)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(chenxi ${MAIN_SRC})

include_directories("${ROOT_PATH}/basic/message_queue")
include_directories("${ROOT_PATH}/basic/statemachine")
target_link_directories(chenxi PUBLIC ./basic)
target_link_libraries(chenxi PUBLIC basic)




message(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Root path : ${ROOT_PATH}")

