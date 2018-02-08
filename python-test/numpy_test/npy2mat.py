# -*-encoding=utf-8-*-
import scipy.io as scio
import numpy as np
import os.path as path

def npy2mat(npy_path):
    npy_data = np.load(npy_path)
    mat_path = path.splitext(npy_path)[0]+'.mat'
    # print npy_data
    ## 注:要使matlab识别需使用基本数据类型list+dict保存数据
    data_list = npy_data.tolist()
    scio.savemat(mat_path, {'data': data_list})

def mat2npy(mat_path):
    mat_data = scio.loadmat(mat_path)


if __name__=='__main__':
    # npy2mat('vidf1_33_000_f001_point_label.npy')
    mat_data = scio.loadmat('vidf1_33_000_frame_full.mat')
    pass