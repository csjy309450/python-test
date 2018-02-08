#ifndef __PYTHONTOCV_H_INCLUDED__
#define __PYTHONTOCV_H_INCLUDED__

#include <iostream>
#include <Python.h>
#include <boost/python.hpp>
#include "numpy/ndarrayobject.h"
#include <numpy/arrayobject.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

#define ERRWRAP2(expr) \
try \
{ \
    PyAllowThreads allowThreads; \
    expr; \
} \
catch (const cv::Exception &e) \
{ \
    PyErr_SetString(opencv_error, e.what()); \
    return 0; \
}

#define ARG_INPUT
#define ARG_OUTPUT
#define ARG_IN_OUT

////////////// ////////////////////////////////////////////////// /////////////
void doImport();

int failmsg(const char * fmt, ...);

//static size_t REFCOUNT_OFFSET =(size_t)&(((PyObject *)0) -> ob_refcnt)+(0x12345678!= *(const size_t *)"\ x78 \\ \\ x56 \x34\x12\0\0\0\0\0")* sizeof(int);
//
//static inline PyObject * pyObjectFromRefcount(const int * refcount)
//{
//    return(PyObject *)((size_t)refcount  -  REFCOUNT_OFFSET);
//}
//
//static inline int * refcountFromPyObject(const PyObject * obj)
//{
//	return(int *)((size_t)obj + REFCOUNT_OFFSET);
//}

class PyAllowThreads
{
public:
    PyAllowThreads() : _state(PyEval_SaveThread()) {}
    ~PyAllowThreads()
    {
        PyEval_RestoreThread(_state);
    }
private:
    PyThreadState* _state;
};

class NumpyAllocator : public MatAllocator
{
public:
    NumpyAllocator() { stdAllocator = Mat::getStdAllocator(); }
    ~NumpyAllocator() {}
    UMatData* allocate(PyObject* o, int dims, const int* sizes, int type, size_t* step) const;
    UMatData* allocate(int dims0, const int* sizes, int type, void* data, size_t* step, int flags, UMatUsageFlags usageFlags) const;
     bool allocate(UMatData* u, int accessFlags, UMatUsageFlags usageFlags) const;
     void deallocate(UMatData* u) const;

     const MatAllocator* stdAllocator;
};

class PyEnsureGIL
{
public:
    PyEnsureGIL() : _state(PyGILState_Ensure()) {}
    ~PyEnsureGIL()
    {
        PyGILState_Release(_state);
    }
private:
    PyGILState_STATE _state;
};

struct ArgInfo
{
    const char * name;
    bool outputarg;
    // more fields may be added if necessary

    ArgInfo(const char * name_, bool outputarg_)
        : name(name_)
        , outputarg(outputarg_) {}

    // to match with older pyopencv_to function signature
    operator const char *() const { return name; }
};


//////////////////////////////////////// /////////////////////////////////////////
/** @brief 将py list->opencv的Mat*/
bool pyopencv_to(PyObject* o, Mat& m, const ArgInfo info);
/** @overload */
bool pyopencv_to(PyObject* o, Mat& m, const char* name="");
/**@brief opencv Mat->py list*/
PyObject* pyopencv_from(const Mat& m);

#endif // __PYTHONTOCV_H_INCLUDED__
