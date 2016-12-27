import numpy as np

# a = np.eye(3, M=5, k=2)
# b = np.identity(5,dtype=np.float16)
# print a
# print b

# c = np.arange(9).reshape((3,3))
# np.diag(c)
#
# print c
# print np.diag(c)
# print np.diag(np.diag(c))

# d = np.matrix([[1,2,3],[4,5,6],[7,8,9]])
# print type(d)

x = np.array([[[1],[2],[3]], [[4],[5],[6]]])
y = np.array([[[[1],[2],[3]],[[4],[5],[6]],[[7],[8],[9]]], [[[10],[11],[12]],[[13],[14],[15]],[[16],[17],[18]]]])
z = np.array([[1,2],[3,4]])
k = np.asscalar(np.array([0]))
print y.ndim
print y[...,0]
print y[0,2,2,0]
print y.flags
#print y.item()
print y.flatten()
print z.T

print k

