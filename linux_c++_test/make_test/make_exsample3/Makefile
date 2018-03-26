step4: 
	g++ -fPIC -c -o libs2/myAPI.o libs2/myAPI.cpp
	g++ -fPIC -c -o libs1/myAPI1.o libs1/myAPI1.cpp

	g++ -fPIC -c -o libs2/myHandler2.o myHandler2.cpp
	g++ -fPIC -c -o libs1/myHandler1_1.o myHandler1_1.cpp

	g++ libs2/myAPI.o -shared -o libs2/libmyAPI2.so
	g++ libs1/myAPI1.o -shared -o libs1/libmyAPI1.so

	g++ libs2/myHandler2.o libs2/libmyAPI2.so -shared -o libs2/libmyHandler2.so
	g++ libs1/myHandler1_1.o libs1/libmyAPI1.so -shared -o libs1/libmyHandler1_1.so

	g++ main5.cpp libs1/libmyHandler1_1.so libs2/libmyHandler2.so -o main7.out
