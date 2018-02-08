#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

from keras.models import Sequential
from keras.layers import Dense

def make_network():
    '''
    构建神经网络模型
    '''
    model = Sequential()
    model.add(Dense(input_dim=706, units=1024))
    model.add(Dense(input_dim=1024, units=960))
    model.add(Dense(input_dim=960, units=256))
    model.add(Dense(input_dim=256, units=1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model