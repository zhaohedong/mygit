aux_source_directory(./service TASK_QUEUE_DIR)

add_subdirectory(service)
add_library(basic_cpp ${TASK_QUEUE_DIR})
# target_link_libraries(basic_cpp PUBLIC pthread)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

