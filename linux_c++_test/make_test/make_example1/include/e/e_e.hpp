#ifndef _E_E_H_
#define _E_E_H_
#include <stdio.h>
#include <iostream>
#include "a.h"
#include "d.h"

void func_e_e(){
	printf("--------func_e_e-------\n");
	func_a();
	func_d();
	printf("--------end func_e_e-------\n");
}

#endif
