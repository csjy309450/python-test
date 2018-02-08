import test0
import cv2

def add(a,b):  
    print "in python function add"  
    print "a = " + str(a)  
    print "b = " + str(b)  
    print "ret = " + str(a+b)  
    return  
  
def foo(a):  
  
    print "in python function foo"  
    print "a = " + str(a)  
    print "ret = " + str(a * a)  
    return   
  
class guestlist:  
    def __init__(self):  
        print "aaaa"  
    def p():  
      print "bbbbb"  
    def __getitem__(self, id):  
      return "ccccc"  

def update(): 
    guest = guestlist()  
    print guest['aa']

def test_0():
	l=[1,2,3,4,5,6]
	print l 
	print "sum", test0.sum(l)
	print "add", test0.add(9,1)
	print "min", test0.min(9,1)

def test_cv():
	img = cv2.imread('/home/yz/testData/test1.png',0)
	img = cv2.resize(img, (100,500))
	cv2.imshow("aa", img)
	cv2.waitKey()
