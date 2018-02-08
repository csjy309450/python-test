#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

import numpy as np
import keras.models
import scipy.io as sio
import matplotlib.pyplot as plt
import os.path as path

feature_dim = 706

def read_mat(_path):
    '''
    读取训练数据
    :param _path: .mat文件路径
    :return: 
    '''
    mat_data = sio.loadmat(_path)
    return mat_data["feature"], mat_data["label"][0]


def training(model, X_train, Y_train, _mod_save_path, epochs=4000, batch_size=200):
    '''
    训练模型
    :param model: 模型结构对象
    :param X_train: 训练特征数据
    :param Y_train: 标签数据
    :param _mod_save_path: 模型存放路径
    :return: 
    '''
    print '----------- Training -----------\n'
    model.fit(X_train, Y_train, epochs=epochs, batch_size=batch_size)
    # model.fit(X_train, Y_train, epochs=2, batch_size=200)
    model.save(_mod_save_path)
    print '----------- End Training -----------\n'

def test(_mod_save_path, X_test, Y_test):
    print '------------ Testing ------------\n'
    model = keras.models.load_model(_mod_save_path)
    cost = model.evaluate(X_test, Y_test, batch_size=100)
    print 'test cost:', cost
    print '------------ End Testing ------------\n'

def batch_predict(model, X):
    return model.predict(X)

def process_prediction(_pre):
    for item in _pre:
        if item[0]<0:
            item[0]=0
        else:
            item[0] = round(item[0])


def VIANN_predict_and_counting(_mod_save_path, X):
    if (type(_mod_save_path) is not str) or (type(X) is not list):
        print "<python warning> func args[_mod_save_path(str), X(list)] is not matching!"
    if len(X) == 0:
        print "<python warning> list X could not empty!"
    if (len(X[0])!=feature_dim):
        print "<python warning> dim of feature is not matching!"
    network = keras.models.load_model(_mod_save_path)
    arr_feature = np.empty((len(X), feature_dim))
    for i in xrange(0,len(X)):
        arr_feature[i] = X[i]
    pred = batch_predict(network, arr_feature)
    process_prediction(pred)
    n_person = np.sum(pred)
    #print pred
    return int(n_person)


def VIANN_predict_and_validat(_mod_save_path,X,Y):
    network = keras.models.load_model(_mod_save_path)
    pre_arr = np.empty((195))
    label_arr = np.empty((195))
    aix_x = range(0,195)
    for i in xrange(0, 195):
        pre = batch_predict(network, X[i*1134:(i+1)*1134, :])
        process_prediction(pre)
        pre_arr[i] = np.sum(pre)
        label_arr[i] = np.sum(Y[i*1134:(i+1)*1134])
    mse = np.power(np.sum(pre_arr)-np.sum(label_arr), 2)/pre_arr.shape[0]
    abs_error = np.abs(np.sum(pre_arr)-np.sum(label_arr))/pre_arr.shape[0]

    model_name, _ = path.splitext(path.split(_mod_save_path)[1])
    print "-------------- ", model_name, "-------------- "
    print 'mse', mse
    print 'abs_error', abs_error
    print "-------------------------------------- "

    plt.title(model_name)
    plt.xlabel('frame(n)')
    plt.ylabel('predict var(count)')
    plt.plot(aix_x, pre_arr, 'r', label='predict')
    plt.plot(aix_x, label_arr, 'b', label='label')
    plt.grid()
    plt.show()
