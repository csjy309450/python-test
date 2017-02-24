import tensorflow as tf

initializer = tf.truncated_normal_initializer(dtype=tf.float32)
logits = var = tf.get_variable('aa', [10,5], initializer=initializer, dtype=tf.float32)
labels = tf.Variable([1,0,0,0,0,0,0,0,0,0])
eval_correct = tf.nn.in_top_k(logits, labels, 1)
top_k = tf.nn.top_k(logits, 1)

init = tf.initialize_all_variables()
sess = tf.Session()
sess.run(init)
print sess.run(logits)
print sess.run(labels)
print sess.run(eval_correct)
print sess.run(top_k)
sess.close()