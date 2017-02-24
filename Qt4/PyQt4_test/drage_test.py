#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
ZetCode PyQt4 tutorial

This is a simple drag and
drop example.

author: Jan Bodnar
website: zetcode.com
last edited: December 2010
"""

import sys
from PyQt4 import QtGui, QtCore

class Button(QtGui.QPushButton):

    def __init__(self, title, parent):
        super(Button, self).__init__(title, parent)

        self.setAcceptDrops(True)

    def dragEnterEvent(self, e):

        if e.mimeData().hasFormat('text/plain'):
            e.accept()
        else:
            e.ignore()

    def dropEvent(self, e):
        self.setText(e.mimeData().text())

    def mousePressEvent(self, e):

        QtGui.QPushButton.mousePressEvent(self, e)
        if e.button() == QtCore.Qt.LeftButton:
            print 'press'


class Example(QtGui.QWidget):

    def __init__(self):
        super(Example, self).__init__()

        self.initUI()

    def initUI(self):

        edit = QtGui.QLineEdit('', self)
        edit.setDragEnabled(True)
        edit.move(30, 65)

        button = Button("Button", self)
        button.move(190, 65)

        self.setWindowTitle('Simple Drag & Drop')
        self.setGeometry(300, 300, 300, 150)


"""
如何拖放一个按钮窗口组件。
"""
class Button2(QtGui.QPushButton):

    def __init__(self, title, parent):
        super(Button2, self).__init__(title, parent)

    def mouseMoveEvent(self, e):

        if e.buttons() != QtCore.Qt.LeftButton:
            return

        mimeData = QtCore.QMimeData()

        drag = QtGui.QDrag(self)
        drag.setMimeData(mimeData)
        drag.setHotSpot(e.pos() - self.rect().topLeft())
        drag.setPixmap(QtGui.QPixmap('1.bmp'))

        dropAction = drag.start(QtCore.Qt.MoveAction)

class Example2(QtGui.QWidget):

    def __init__(self):
        super(Example2, self).__init__()

        self.initUI()

    def initUI(self):

        self.setAcceptDrops(True)

        self.button = Button2('Button', self)
        self.button.move(100, 65)

        self.setWindowTitle('Click or Move')
        self.setGeometry(300, 300, 280, 150)

    def dragEnterEvent(self, e):
        e.accept()

    def dropEvent(self, e):

        position = e.pos()
        self.button.move(position)

        # e.setDropAction(QtCore.Qt.MoveAction)
        # e.accept()


def main():

    app = QtGui.QApplication(sys.argv)
    ex = Example2()
    ex.show()
    app.exec_()


if __name__ == '__main__':
    main()