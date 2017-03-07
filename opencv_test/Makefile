INCLUDE_OPENCV = `$(prefix)pkg-config --cflags opencv`
LIBS_OPENCV = `$(prefix)pkg-config --libs opencv`

default:main.cpp
	g++ -o main -ggdb -O3 $(INCLUDE_OPENCV) main.cpp $(LIBS_OPENCV)
	g++ -o ColorBalance -ggdb -O3 $(INCLUDE_OPENCV) ColorBalance.cpp $(LIBS_OPENCV)
	g++ -o GammaCorrection -ggdb -O3 $(INCLUDE_OPENCV) GammaCorrection.cpp $(LIBS_OPENCV)
