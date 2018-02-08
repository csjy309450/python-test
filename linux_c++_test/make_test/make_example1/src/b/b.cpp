#include "b.h"

void func_b(){
	std::cout<<"func_b"<<std::endl;
}

void func_b1(){
	std::cout<<"----func_b1----"<<std::endl;
	func_a();
	func_b();
	std::cout<<"----end func_b1----"<<std::endl;
}
