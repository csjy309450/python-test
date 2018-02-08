set(PROJECT_NAME c++_mat)
cmake_minimum_required(VERSION 3.5)
set(CXX_SRC main.cpp)

include(mat/c_mat.cmake)

add_library(${PROJECT_NAME} STATIC ${CXX_SRC})
target_link_libraries(${PROJECT_NAME} c_mat)