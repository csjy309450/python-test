CXX=g++

INCLUDE_OPENCV = `$(prefix)pkg-config --cflags opencv`
LIBS_OPENCV = `$(prefix)pkg-config --libs opencv`

# You need to change this path according to your cuda installation
CUDA_PATH=/usr/local/cuda

# You need to change this path according to your caffe installation
CAFF_PATH=/home/yangzheng/Libs/caffe


CFLAGS=-c -Wall -std=c++0x -g3 -Ofast -msse2 $(INCLUDE_OPENCV) -I. -I$(CUDA_PATH)/include -I$(CAFF_PATH)/include/ -I$(CAFF_PATH)/build/src/
LDFLAGS= -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -L$(CUDA_PATH)/lib64 $(LIBS_OPENCV) -L$(CUDA_PATH)/lib -lcudart -lcublas -lcurand -lglog -lgflags -lprotobuf -lleveldb -lsnappy -llmdb -lboost_system  -lhdf5 -lhdf5_hl -lm -lopencv_core -lopencv_highgui -lopencv_imgproc -lboost_thread -lstdc++  -lcblas -latlas -L$(CAFF_PATH)/build/lib/ -lcaffe

# You need to change this flag if you want to use GPU
DEFINE= -D USE_OPENCV

SOURCES=${wildcard main.cpp }
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=classification

all:  $(SOURCES) $(EXECUTABLE)


clean:
	rm -f *.o
	rm -f $(EXECUTABLE)
	find ./ -name \*.o  -delete

$(EXECUTABLE): $(OBJECTS)
	@echo 'Linking'
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	@echo 'Compiling'
	$(CXX) $(DEFINE) $(CFLAGS) $< -o $@
