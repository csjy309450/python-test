#include "a.h"
#include "b.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

char * g_name = "hello world";

void tty_test(){
	const char * _name = "test.txt\0";
	freopen(_name, "w", stdout);
	execl("/bin/sh", "sh", 0); 
}

void tty_test2(){
	char cmd[1024]="\0";
	char result[1024]="\0";
	//fread(result, sizeof(char), 1024, pip);
	//printf("%s", result);
	while(1){
		int n=0;
		gets(cmd);
		FILE * pip = popen(cmd, "r");
		n = fread(result, sizeof(char), 1024, pip);
		while(n>0){
			n = fread(result, sizeof(char), 1024, pip);
			printf("%s", result);
		}
		cmd[0]=0;
		pclose(pip);
	}
}

int main(){
	/*tty_test();
	size_t n=32;
	char * str = (char*)malloc(n*sizeof(char));
	memset(str, '1', n*sizeof(char));
	str[n-1]=0;
	printf("%s\n", str);

	while(1);
		//printf("1\n");
	*/
	tty_test2();
	
	return 0;
}
