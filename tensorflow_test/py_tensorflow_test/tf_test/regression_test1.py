import tensorflow as tf
import numpy as np

data = np.array([[1,1],[2,2],[3,3]])

X = tf.placeholder(tf.float32, shape=[], name='input')
Y = tf.placeholder(tf.float32, shape=[], name='label')
w = tf.get_variable('weight', shape=[], initializer=tf.truncated_normal_initializer())
b = tf.get_variable('bias', shape=[], initializer=tf.zeros_initializer)
Y_predicted = w * X + b
loss = tf.square(Y - Y_predicted, name='loss')
optimizer = tf.train.GradientDescentOptimizer(learning_rate=1e-3).minimize(loss)

init = tf.global_variables_initializer()

sess = tf.Session()
writer = tf.summary.FileWriter('./linear_log', graph=sess.graph)
sess.run(init)
for i in range(100):
    total_loss = 0
    for x, y in data:
        _, l = sess.run([optimizer, loss], feed_dict={X: x, Y: y})
        total_loss += l
    print("Epoch {0}: {1}".format(i, total_loss / data.shape[0]))
sess.close()