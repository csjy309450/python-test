#! /usr/bin/python2.7

import time
import numpy
import theano
import theano.tensor as T
import matplotlib.pyplot as plt
rng = numpy.random

N = 400                                   # training sample size
feats = 2                               # number of input variables

# generate a dataset: D = (input_values, target_class)
D = (rng.randn(N, feats), rng.randint(size=N, low=0, high=2))
#D = (((-1,-1),(-1.1,-1.2),(-1.5,-1.1),(-1.2,-1.3),(-1.5,-1.2),(1,1),(1.2,1.6),(1.0,1.2),(1.1,1.2),(1.2,1.3)),(0,0,0,0,0,1,1,1,1,1))
training_steps = 100

# Declare Theano symbolic variables
x = T.dmatrix("x")
y = T.dvector("y")

# initialize the weight vector w randomly
#
# this and the following bias variable b
# are shared so they keep their values
# between training iterations (updates)
w = theano.shared(rng.randn(feats), name="w")

# initialize the bias term
b = theano.shared(0., name="b")

print("Initial model:")
print(w.get_value())
print(b.get_value())

# Construct Theano expression graph
p_1 = 1 / (1 + T.exp(-T.dot(x, w) - b))   # Probability that target = 1
prediction = p_1 > 0.5                    # The prediction thresholded
xent = -y * T.log(p_1) - (1-y) * T.log(1-p_1) # Cross-entropy loss function
cost = xent.mean() + 0.01 * (w ** 2).sum()# The cost to minimize
gw, gb = T.grad(cost, [w, b])             # Compute the gradient of the cost
                                          # w.r.t weight vector w and
                                          # bias term b
                                          # (we shall return to this in a
                                          # following section of this tutorial)

# Compile
train = theano.function(
          inputs=[x,y],
          outputs=[prediction, xent],
          updates=((w, w - 0.1 * gw), (b, b - 0.1 * gb)))
predict = theano.function(inputs=[x], outputs=prediction)

# Train
for i in range(training_steps):
    pred, err = train(D[0], D[1])
    if i%100 == 0:
        print i, ":", numpy.mean(err)

print "cost time" + repr(time.clock())

pre = predict(D[0])

accurcy = 0.0
for i in range(0, N):
    if pre[i]==D[1][i]:
       accurcy+= float(1)/N

print pre
print D[1]
print "accurcy:", accurcy

samle_0 = D[0][D[1]==0]
samle_1 = D[0][D[1]==1]
plt.figure(figsize=(8, 5), dpi=80)
axes = plt.subplot(111)
type1 = axes.scatter(samle_0[:,0],samle_0[:,1], s=20, c='red')
type2 = axes.scatter(samle_1[:,0],samle_1[:,1], s=40, c='green')
plt.show()