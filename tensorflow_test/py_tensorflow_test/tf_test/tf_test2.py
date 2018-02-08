#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

import tensorflow as tf
import numpy as np
flags = tf.flags #flags是一个文件：flags.py，用于处理命令行参数的解析工作
logging = tf.logging

#调用flags内部的DEFINE_string函数来制定解析规则
flags.DEFINE_string("para_name_1","default_val", "description")
flags.DEFINE_bool("para_name_2","default_val", "description")

#FLAGS是一个对象，保存了解析后的命令行参数
FLAGS = flags.FLAGS

def main2(_):
    print FLAGS.para_name_1 #调用命令行输入的参数
    _axis = 0
    np_X = np.array([[1,1,1,1],[2,2,2,2]])
    np_Y = np.empty((np_X.shape[_axis]), dtype=np.int32)
    tf_X = tf.placeholder(tf.int32,(2,4))
    tf_Y = tf.reduce_sum(tf_X,axis=_axis)

    tf_arr = tf.Variable(tf.truncated_normal([1,2,1,2,1], stddev=0.1))
    tf_arr2 = tf.squeeze(tf_arr)
    print tf.shape(tf_arr)
    print tf.shape(tf_arr2)
    # tf_arr3 = tf.squeeze(tf_arr)
    var_init = tf.initialize_variables([tf_arr])

    with tf.Session() as sess:
        # np_Y = sess.run(tf_Y, feed_dict={tf_X: np_X})
        sess.run(var_init)
        print tf.shape(tf_arr)
        print tf.shape(tf_arr2)
        # print sess.run(tf_arr3)
    sess.close()

if __name__ == "__main__": #使用这种方式保证了，如果此文件被其它文件import的时候，不会执行main中的代码
    tf.app.run(main=main2) #解析命令行参数，调用main函数 main(sys.argv)