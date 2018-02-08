#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

import tensorflow as tf
import numpy as np

arr_a = np.array([[1],[1],[1]])
ph_a = tf.placeholder(tf.int32, [3, 1], name='ph_a')
vec_a = tf.Variable(initial_value=tf.ones([3,1], dtype=tf.int32), name='vec_a')
init_vec_a = tf.initialize_variables([vec_a], name='init_vec_a')
vec_resulta_a = tf.add(ph_a, vec_a, name='vec_resulta_a')

arr_b = np.array([[2],[2],[2]])
ph_b = tf.placeholder(tf.int32, [3, 1], name='ph_b')
vec_b = tf.Variable(initial_value=tf.ones([3,1], dtype=tf.int32), name='vec_b')
init_vec_b = tf.initialize_variables([vec_b], name='init_vec_b')
vec_resulta_b = tf.add(ph_b, vec_b, name='vec_resulta_b')

vec_result_c = tf.add(vec_resulta_a, vec_resulta_b, name='vec_result_c')
vec_result_d = tf.add(vec_result_c, vec_b, name='vec_result_d')

with tf.Session() as sess:
    writer = tf.summary.FileWriter('./graphs', sess.graph)
    sess.run(init_vec_a)
    sess.run(init_vec_b)
    print "vec_a:\n", vec_a.eval()
    print "vec_b:\n", vec_b.eval()
    print "result:\n", sess.run(vec_result_d, feed_dict={ph_a: arr_a, ph_b: arr_b})
writer.close()