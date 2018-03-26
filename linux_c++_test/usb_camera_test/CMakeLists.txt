cmake_minimum_required(VERSION 3.5)
project(usb_camera_test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

set(jpeg_libs )
find_library(jpeg_libs0 jpeg /usr/lib/x86_64-linux-gnu)
find_library(jpeg_libs1 openjpeg /usr/lib/x86_64-linux-gnu)
list(APPEND jpeg_libs ${jpeg_libs1})
message(">>>${jpeg_libs}")

add_executable(usb_camera_test ${SOURCE_FILES})
target_link_libraries(usb_camera_test ${jpeg_libs})