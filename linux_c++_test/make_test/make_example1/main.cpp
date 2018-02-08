#include "a.h"
#include "b.h"
#include "c.h"
#include "d.h"
#include "e.h"
#include "e_e.hpp"
#include "e_2.h"
#include "macro_test.h"

int main(){
	func_a();
	func_b();
	func_c();
	func_b1();
	func_d();
	func_e();
	func_e_e();
	func_e_2();
	macro_test();
	
	size_t n=4;
	int * data = new int[n];
	for(size_t i=0;i<n;i++){
		*(data+i)=i;
		printf("%d", *(data+i));
	}
	
	return 0;
}
