#ifndef __PYTHONTOCV_H_INCLUDED__ 
#define __PYTHONTOCV_H_INCLUDED__ 
 
#include <iostream> 
#include <Python.h> 
#include <boost/python.hpp> 
#include "numpy/ndarrayobject.h"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;
 
////////////// ////////////////////////////////////////////////// ///////////// 
/// \brief导入Numpy数组。有必要避免PyArray_Check()崩溃
void doImport(); 
 
int failmsg(const char * fmt，...); 
 
static size_t REFCOUNT_OFFSET =(size_t)&(((PyObject *)0) - > ob_refcnt)+ 
(0x12345678！= *(const size_t *)“\ x78 \\ \\ x56 \x34\x12\0\0\0\0\0“)* sizeof(int); 
 
static inline PyObject * pyObjectFromRefcount(const int * refcount)
{
	return(PyObject *)((size_t)refcount  -  REFCOUNT_OFFSET); 
} 
 
static inline int * refcountFromPyObject(const PyObject * obj)
{
	return(int *)((size_t)obj + REFCOUNT_OFFSET); 
} 
 
class NumpyAllocator:public cv::MatAllocator 
{
public:
	NumpyAllocator(){} 
	~NumpyAllocator(){} 
 
	void allocate(int dims，const int * sizes，int type，int *& refcount，
		uchar *& datastart，uchar *& data，size_t * step); 
	void deallocate(int * refcount，uchar * datastart，uchar * data); 
}; 
 
 
//////////////////////////////////////// ///////////////////////////////////////// 
/// \brief转换一个numpy数组到一个cv :: Mat。这用于从Python导入图像
///。 
///这个函数是从OpenCV 2.4中的opencv / modules / python / src2 / cv2.cpp 
///中提取的
int pyopencv_to(const PyObject * o，cv :: Mat& m，const char * name =“< unknown>”，bool allowND = true); 
#endif // __PYTHONTOCV_H_INCLUDED__
