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

image = tf.placeholder(tf.float32, shape=[100, IMAGE_SIZE[0], IMAGE_SIZE[1], IMAGE_CHANNEL], name="input_image")
annotation = tf.placeholder(tf.int32, shape=[100, IMAGE_SIZE[0], IMAGE_SIZE[1], 1], name="annotation")

kernels = tf.Variable(tf.truncated_normal(shape=(3, 3, IMAGE_CHANNEL, 32)))
bias = tf.Variable(tf.zeros(shape=32))
current = tf.nn.conv2d(image, kernels, strides=[1,1,1,1], padding='SAME',
                                   name='conv_0')
current = tf.nn.bias_add(current, bias)

current = tf.nn.relu(current, name='relu_0')

current = tf.nn.avg_pool(current, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1],
                                     padding='SAME')

kernels = tf.Variable(tf.truncated_normal(shape=(3, 3, 1, 32)))
bias = tf.Variable(tf.zeros(shape=1))
current = tf.nn.conv2d_transpose(current, filter=kernels, output_shape=[100,50,50,1],
                                             strides=[1,2,2,1], padding='SAME', name='dcon')
current = tf.nn.bias_add(current, bias)
current=tf.contrib.layers.flatten(current)

loss = tf.reduce_mean((tf.nn.sparse_softmax_cross_entropy_with_logits(logits=tf.expand_dims(current, dim=2),
                                                                          labels=tf.contrib.layers.flatten(annotation),
                                                                          name="entropy")))

optimizer = tf.train.GradientDescentOptimizer(FLAGS.learning_rate).minimize(loss)
