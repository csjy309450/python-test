set(COMPILE_LIBS )

# find caffe header and libs
set(Caffe_INCLUDE_DIR /home/yz/Libs/caffe_fast_rcnn/include /home/yz/Libs/caffe_fast_rcnn/build/src)
set(Caffe_LIBRARIE_DIR /home/yz/Libs/caffe_fast_rcnn/build/lib /home/yz/Libs/caffe_fast_rcnn/build/src)
include_directories(${Caffe_INCLUDE_DIR})
link_directories(${Caffe_LIBRARIE_DIR})
find_library(Caffe_LIBRARIES caffe ${Caffe_LIBRARIE_DIR})
#message(">>> ${RUNTIME}")
list(APPEND COMPILE_LIBS ${Caffe_LIBRARIES})
message(">>> include caffe header and libs success!")

# find cuda header and libs
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})
list(APPEND COMPILE_LIBS ${CUDA_LIBRARIES})
message(">>> include CUDA header and libs success!")

# find Boost header and libs
find_package(Boost 1.58 REQUIRED COMPONENTS system thread filesystem regex)
include_directories(${Boost_INCLUDE_DIR})
list(APPEND COMPILE_LIBS ${Boost_LIBRARIES})
message(">>> include Boost header and libs success!")

# find OpenCV header and libs
find_package(OpenCV 3.1.0 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
list(APPEND COMPILE_LIBS ${OpenCV_LIBRARIES})
message(">>> include OpenCV header and libs success!")

# print compile libs
message(">>> compile libs have: ${COMPILE_LIBS}")

