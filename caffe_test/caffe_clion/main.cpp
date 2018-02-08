//
// Created by yz on 17-12-4.
//

#include <iostream>
#include <caffe/caffe.hpp>

using namespace std;
using namespace caffe;

int main(){
    caffe::Blob<double> m_blob;
    std::cout<<m_blob.count() <<std::endl;
    return 0;
}
