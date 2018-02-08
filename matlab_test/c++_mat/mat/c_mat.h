//
// Created by yz on 17-12-7.
//

#ifndef PROJECT_C_MAT_H
#define PROJECT_C_MAT_H

#include <vector>
#include <string>
#include <mat.h>

using namespace std;

void read_mat_2D(const string _path, vector<vector<long> > & _out);
template<class T_item>
void read_mat_2D(const string _path, vector<vector<T_item> > & _out);

///
template<class T_item>
void read_mat_2D(const string _path, vector<vector<T_item> > & _out){
    MATFile *pmatFile = NULL;
    mxArray *pMxArray = NULL;
    size_t M,N;

    // 读取.mat文件（例：mat文件名为"initUrban.mat"，其中包含"initA"）
    T_item *buffer;
    pmatFile = matOpen(_path.c_str(),"r");
    pMxArray = matGetVariable(pmatFile, "point");
    buffer = (T_item*) mxGetData(pMxArray);
    M = mxGetM(pMxArray);
    N = mxGetN(pMxArray);
    if(_out.size()<M)
        _out.resize(M, vector<T_item>(N, 0));
    for(size_t i=0; i<M; i++){
        if(_out[i].size()<N)
            _out[i].resize(N,0);
        for(size_t j=0; j<N; j++)
            _out[i][j] = buffer[M*j+i];
    }

    matClose(pmatFile);
    mxFree(buffer);
}


#endif //PROJECT_C_MAT_H
