from VIANN_train_predict import *

mat_path = '../via_training_set.mat'

if __name__=='__main__':
    X, Y = read_mat(mat_path)
    '''
    training model VIANN1_*
    '''
    # VIANN1_0_train(X, Y, './VIANN1/VIANN1_0_model.h5')
    # VIANN1_1_train(X, Y, './VIANN1/VIANN1_1_model.h5')
    # VIANN1_2_train(X, Y, './VIANN1/VIANN1_2_model.h5')
    # VIANN1_3_train(X, Y, './VIANN1/VIANN1_3_model.h5')
    # VIANN2_0_train(X, Y, './VIANN2/VIANN2_0_model.h5')

    '''
    test model
    '''
    # VIANN_predict_and_validat('./VIANN1/VIANN1_0_model.h5', X, Y)
    # VIANN_predict_and_validat('./VIANN1/VIANN1_1_model.h5', X, Y)
    # VIANN_predict_and_validat('./VIANN1/VIANN1_2_model.h5', X, Y)
    # VIANN_predict_and_validat('./VIANN1/VIANN1_3_model.h5', X, Y)
    '''
    predict model
    '''
    sub_X = X[0:10]
    lX=[]
    for it in sub_X:
        lX.append(list(it))
    print VIANN_predict_and_counting('./VIANN1/VIANN1_1_model.h5', lX)
