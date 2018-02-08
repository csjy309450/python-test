#include "macro_test.h"

void macro_test(){
	std::cout<<">>>>>"<<__FUNCTION__<<std::endl;
    printf("当前源代码函数名：__FUNCTION__==%s\n",__FUNCTION__);
    printf("当前源代码行号：__LINE__==%d\n",__LINE__);
    printf("当前源代码文件名：__FILE__==%s\n",__FILE__);
    printf("当前编译日期〔注意和当前系统日期区别开来〕:__DATE__==%s\n",__DATE__);
    printf("当前编译时间〔注意和当前系统日期区别开来〕:__TIME__==%s\n",__TIME__);
    printf("当前系统时间戳：__TIMESTAMP__==%s\n",__TIMESTAMP__);
    printf("当要求程序严格遵循ANSIC标准时该标识符被赋值为1:__STDC__==%d\n",__STDC__);
    printf("当用C++编译程序编译时，标识符__cplusplus就会被定义:__cplusplus==%ld\n",__cplusplus);
}
