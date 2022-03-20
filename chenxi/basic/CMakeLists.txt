aux_source_directory(./statemachine STATEMACHINE_DIR)
aux_source_directory(./thread THREAD_DIR)

set (MESSAGE_QUEUE_SRC_FILES "")
add_subdirectory(message_queue)
add_subdirectory(thread)
add_subdirectory(statemachine)
add_library(basic ${MESSAGE_QUEUE_SRC_FILES} ${STATEMACHINE_DIR} ${THREAD_DIR})
target_link_libraries(basic PUBLIC pthread)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
