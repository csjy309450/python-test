cmake_minimum_required(VERSION 2.4)
#add_definitions("-std=c++11")
project(glib_test)

set(SOURCE_FILES main.cpp GBoy.cpp GBoy.h GMan.cpp GMan.h)

include_directories(/usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include)
set(glib_libs )
find_library(glib_libs_0 glib-2.0 /usr/lib/x86_64-linux-gnu)
find_library(glib_libs_1 gobject-2.0 /usr/lib/x86_64-linux-gnu)
find_library(glib_libs_2 gthread-2.0 /usr/lib/x86_64-linux-gnu)
list(APPEND glib_libs ${glib_libs_0} ${glib_libs_1} ${glib_libs_2})
message(>>>${glib_libs})

add_executable(glib_test ${SOURCE_FILES})
target_link_libraries(glib_test ${glib_libs})
