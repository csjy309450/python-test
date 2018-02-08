cmake_minimum_required(VERSION 2.6)
project(qt_test0)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${Qt4_INCLUDE_DIRECTORIES})
QT4_WRAP_CPP(exe_MOCS dlg_sort.hpp)
add_definitions(${QT_DEFINITIONS})

set(SOURCE_FILES dlg_sort.hpp main.cpp  ${exe_MOCS})
add_executable(qt_test0 ${SOURCE_FILES})
target_link_libraries(qt_test0 ${QT_LIBRARIES})