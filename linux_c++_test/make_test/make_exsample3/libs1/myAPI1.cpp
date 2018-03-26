#include "myAPI1.h"
#include <iostream>

using namespace std;
  
int ADD(int a, int b){ 
	std::cout<<"libs1: ADD"<<std::endl;
    return (a + b); 
}  
  
int MINUS(int a, int b){
	std::cout<<"libs1: MINUS"<<std::endl;
    return (a - b);
}  
