cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME mysql_test)

set(CMAKE_CXX_STANDARD 98)

## find mysql libs
include_directories(/usr/include/)
find_library(mysql_libs mysqlclient /usr/lib/x86_64-linux-gnu)

## find boost libs
find_package(Boost 1.54.0 REQUIRED thread system log log_setup program_options)
find_library(boost_libs boost* /usr/lib/x86_64-linux-gnu)
include_directories(${Boost_INCLUDE_DIRS})
#message(">>> ${Boost_LIBRARIES}")

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${mysql_libs})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})