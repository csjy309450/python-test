CXX_SRCS:=myAPI.cpp test1.cpp
CXX_OBJS:=myAPI.o test1.o
CXX_PATH:=.

step1: step2
	echo main
	g++ main.cpp -L. -lmyAPI -o main
	echo finish main

step2: $(CXX_OBJS)
	echo libmyAPI.so
	g++ $(CXX_OBJS) -fPIC -shared -o libmyAPI.so

test1.o: test1.cpp
	echo $<
	g++ -fPIC -c -o $@ $<
