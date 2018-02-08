set(PROJECT_NAME caffe_clion)
cmake_minimum_required(VERSION 3.5)

include("cmake/Dependance.cmake")

set(CXX_SRC main.cpp)

add_executable(${PROJECT_NAME} ${CXX_SRC})
target_link_libraries(${PROJECT_NAME} ${COMPILE_LIBS})