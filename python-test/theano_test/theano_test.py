#! /usr/bin/python2.7
# -*- coding=utf-8 -*-

import numpy as np
import theano.tensor as T
import theano

x = T.dscalar()
y = T.dscalar()
k = T.dscalar()

z = x+y
f = theano.function([x, y], z)
z_default = x+y+k
f_default = theano.function([x, theano.In(y, value=2), theano.In(k, value=3)], z_default)

mat_x = T.dmatrix()
mat_y = T.dmatrix()

mat_z = mat_x + mat_y
f2 = theano.function([mat_x, mat_y], mat_z)

vector_a = T.dvector()
vector_out = vector_a ** 2
f3 = theano.function([vector_a], vector_out)

s = 1 / (1 + T.exp(mat_x))
f4 = theano.function([mat_x], s)

state = theano.shared(np.array([1,1,1]))
state1 = theano.shared(1)
inc = T.ivector('inc')
#accumulator = theano.function([inc], [state,state1], updates=[(state, state+inc),(state1, state1+inc)])
accumulator = theano.function([inc], outputs=state, updates=[(state, state+inc)])


#print x
#print "dscalar", f(2 ,3)

#print "dmatrix", f2([[1, 1], [1, 1], [2, 2], [2, 2]])

#print "dvector", f3([1, 2, 3])

#print f4([[1, 1], [1, 1], [2, 2], [2, 2]])
#print f_default(3)
#print y
#print state.get_value(1)

print state
for i in range(0,10):
    print accumulator([1,1,1])

theano.scan()
