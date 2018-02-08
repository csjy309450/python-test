cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME dbm)

set(CMAKE_CXX_STANDARD 98)

include_directories(/usr/include)
find_library(gdbm_libs gdbm /usr/lib/x86_64-linux-gnu)
message(">>>${gdbm_libs}")

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${gdbm_libs})
