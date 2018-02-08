//
// Created by yz on 17-12-5.
//

#include <vector>
#include <string>
#include "mat/c_mat.h"


int main(){
//    MATFile *pmatFile = NULL;
//    mxArray *pMxArray0 = NULL, *pMxArray1 = NULL;
//    size_t M,N;
//    char **name;
//    int n = 0;
//    // 读取.mat文件（例：mat文件名为"initUrban.mat"，其中包含"initA"）
//    long *point;
//    pmatFile = matOpen("/home/yz/testData/ucsd/labels/vidf1_33_000.y/point_mat/vidf1_33_000_f001_point_label.mat","r");
//    // 获取matfile中保存的变量列表
//    name = matGetDir(pmatFile, &n);
//    // 获取某个制定变量的数组地址
//    pMxArray0 = matGetVariable(pmatFile, "point");
//    // 获取制定数组数据区指针，需要预先知晓数据类型并显式转换，为了正确得到数据一个做法是将数组数据的类型信息与数组打包保存在.mat文件中
//    point = (long*) mxGetData(pMxArray0);
//    // 获取数组的shape
//    M = mxGetM(pMxArray0);
//    N = mxGetN(pMxArray0);
//    vector<vector<long> > A(M,vector<long>(N,0));
//    for (int i=0; i<M; i++)
//        for (int j=0; j<N; j++)
//            A[i][j] = point[M*j+i];
//
//    matClose(pmatFile);
//    mxFree(point);

//// 生成.mat文件
//    double *outA = new double[M*N];
//    for (int i=0; i<M; i++)
//        for (int j=0; j<N; j++)
//            outA[M*j+i] = A[i][j];
//    pmatFile = matOpen("A.mat","w");
//    pMxArray = mxCreateDoubleMatrix(M, N, mxREAL);
//    mxSetData(pMxArray, outA);
//    matPutVariable(pmatFile, "A", pMxArray);
//    matClose(pmatFile);
    vector<vector<long> > out;
    read_mat_2D("/home/yz/testData/ucsd/labels/vidf1_33_000.y/point_mat/vidf1_33_000_f001_point_label.mat", out);

    return 0;
}