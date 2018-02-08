#include<stdio.h>  
#include<stdlib.h>  
#include <iostream>  
#include <string>  
  
#include "H5Cpp.h" //包含HDF5需要的头文件  
  
#ifndef H5_NO_NAMESPACE  
using namespace H5;  
#ifndef H5_NO_STD  
using std::cout;  
using std::endl;  
#endif  // H5_NO_STD  
#endif  
  
const H5std_string FILE_NAME("h5_test.h5");//定义要创建的文件名字  
const int RANK = 2; //定义数组的维度  
const int M = 3;  
const int N = 4;  
  
int main(void)  
{  
    const H5std_string GROUP_NAME("MyGroup");//定义要创建group的名字  
    const H5std_string DATASET_NAME("MyData");//定义要创建dataset的名字  
    const H5std_string  ATTR_NAME( "MyAttribute" );//定义要创建数据集属性的名字   
  
    //准备要存储的数据  
    float *data = (float*)malloc(M*N*sizeof(float));  
    float *tmp=NULL;  
    for(int i =0; i<M; ++i){  
        for(int j =0; j<N; ++j){  
            tmp = data + i*N +j;  
            *tmp = 1.3;  
        }  
    }  
    try  
    {  
        // Turn off the auto-printing when failure occurs so that we can  
        // handle the errors appropriately  
        Exception::dontPrint();  
  
        //创建文件  
        H5File file(FILE_NAME, H5F_ACC_TRUNC);  
  
        //创建 group  
        Group group(file.createGroup(GROUP_NAME));  
  
        //创建数据空间  
        hsize_t dims[RANK];               // dataset dimensions  
        dims[0] = M;  
        dims[1] = N;  
        DataSpace *dataspace = new DataSpace (RANK, dims);  
  
        //创建数据集  
        DataSet *dataset = new DataSet (group.createDataSet(DATASET_NAME, PredType::NATIVE_FLOAT, *dataspace));  
  
        //将准备好的数据，写到数据集中。  
        dataset->write(data, PredType::NATIVE_FLOAT);  
  
        // 创建数据集属性空间.  
        //int attr_data[2] = { 100, 200};  
        //hsize_t attr_dims[1] = { 2 };  
        //DataSpace attr_dataspace = DataSpace (1,attr_dims );  
  
        // 创建数据集的属性.  
        //Attribute attribute = dataset->createAttribute( ATTR_NAME, PredType::STD_I32BE, attr_dataspace);  
  
        // 写属性.  
        //attribute.write( PredType::NATIVE_INT, attr_data);  
  
        // 关闭数据空间、数据集、group对象.  
        delete dataspace;  
        delete dataset;  
        group.close();  
    }  
      
    // catch failure caused by the H5File operations  
    catch(FileIException error)  
    {  
        error.printError();  
        return -1;  
    }  
    // catch failure caused by the DataSpace operations  
    catch(DataSpaceIException error)  
    {  
        error.printError();  
        return -1;  
    }  
    // catch failure caused by the Group operations  
    catch(GroupIException error)  
    {  
        error.printError();  
        return -1;  
    }  
    // catch failure caused by the DataSet operations  
    catch(DataSetIException error)  
    {  
        error.printError();  
        return -1;  
    }  
    return 0;  
}  
