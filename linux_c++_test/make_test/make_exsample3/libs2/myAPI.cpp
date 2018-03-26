#include "myAPI.h"
#include <iostream>

using namespace std;
  
int ADD(int a, int b){ 
	std::cout<<"libs2: ADD"<<std::endl;
    return (a + b); 
}  
  
int MINUS(int a, int b){
	std::cout<<"libs2: MINUS"<<std::endl;
    return (a - b);
}  
