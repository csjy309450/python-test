#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Dropout

def make_VIANN1_0():
    '''
    构建神经网络模型
    '''
    model = Sequential()
    model.add(Dense(input_dim=706, units=1024, activation='relu'))
    model.add(Dense(input_dim=1024, units=960, activation='relu'))
    model.add(Dense(input_dim=960, units=256, activation='relu'))
    model.add(Dense(input_dim=256, units=1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model

def make_VIANN1_1():
    '''
    构建神经网络模型
    '''
    model = Sequential()
    model.add(Dense(input_dim=706, units=1024, activation='relu'))
    model.add(Dense(input_dim=1024, units=256, activation='relu'))
    model.add(Dense(input_dim=256, units=1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model

def make_VIANN1_2():
    '''
    构建神经网络模型
    '''
    model = Sequential()
    model.add(Dense(input_dim=706, units=1024, activation='relu'))
    model.add(Dense(input_dim=1024, units=960, activation='relu'))
    model.add(Dropout(0.3))
    model.add(Dense(input_dim=960, units=256, activation='relu'))
    model.add(Dense(input_dim=256, units=1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model

def make_VIANN1_3():
    '''
    构建神经网络模型
    '''
    model = Sequential()
    model.add(Dense(input_dim=706, units=1024, activation='relu'))
    model.add(Dense(input_dim=1024, units=256, activation='relu'))
    model.add(Dropout(0.3))
    model.add(Dense(input_dim=256, units=1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model

