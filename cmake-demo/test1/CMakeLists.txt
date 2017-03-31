cmake_minimum_required(VERSION 2.8)

project(test1)

set(DIR_A ./a)
set(DIR_A0 ./a/a0)
set(DIR_B0 ./b/b0)
## 为了能调试程序设置以下三个变量
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

## 为了使用opencv库，要搜索opencv的include路径和libaray路径，不过
#  只要是通过opencv正常安装部署都可以这样找到。因为opencv安装的时候估
#  计会自动注册一些用于其他程序链接的环境变量
find_package(OpenCV REQUIRED)
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "version: ${OpenCV_VERSION}")
message(STATUS "libraries: ${OpenCV_LIBS}")
message(STATUS "include path: ${OpenCV_INCLUDE_DIRS}")
# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(. SRCS_PATH)

add_subdirectory(${DIR_A})
add_subdirectory(${DIR_A0})
add_subdirectory(${DIR_B0})

add_executable(main ${SRCS_PATH})
target_link_libraries(main a a0 b0)
# 链接opencv的链接库
target_link_libraries(main ${OpenCV_LIBS})

