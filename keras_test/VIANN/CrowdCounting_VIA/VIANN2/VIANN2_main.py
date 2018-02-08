#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

from __future__ import print_function

import numpy as np
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense
import matplotlib.pyplot as plt
from VIANN_train_predict import *
import VIANN2.VIANN2_model


from keras.preprocessing import sequence
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation
from keras.layers import Embedding
from keras.layers import Conv1D, GlobalMaxPooling1D
from keras.datasets import imdb

trainingSet_path = '/home/yz/myPrograms/peopleCounting/gst_video_player/cmake-build-debug/via_training_set.mat'

def VIANN2_0_train(X,Y,mode_path='VIANN2_0_model.h5',epochs=4000, batch_size=200):
    network = VIANN2.VIANN2_model.make_VIANN2_0()
    trainSet_indx = np.random.randint(0, X.shape[0], 199017)
    testSet_indx = np.setdiff1d(np.arange(0, X.shape[0]), trainSet_indx)
    training(network, X[trainSet_indx], Y[trainSet_indx], mode_path, epochs=epochs, batch_size=batch_size)
    test(mode_path, X[testSet_indx], Y[testSet_indx])

def VIANN2_1_train(X,Y,mode_path='VIANN2_1_model.h5', epochs=4000, batch_size=200):
    network = VIANN2.VIANN2_model.make_VIANN2_1()
    trainSet_indx = np.random.randint(0, X.shape[0], 199017)
    testSet_indx = np.setdiff1d(np.arange(0, X.shape[0]), trainSet_indx)
    training(network, X[trainSet_indx], Y[trainSet_indx], mode_path,epochs=epochs, batch_size=batch_size)
    test(mode_path, X[testSet_indx], Y[testSet_indx])

def VIANN2_predict(_mod_save_path,X,Y):
    network = load_model(_mod_save_path)
    pre_arr = np.empty((195))
    label_arr = np.empty((195))
    aix_x = range(0,195)
    for i in xrange(0, 195):
        pre = batch_predict(network, X[i*1134:(i+1)*1134, :])
        process_prediction(pre)
        pre_arr[i] = np.sum(pre)
        label_arr[i] = np.sum(Y[i*1134:(i+1)*1134])

    plt.title('VIANN2 predict')
    plt.xlabel('frame(n)')
    plt.ylabel('predict var(count)')
    plt.plot(aix_x, pre_arr, 'r', label='predict')
    plt.plot(aix_x, label_arr, 'b', label='label')
    plt.grid()
    plt.show()

if __name__ == '__main__':
    # X,Y = make_data()
    X, Y = read_mat(trainingSet_path)
    # network=make_network()
    '''
    training model VIANN2_*
    '''
    VIANN2_0_train(X, Y, 'VIANN2_0_model.h5', epochs=4000, batch_size=200)
    # VIANN2_1_train(X, Y, 'VIANN2_1_model.h5', epochs=4000, batch_size=200)
    # test()
    '''
    predict
    '''
    # VIANN1_predict('VIANN1_0_model.h5', X, Y)