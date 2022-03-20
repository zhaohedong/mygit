# remove last end of "/"
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})

# get current relative dir name and set target name
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})

aux_source_directory(./ CURRENT_FOLDER_SRC)

file(GLOB SRC_FILES ${CURRENT_FOLDER_ABSOLUTE}/*.c)
add_executable(${CURRENT_FOLDER}  ${SRC_FILES})

include_directories("${ROOT_PATH}/basic/message_queue")
include_directories("${ROOT_PATH}/basic/thread")
target_link_directories(${CURRENT_FOLDER} PUBLIC ${ROOT_PATH}/basic)
target_link_libraries(${CURRENT_FOLDER} PUBLIC basic)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

message(STATUS "SRC_FILES ${SRC_FILES}") 