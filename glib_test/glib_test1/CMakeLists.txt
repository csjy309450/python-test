cmake_minimum_required(VERSION 2.4)
#add_definitions("-std=c++11")
project(glib_test)

set(SOURCE_FILES main.cpp)

include_directories(/usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include)
set(glib_libs )
find_library(glib_libs_0 glib-2.0 /usr/lib/x86_64-linux-gnu)
list(APPEND glib_libs ${glib_libs_0})
message(>>>${glib_libs})

add_executable(glib_test ${SOURCE_FILES})
target_link_libraries(glib_test ${glib_libs})
