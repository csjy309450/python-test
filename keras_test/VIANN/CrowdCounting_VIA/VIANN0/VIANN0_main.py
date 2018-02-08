#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

import numpy as np
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense
import matplotlib.pyplot as plt
from VIANN_train_predict import *
import VIANN0.VIANN0_model

mode_path = 'VIANN0_model.h5'

def VIANN0_train():
    network = VIANN0.VIANN0_model.make_network()
    training(network, X[0:199017], Y[0:199017], 'VIANN0_model.h5')

def VIANN0_predict(_mod_save_path,X,Y):
    network = load_model(_mod_save_path)
    pre_arr = np.empty((195))
    label_arr = np.empty((195))
    aix_x = range(0,195)
    for i in xrange(0, 195):
        pre = batch_predict(network, X[i*1134:(i+1)*1134, :])
        process_prediction(pre)
        pre_arr[i] = np.sum(pre)
        label_arr[i] = np.sum(Y[i*1134:(i+1)*1134])

    plt.title('VIANN0 predict')
    plt.xlabel('frame(n)')
    plt.ylabel('predict var(count)')
    plt.plot(aix_x, pre_arr, 'r', label='predict')
    plt.plot(aix_x, label_arr, 'b', label='label')
    plt.grid()
    plt.show()

if __name__ == '__main__':
    # X,Y = make_data()
    X,Y = read_mat('/home/yz/myPrograms/peopleCounting/gst_video_player/cmake-build-debug/via_training_set.mat')
    # network=make_network()
    '''
    training model
    '''
    # VIANN0_train()
    # test(network,X[199017:],Y[199017:])
    '''
    predict
    '''
    # pre = predict(network, X[0 * 1134:(0 + 1) * 1134, :])
    # print np.sum(pre), np.sum(Y[0 * 1134:(0 + 1) * 1134])
    VIANN0_predict(mode_path, X,Y)