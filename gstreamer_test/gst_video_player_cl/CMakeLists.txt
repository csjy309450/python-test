cmake_minimum_required(VERSION 3.6)
project(gst_video_player)

#set(CMAKE_C_STANDARD 98)
set(PROJECT_NAME gst_video_player)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
set(SOURCE_FILES main.cpp gst_player.cpp mydialog.cpp)
add_compile_options(-fpermissive)


# 寻找qt4 pkg
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${Qt4_INCLUDE_DIRECTORIES})
QT4_WRAP_CPP(exe_MOCS mydialog.h)
add_definitions(${QT_DEFINITIONS})

#寻找gst pkg
find_package(PkgConfig)
pkg_check_modules(GST QUIET gstreamer-1.0)
include_directories(${GST_INCLUDE_DIRS})
#set(GST_FLAG "pkg-config --cflags --libs gstreamer-1.0")
#include_directories(${GST_FLAG})


add_executable(gst_video_player ${SOURCE_FILES} ${exe_MOCS})
# 链接动态链接库
target_link_libraries(${PROJECT_NAME} ${GST_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} gstvideo-1.0)