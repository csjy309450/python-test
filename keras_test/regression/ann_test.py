#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

import numpy as np
from keras.models import Sequential
from keras.layers import Dense
import scipy.io as sio
import matplotlib.pyplot as plt

def make_data():
    X = np.linspace(-1, 1, 200)  # 在返回（-1, 1）范围内的等差序列
    np.random.shuffle(X)  # 打乱顺序
    Y = 0.5 * X + 2 + np.random.normal(0, 0.05, (200,))  # 生成Y并添加噪声
    Z = 0.2 * Y + np.random.normal(4, 5, (200,))  # 生成Y并添加噪声
    X_data=np.empty([200, 2], dtype=np.float32)
    X_data[:, 0]=X
    X_data[:, 1] = Y
    return X_data, Z

def read_mat(path):
    mat_data = sio.loadmat(path)
    return mat_data["feature"], mat_data["label"][0]

def make_network():
    '''构建神经网络模型'''
    model = Sequential()
    model.add(Dense(input_dim=706, units=1024))
    model.add(Dense(input_dim=1024, units=960))
    model.add(Dense(input_dim=960, units=256))
    model.add(Dense(input_dim=256, units=1))

    # 选定loss函数和优化器
    model.compile(loss='mse', optimizer='sgd')
    return model

def training(model, X_train, Y_train):
    print('----------- Training -----------\n')
    # pre_cost = 0
    # for step in range(100000):
    #     cost = model.train_on_batch(X_train, Y_train)
    #     # if(abs(cost-pre_cost)<0.000001):
    #     #     return
    #     # pre_cost = cost
    #     if step % 200 == 0:
    #         print("%d step : %f" % (step, cost))

    # model.fit(X_train, Y_train, epochs=5000, batch_size=200)
    model.fit(X_train, Y_train, epochs=2, batch_size=200)
    model.save('viann_model.h5')

    print('----------- End Training -----------\n')

def test(model, X_test, Y_test):
    print('------------ Testing ------------\n')
    cost = model.evaluate(X_test, Y_test, batch_size=100)
    print('test cost:', cost)
    print('------------ End Testing ------------\n')

def predict(model, X):
    return model.predict(X)

if __name__ == '__main__':
    # X,Y = make_data()
    X,Y = read_mat('/home/yz/myPrograms/peopleCounting/gst_video_player/cmake-build-debug/via_training_set.mat')
    network=make_network()
    network.load_model('viann_model.h5')
    # training(network,X[0:199017],Y[0:199017])
    # test(network,X[199017:],Y[199017:])
    print("predict: %f, label: %f\n", predict(network, X[199017]), Y[199017])