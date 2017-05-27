cmake_minimum_required(VERSION 3.6)
project(gst_test_clion)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES mp4_player.c)
#set(GST_FLAG "pkg-config --cflags --libs gstreamer-1.0")
find_package(PkgConfig)
pkg_check_modules(GST QUIET gstreamer-1.0)
include_directories(${GST_INCLUDE_DIRS})


add_executable(gst_test_clion ${SOURCE_FILES})
target_link_libraries(gst_test_clion ${GST_LIBRARIES})