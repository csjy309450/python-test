#include "PythonToCV.h"

static PyObject* opencv_error = 0;

void doImport()
{
	import_array();
}

int failmsg(const char * fmt,...)
{
	char str [1000];

 	va_list ap;
 	va_start(ap,fmt);
 	vsnprintf(str,sizeof(str),fmt,ap);
 	va_end(ap);
 	PyErr_SetString(PyExc_TypeError,str);
 	return 0;
}

UMatData* NumpyAllocator::allocate(PyObject* o, int dims, const int* sizes, int type, size_t* step) const
{
    UMatData* u = new UMatData(this);
    u->data = u->origdata = (uchar*)PyArray_DATA((PyArrayObject*) o);
    npy_intp* _strides = PyArray_STRIDES((PyArrayObject*) o);
    for( int i = 0; i < dims - 1; i++ )
        step[i] = (size_t)_strides[i];
    step[dims-1] = CV_ELEM_SIZE(type);
    u->size = sizes[0]*step[0];
    u->userdata = o;
    return u;
}

UMatData* NumpyAllocator::allocate(int dims0, const int* sizes, int type, void* data, size_t* step, int flags, UMatUsageFlags usageFlags) const
{
    if( data != 0 )
    {
        CV_Error(Error::StsAssert, "The data should normally be NULL!");
        // probably this is safe to do in such extreme case
        return stdAllocator->allocate(dims0, sizes, type, data, step, flags, usageFlags);
    }
    PyEnsureGIL gil;

    int depth = CV_MAT_DEPTH(type);
    int cn = CV_MAT_CN(type);
    const int f = (int)(sizeof(size_t)/8);
    int typenum = depth == CV_8U ? NPY_UBYTE : depth == CV_8S ? NPY_BYTE :
        depth == CV_16U ? NPY_USHORT : depth == CV_16S ? NPY_SHORT :
        depth == CV_32S ? NPY_INT : depth == CV_32F ? NPY_FLOAT :
        depth == CV_64F ? NPY_DOUBLE : f*NPY_ULONGLONG + (f^1)*NPY_UINT;
    int i, dims = dims0;
    cv::AutoBuffer<npy_intp> _sizes(dims + 1);
    for( i = 0; i < dims; i++ )
        _sizes[i] = sizes[i];
    if( cn > 1 )
        _sizes[dims++] = cn;
    PyObject* o = PyArray_SimpleNew(dims, _sizes, typenum);
    if(!o)
        CV_Error_(Error::StsError, ("The numpy array of typenum=%d, ndims=%d can not be created", typenum, dims));
    return allocate(o, dims0, sizes, type, step);
}

bool NumpyAllocator::allocate(UMatData* u, int accessFlags, UMatUsageFlags usageFlags) const
{
    return stdAllocator->allocate(u, accessFlags, usageFlags);
}

void NumpyAllocator::deallocate(UMatData* u) const
{
    if(!u)
        return;
    PyEnsureGIL gil;
    CV_Assert(u->urefcount >= 0);
    CV_Assert(u->refcount >= 0);
    if(u->refcount == 0)
    {
        PyObject* o = (PyObject*)u->userdata;
        Py_XDECREF(o);
        delete u;
    }
}

////声明对象
//NumpyAllocator g_numpyAllocator;

/** @brief 将py list->opencv的Mat
*/
bool pyopencv_to(ARG_INPUT PyObject* o, ARG_OUTPUT Mat& m, const ArgInfo info)
{
    if(PyList_Check(o))
    {
        int nrow=static_cast<int>(PyList_Size(static_cast<PyObject*>(o)));
        if(nrow<=0)
            return false;
        int ncol=static_cast<int>(PyList_Size(static_cast<PyObject*>(PyList_GET_ITEM(o, 0))));
        if(ncol<=0)
            return false;
//        int nchannel = static_cast<int>(PyList_Size(static_cast<PyObject*>(PyList_GET_ITEM(PyList_GET_ITEM(o, 0), 0))));
        m=Mat(nrow, ncol, CV_8U);
        for(int i=0;i<nrow;i++)
        {
            PyObject *oi = (PyObject*)PyList_GET_ITEM(o, i);
            for(int j=0;j<ncol;j++)
            {
                PyObject* oj = PyList_GET_ITEM(oi, j);
                if( PyInt_Check(oj) )
                    m.at<uchar>(i,j) = (uchar)PyInt_AsLong(oj);
                else if( PyFloat_Check(oi) )
                    m.at<uchar>(i,j) = (uchar)PyFloat_AsDouble(oj);
                else if(PyList_Check(oj))
                {
                    m.at<uchar>(i,j) = (uchar)PyInt_AsLong(PyList_GET_ITEM(oj, 2));
                }
                else
                {
                    failmsg("%s is not a numerical list", info.name);
                    m.release();
                    return false;
                }
            }
        }
    }
    return true;
}

/** @overload */
bool pyopencv_to(ARG_INPUT PyObject* o, ARG_OUTPUT Mat& m, const char* name)
{
    return pyopencv_to(o, m, ArgInfo(name, 0));
}

/**@brief opencv Mat->py list
*/
PyObject* pyopencv_from(ARG_INPUT const Mat& m)
{
    if( !m.data )
        Py_RETURN_NONE;
    std::cout<<m.channels();
    PyObject *o;
    Py_ssize_t _size_rows = m.rows, _size_cols=m.cols;
    o = PyList_New(_size_rows);
#ifdef __DEBUG__
    std::cout<<"[";
#endif // __DEBUG__
    for(int i=0;i<_size_rows;i++)
    {
        PyObject *oi = PyList_New(_size_cols);
#ifdef __DEBUG__
        std::cout<<"[";
#endif // __DEBUG__
        for(int j=0;j<_size_cols;j++)
        {
            PyList_GET_ITEM(oi, j) = PyInt_FromLong(static_cast<long>(m.at<uchar>(i,j)));
#ifdef __DEBUG__
            std::cout<<(int)(m.at<uchar>(i,j))<<((j<_size_cols-1)?",":"");
#endif // __DEBUG__
        }
#ifdef __DEBUG__
        std::cout<<"],";
#endif // __DEBUG__
        PyList_GET_ITEM(o, i) = oi;
    }
#ifdef __DEBUG__
    std::cout<<"]";
#endif // __DEBUG__
    return o;
}

