#include "CallPython.h"

CallPython::CallPython(){
    Py_Initialize();
    if(!Py_IsInitialized())
        throw "python iterator is not prepared!";
}
CallPython::~CallPython(){
    Py_Finalize();
}

void CallPython::numpy_test(){
    PyObject *arr;
    int dim=2, typenum=NPY_FLOAT;
    npy_intp *_sizes = new npy_intp[2];
    _sizes[0]=5;
    _sizes[1]=10;
    arr = PyArray_SimpleNew(dim, _sizes, typenum);
}
/**@brief 使用python提供的方法处理输入图像
@param Img 输入输出图像
@param pFunc python方法对象， 要求方法的输入参数为list对象, 返回值也是list对象
*/
bool CallPython::getMatFromPyFunc(
    ARG_IN_OUT Mat& Img,
    PyObject *pFunc){
    if(!pFunc||!PyCallable_Check(pFunc))
        return false;

    PyObject *pyListImg, *pArgs, *pResult, *pr;
    pyListImg=pyopencv_from(Img);
    pArgs=PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("O",pyListImg));
    pResult = PyObject_CallObject(pFunc, pArgs);
    pr = PyObject_CallObject(pFunc, pArgs);
    if(!pResult)
        return false;
    pyopencv_to(pResult, Img);
//    Py_DECREF(pyListImg);
//    Py_DECREF(pArgs);
//    Py_DECREF(pResult);
    return true;
}

int CallPython::test0(){
    PyObject *pName, *pModule, *pDict, *pFunc;
    pName = PyString_FromFormat("cvtest");
    pModule = PyImport_Import(pName);
    if(!pModule){
        std::cerr<<"can't find cvtest";
        return -1;
    }
    pDict = PyModule_GetDict(pModule);
    if(!pDict)
        return -1;
    pFunc = PyDict_GetItemString(pDict, "cvtest3");
    if(!pFunc||!PyCallable_Check(pFunc))
    {
        std::cerr<<"can't find function(cvtest)";
        return -1;
    }
    Mat m=imread("/home/yz/testData/crown.jpg", IMREAD_GRAYSCALE);
    getMatFromPyFunc(m,pFunc);

#ifdef __DEBUG__
    imshow("img", m);
    waitKey();
    destroyAllWindows();
#endif // __DEBUG__

//    Py_DECREF(pName);
//    Py_DECREF(pModule);
//    Py_DECREF(pDict);
//    Py_DECREF(pFunc);
    return 0;
}

int CallPython::test1(){
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pResult;
    pName = PyString_FromFormat("cvtest");
    pModule = PyImport_Import(pName);
    if(!pModule){
        std::cerr<<"can't find cvtest";
        return -1;
    }
    pDict = PyModule_GetDict(pModule);
    if(!pDict)
        return -1;
    pFunc = PyDict_GetItemString(pDict, "cvtest");
    if(!pFunc||!PyCallable_Check(pFunc))
    {
        std::cerr<<"can't find function(cvtest)";
        return -1;
    }
    pArgs=PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "/home/yz/testData/crown.jpg"));
    pResult = PyObject_CallObject(pFunc, pArgs);
    if(!pResult)
        return -1;

    Mat *pImg=new Mat;
    pyopencv_to(pResult, *pImg, "img");

#ifdef __DEBUG__
    imshow("img", *pImg);
    waitKey();
    destroyAllWindows();
#endif // __DEBUG__

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pDict);
    Py_DECREF(pResult);
    Py_DECREF(pFunc);
    return 0;
}

int CallPython::test2(){
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pResult;
    pName = PyString_FromFormat("cvtest");
    pModule = PyImport_Import(pName);
    if(!pModule){
        std::cerr<<"can't find cvtest";
        return -1;
    }
    pDict = PyModule_GetDict(pModule);
    if(!pDict)
        return -1;
    pFunc = PyDict_GetItemString(pDict, "cvtest2");
    if(!pFunc||!PyCallable_Check(pFunc))
    {
        std::cerr<<"can't find function(cvtest2)";
        return -1;
    }
    Mat img = imread("/home/yz/testData/test1.png", IMREAD_GRAYSCALE);
    PyObject *pImg=pyopencv_from(img);
    pArgs=PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("O",pImg));
    pResult = PyObject_CallObject(pFunc, pArgs);
    if(!pResult)
        return -1;

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pDict);
    Py_DECREF(pResult);
    Py_DECREF(pFunc);
    Py_DECREF(pImg);
    return 0;
}
