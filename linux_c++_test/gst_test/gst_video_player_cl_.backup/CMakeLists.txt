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

## 为了使用opencv库，要搜索opencv的include路径和libaray路径，不过
#  只要是通过opencv正常安装部署都可以这样找到。因为opencv安装的时候估
#  计会自动注册一些用于其他程序链接的环境变量
find_package(OpenCV 3.1.0 REQUIRED)
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(gst_video_player ${SOURCE_FILES} ${exe_MOCS})
# 链接动态链接库
target_link_libraries(${PROJECT_NAME} ${GST_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} gstvideo-1.0)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})