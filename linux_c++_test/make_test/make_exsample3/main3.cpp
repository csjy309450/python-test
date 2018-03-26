#include <iostream>
#include "myAPI.h"
#include "myHandler.h"

using namespace std;

int main(){
	std::cout<<"ADD(1,2) = "<<ADD(1,2)<<std::endl;
	std::cout<<"MINUS(1,2) = "<<MINUS(1,2)<<std::endl;
	std::cout<<"mutil_ADD(1,2,3) = "<<mutil_ADD(1,2,3)<<std::endl;
	std::cout<<"mutil_MINUS(1,2,3) = "<<mutil_MINUS(1,2,3)<<std::endl;
}
