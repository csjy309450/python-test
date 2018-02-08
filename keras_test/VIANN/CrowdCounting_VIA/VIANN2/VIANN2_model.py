#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Dropout
from keras.layers import Convolution1D, Conv1D, GlobalAveragePooling1D, MaxPooling1D, Activation, Embedding, GlobalMaxPooling1D

def make_VIANN2_0():
    '''
    构建神经网络模型
    '''
    max_features = 5000
    maxlen = 706
    embedding_dims = 50
    model = Sequential()
    model.add(Embedding(max_features,
                        embedding_dims,
                        input_length=maxlen))
    # model.add(Dropout(0.2))
    model.add(Conv1D(1024, 5, activation='relu'))
    model.add(GlobalMaxPooling1D())
    model.add(Dense(512))
    model.add(Dropout(0.2))
    model.add(Activation('relu'))
    model.add(Dense(256, activation='relu'))
    model.add(Dense(1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model

def make_VIANN2_1():
    '''
    构建神经网络模型
    '''
    model = Sequential()
    model.add(Conv1D(1024, 3, activation='relu', input_shape=(None,706)))
    model.add(Conv1D(512, 3, activation='relu'))
    model.add(MaxPooling1D(3))
    model.add(Conv1D(868, 3, activation='relu'))
    model.add(Conv1D(256, 3, activation='relu'))
    model.add(GlobalAveragePooling1D())
    model.add(Dropout(0.5))
    model.add(Dense(1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model
