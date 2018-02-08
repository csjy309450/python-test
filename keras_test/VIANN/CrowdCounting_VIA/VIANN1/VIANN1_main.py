#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

import VIANN1.VIANN1_model
from VIANN_train_predict import *

trainingSet_path = '/home/yz/myPrograms/peopleCounting/gst_video_player/cmake-build-debug/via_training_set.mat'

def VIANN1_0_train(X,Y,mode_path='VIANN1_0_model.h5'):
    network = VIANN1.VIANN1_model.make_VIANN1_0()
    trainSet_indx = np.random.randint(0, X.shape[0], 199017)
    testSet_indx = np.setdiff1d(np.arange(0, X.shape[0]), trainSet_indx)
    training(network, X[trainSet_indx], Y[trainSet_indx], mode_path)
    test(mode_path, X[testSet_indx], Y[testSet_indx])

def VIANN1_1_train(X,Y,mode_path='VIANN1_1_model.h5'):
    network = VIANN1.VIANN1_model.make_VIANN1_1()
    trainSet_indx = np.random.randint(0, X.shape[0], 199017)
    testSet_indx = np.setdiff1d(np.arange(0, X.shape[0]), trainSet_indx)
    training(network, X[trainSet_indx], Y[trainSet_indx], mode_path)
    test(mode_path, X[testSet_indx], Y[testSet_indx])

def VIANN1_2_train(X,Y,mode_path='VIANN1_2_model.h5'):
    network = VIANN1.VIANN1_model.make_VIANN1_2()
    trainSet_indx = np.random.randint(0, X.shape[0], 199017)
    testSet_indx = np.setdiff1d(np.arange(0, X.shape[0]), trainSet_indx)
    training(network, X[trainSet_indx], Y[trainSet_indx], mode_path)
    test(mode_path, X[testSet_indx], Y[testSet_indx])

def VIANN1_3_train(X, Y, mode_path='VIANN1_3_model.h5'):
    network = VIANN1.VIANN1_model.make_VIANN1_3()
    trainSet_indx = np.random.randint(0, X.shape[0], 199017)
    testSet_indx = np.setdiff1d(np.arange(0, X.shape[0]), trainSet_indx)
    training(network, X[trainSet_indx], Y[trainSet_indx], mode_path)
    test(mode_path, X[testSet_indx], Y[testSet_indx])

if __name__ == '__main__':
    # X,Y = make_data()
    X, Y = read_mat(trainingSet_path)
    # network=make_network()
    '''
    training model VIANN1_*
    '''
    VIANN1_0_train(X, Y, 'VIANN1_0_model.h5')
    VIANN1_1_train(X, Y, 'VIANN1_1_model.h5')
    VIANN1_2_train(X, Y, 'VIANN1_2_model.h5')
    VIANN1_3_train(X, Y, 'VIANN1_3_model.h5')
    '''
    predict
    '''
    # VIANN1_predict('VIANN1_0_model.h5', X, Y)