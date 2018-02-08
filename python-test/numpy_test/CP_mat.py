#! /usr/bin/python2.7
# -*-encoding=utf-8-*-

import numpy as np
import scipy.io as sio

def load_mat(mat_path, var_name):
    """
        读取.mat文件并以list返回矩阵数组
        :param mat_path: ./mat文件路径
        :param var_name: 要提取的数组变量的名称
        :return: 
        """
    data = sio.loadmat(mat_path)
    mat = data[var_name]
    lmat = []
    for it in mat:
        lmat.append(list(it))
    return lmat

def save_mat(mat_path, data):
    """
    将data中的数据以.mat文件保存
    :param mat_path: ./mat文件路径
    :param data: 要存入文件的数据，要求是dict数据结构
    :return: 
    """
    assert(type(data) is dict)
    # print(data["feature"])
    sio.savemat(mat_path, data)

if __name__ == "__main__":
    #load_mat("/home/yz/testData/ucsd/labels/vidf1_33_000.y/gaussain_mat/vidf1_33_000_f001_gaussian_label.mat","gaussian_labe_data")
    data_dict = {'a': [[1,1,1], [2,2,2], [3,3,3]], 'b': 0.000000001}
    save_mat('data_dict.mat', data_dict)
    pass