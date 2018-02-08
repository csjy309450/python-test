#-*-encoding=utf-8-*-
import argparse
import os
import mypyt
import sys
import time
import cStringIO
import threading as th

from PyQt4.QtGui import *
from PyQt4.QtCore import *

#构造命令行参数对象
parser = argparse.ArgumentParser()
parser.add_argument('-a', dest='append', action='store_true')
parser.add_argument('-p', dest='use_python', action='store_true')
parser.add_argument('filename', nargs='?', default='typescript')
options = parser.parse_args()

## 根据参数对象,shell存放命令解释器的路径,python解释器或者bash
shell = sys.executable if options.use_python else os.environ.get('SHELL', 'sh')
## 创建文件用于保存所有信息
mode = 'ab' if options.append else 'wb'
filename = 'test.txt'
## 创建一个字符串I/O流对象
t_file = cStringIO.StringIO()

def process(data):
    if data.endswith("\r\n"):
        print data

def func():
    def read(fd):
        data = os.read(fd, 1024)
        t_file.write(data)
        process(data)
        return data
    mypyt.spawn(shell, read)

def func1():
    def read(fd):
        data = os.read(fd, 1024)
        t_file.write(data)
        process(data)
        return data
    mypyt.spawn(shell, read)



class MainWidget(QMainWindow):
    def __init__(self, parent=None):
        super(MainWidget, self).__init__(parent)
        self.edit = QLineEdit(self)
        self.edit.setGeometry(0, 0, 200, 30)
        self.label = QLabel(self)
        self.label.setGeometry(0, 31, 200, 400)
        pa = QPalette()
        pa.setColor(QPalette.Background, Qt.black)
        self.setPalette(pa)
        self.label.autoFillBackground()
        self.resize(200, 450)
        self.edit.editingFinished.connect(self.On_editingFinished)

        t1 = th.Thread(target=func)
        t1.setDaemon(True)
        t1.start()
        self.show()

    def On_editingFinished(self):
        get_cmd = str(self.edit.text())
        pid, master_fd = mypyt.fork()
        print get_cmd
        if get_cmd != '':

            shell(master_fd)

if __name__=='__main__':
    # app = QApplication(sys.argv)
    # main = MainWidget()
    # main.show()
    # app.exec_()
    func1()