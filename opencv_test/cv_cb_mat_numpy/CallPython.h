#ifndef CALLPYTHON_H_INCLUDED
#define CALLPYTHON_H_INCLUDED

#include "PythonToCV.h"

class CallPython
{
public:
    CallPython();
    ~CallPython();
    void numpy_test();
    /**@brief 使用python提供的方法处理输入图像
    @param Img 输入输出图像
    @param pFunc python方法对象， 要求方法的输入参数为list对象, 返回值也是list对象
    */
    bool getMatFromPyFunc(
        ARG_IN_OUT Mat& Img,
        PyObject *pFunc);
    int test0();
    int test1();
    int test2();
};


#endif // CALLPYTHON_H_INCLUDED
