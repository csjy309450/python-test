#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

from __future__ import print_function
import tensorflow as tf
import numpy as np


kernel = tf.constant(1.0, shape=[3,3,1,1])
x1 = tf.constant(1.0, shape=[1,5,5,1])
x2 = tf.constant(1.0, shape=[1,3,3,1])
x3 = tf.constant(2.0, shape=[1,3,3,2])
x4 = tf.constant(3.0, shape=[1,3,3,3])
x5 = tf.Variable(initial_value=x2)
x6 = tf.Variable(initial_value=x3)
x7 = tf.Variable(initial_value=x4)

var_init = tf.variables_initializer([x5,x6,x7])

y1 = tf.concat_v2([x2,x3,x4], axis=3)
y2 = tf.nn.conv2d(x1, kernel, strides=[1,1,1,1], padding="VALID")
y3 = tf.nn.conv2d_transpose(x2, kernel, output_shape=[1,3,3,1], strides=[1,1,1,1], padding="SAME")


with tf.Session() as sess:
    sess.run(var_init)
    print(sess.run(y1))
    # print(sess.run(y2))
    # print(sess.run(y3))
    pass