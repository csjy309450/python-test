#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

from VIANN_train_predict import *

class VIANN_Caller:
    def __init__(self):
        self.feature_dim = 706
        self.is_loading = False

    def VIANN_predict_and_counting(self, _mod_save_path, X):
        if (type(_mod_save_path) is not str) or (type(X) is not list):
            print "<python warning> func args[_mod_save_path(str), X(list)] is not matching!"
        if len(X) == 0:
            print "<python warning> list X could not empty!"
        if (len(X[0])!=feature_dim):
            print "<python warning> dim of feature is not matching!"
        if self.is_loading == False:
            self.network = keras.models.load_model(_mod_save_path)
            self.is_loading = True
        arr_feature = np.empty((len(X), feature_dim))
        for i in xrange(0,len(X)):
            arr_feature[i] = X[i]
        pred = batch_predict(self.network, arr_feature)
        process_prediction(pred)
        n_person = np.sum(pred)
        #print pred
        return int(n_person)

viann = VIANN_Caller()

def call_viann(_mod_save_path, X):
    return viann.VIANN_predict_and_counting(_mod_save_path, X)
