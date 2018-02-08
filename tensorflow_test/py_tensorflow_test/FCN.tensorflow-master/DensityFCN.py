#!/usr/bin/python2.7
# -*-encoding=utf-8-*-

from __future__ import print_function
import tensorflow as tf
import numpy as np
import h5py as h5
import TensorflowUtils as utils
from six.moves import xrange

FLAGS = tf.flags.FLAGS
tf.flags.DEFINE_integer("batch_size", "2", "batch size for training")
tf.flags.DEFINE_string("logs_dir", "logs/", "path to logs directory")
tf.flags.DEFINE_string("data_dir", "Data_zoo/", "path to dataset")
tf.flags.DEFINE_float("learning_rate", "1e-4", "Learning rate for Adam Optimizer")
tf.flags.DEFINE_string("model_dir", "Model_zoo/", "Path to vgg model mat")
tf.flags.DEFINE_bool('debug', "False", "Debug mode: True/ False")
tf.flags.DEFINE_string('mode', "train", "Mode train/ test/ visualize")

MAX_ITERATION = int(1e5 + 1)
IMAGE_SIZE = (50, 50)
IMAGE_CHANNEL = 5

def feed_dict():
    # h5.
    pass

def density_net0(input):
    layers = (
        ('conv1_1', {'filter': (3, 3, IMAGE_CHANNEL, 32),
                     'strides': [1,1,1,1],
                     'padding': 'SAME'}),
        ('relu1_1', {}),
        ('conv1_2', {'filter': (3, 3, 32, 64),
                     'strides': [1,1,1,1],
                     'padding': 'SAME'}),
        ('relu1_2', {})
    )
    net = {}
    current = input
    for i, var in enumerate(layers):
        kind=var[0][:4]
        if kind == 'conv':
            # matconvnet: weights are [width, height, in_channels, out_channels]
            # tensorflow: weights are [height, width, in_channels, out_channels]
            kernels = tf.Variable(tf.truncated_normal(shape=var[1]['filter']), name=var[0] + "_w")
            bias = tf.Variable(tf.zeros(shape=var[1]['filter'][3]), name=var[0] + "_b")
            current = tf.nn.conv2d(current, kernels, strides=var[1]['strides'], padding=var[1]['padding'], name=var[0])
            current = tf.nn.bias_add(current, bias)
        elif kind == 'relu':
            current = tf.nn.relu(current, name=var[0])
            if FLAGS.debug:
                utils.add_activation_summary(current)
        net[var[0]] = current
    return net

def density_net1(input):
    layers = (
        ('conv2_1', {'filter': (3, 3, IMAGE_CHANNEL, 32),
                     'strides': [1,1,1,1],
                     'padding': 'SAME'}),
        ('relu2_1', {}),
        ('pool2_1', {'ksize': [1, 2, 2, 1],
                     'strides': [1, 2, 2, 1],
                     'padding': "SAME"}),
        ('dcon2_1', {'filter': (3, 3, 32, 32),
                     'strides': [1,1,1,1],
                     'padding': 'SAME',
                     'output_shape': [50, 50, 1]}),
        ('conv2_2', {'filter': (3, 3, 32, 64),
                     'strides': [1,1,1,1],
                     'padding': 'SAME'}),
        ('relu2_2', {}),
        ('pool2_2', {'ksize': [1, 2, 2, 1],
                     'strides': [1, 2, 2, 1],
                     'padding': "SAME"}),
        ('dcon2_2', {'filter': (3, 3, 64, 64),
                     'strides': [1, 1, 1, 1],
                     'padding': 'SAME',
                     'output_shape': [50, 50, 1]}),
    )
    net = {}
    current = input
    for i, var in enumerate(layers):
        kind=var[0][:4]
        if kind == 'conv':
            # matconvnet: weights are [width, height, in_channels, out_channels]
            # tensorflow: weights are [height, width, in_channels, out_channels]
            kernels = tf.Variable(tf.truncated_normal(shape=var[1]['filter']), name=var[0] + "_w")
            bias = tf.Variable(tf.zeros(shape=var[1]['filter'][3]), name=var[0] + "_b")
            current = tf.nn.conv2d(current, kernels, strides=var[1]['strides'], padding=var[1]['padding'],
                                   name=var[0])
            current = tf.nn.bias_add(current, bias)
        elif kind == 'relu':
            current = tf.nn.relu(current, name=var[0])
            if FLAGS.debug:
                utils.add_activation_summary(current)
        elif kind == 'pool':
            current = tf.nn.max_pool(current, ksize=var[1]['ksize'], strides=var[1]['strides'],
                                     padding=var[1]['padding'])
        elif kind == 'dcon':
            _shap = tf.shape(current)
            _deconv_shape = tf.stack([_shap[0],
                                    var[1]['output_shape'][0],
                                    var[1]['output_shape'][1],
                                    var[1]['output_shape'][2]])
            kernels = tf.Variable(tf.truncated_normal(shape=var[1]['filter']), name=var[0]+'_w')
            bias = tf.Variable(tf.zeros(shape=var[1]['filter'][3]), name=var[0] + "_b")
            current = tf.nn.conv2d_transpose(current, filter=kernels, output_shape=_deconv_shape,
                                             strides=var[1]['strides'], padding=var[1]['padding'], name=var[0])
            current = tf.nn.bias_add(current, bias)
        net[var[0]] = current
    return net


def inference(image):
    '''
    
    :return: 
    '''

    with tf.variable_scope("inference"):
        network0 = density_net0(image)
        network1 = density_net1(image)
        current = tf.concat_v2([network0['relu1_2'], network1['dcon2_2']], 3, name='concat_v2')
        _out_shape=tf.shape(current)
        _in_shape=tf.stack([3,3,128,1])
        kernels = tf.Variable(tf.truncated_normal(shape=_in_shape))
        bias = tf.Variable(tf.zeros(shape=1))
        current = tf.nn.conv2d(current, kernels, strides=[1,1,1,1], padding='SAME', name='conv_merge')
        current = tf.nn.bias_add(current, bias)
        current=tf.contrib.layers.flatten(current)
        return tf.expand_dims(current, dim=2)

def main(_):
    sess = tf.InteractiveSession()
    image = tf.placeholder(tf.float32, shape=[None, IMAGE_SIZE[0], IMAGE_SIZE[1], IMAGE_CHANNEL], name="input_image")
    annotation = tf.placeholder(tf.int32, shape=[None, IMAGE_SIZE[0], IMAGE_SIZE[1], 1], name="annotation")
    label=tf.contrib.layers.flatten(annotation)
    pred_annotation = inference(image)
    loss = tf.reduce_mean((tf.nn.sparse_softmax_cross_entropy_with_logits(logits=pred_annotation,
                                                                          labels=label,
                                                                          name="entropy")))
    train_writer = tf.summary.FileWriter(FLAGS.logs_dir + '/train', sess.graph)
    # sess.run([train_writer])


if __name__ == '__main__':
    tf.app.run(main)

