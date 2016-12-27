# -*- coding: utf-8 -*-

"""
This example shows an icon
in the titlebar of the window.
"""

import sys
from PyQt4 import QtGui, QtCore
"""
The most simple example
"""

class Example(QtGui.QWidget):

    def __init__(self):
        super(Example, self).__init__()
        self.initUI()

    def initUI(self):
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Icon')
        self.setWindowIcon(QtGui.QIcon('Icon.png'))
        self.show()

"""
This example shows a tooltip on a window and a button, and MessageBox
"""

class Example2(QtGui.QWidget):

    def __init__(self):
        super(Example2, self).__init__()

        self.initUI()

        self.btn.clicked.connect(self.close)

    def initUI(self):

        QtGui.QToolTip.setFont(QtGui.QFont('SansSerif', 10))

        self.setToolTip('This is a <b>QWidget</b> widget')

        self.btn = btn = QtGui.QPushButton('Q', self)
        btn.setToolTip('This is a <b>QPushButton</b> widget')
        btn.resize(btn.sizeHint())
        btn.move(50, 50)

        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Tooltips')
        self.show()

    def closeEvent(self, QCloseEvent):

        reply = QtGui.QMessageBox.question(self, 'Message',
                                           "Are you sure to quit?", QtGui.QMessageBox.Yes |
                                           QtGui.QMessageBox.No, QtGui.QMessageBox.Yes)
        if reply == QtGui.QMessageBox.Yes:
            QCloseEvent.accept()
        else:
            QCloseEvent.ignore()

"""
QMainWindow example
"""
class Example3(QtGui.QMainWindow):

    def __init__(self):
        super(Example3, self).__init__()

        self.initUI()


    def initUI(self):

        self.statusBar().showMessage('Ready')

        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Statusbar')
        self.show()

"""
This program creates a menubar. The
menubar has one menu with an exit action.
"""
class Example4(QtGui.QMainWindow):

    def __init__(self):
        super(Example4, self).__init__()

        self.initUI()


    def initUI(self):
        self.statusBar()
        self.setToolTip('ToolTip frame')
        self.setStatusTip('StatusTip frame')
        self.setWhatsThis('WhatsThis frame')

        self.exitAction = exitAction = QtGui.QAction(QtGui.QIcon('Icon.png'), '&Exit', self)
        exitAction.setShortcut('Ctrl+Q')

        exitAction.triggered.connect(QtCore.QCoreApplication.instance().quit)
        exitAction.triggered.connect(self.OnChanged)

        toolbar = self.addToolBar('&Exit')
        toolbar.addAction(exitAction)
        toolbar.setToolTip('Tool bar')


        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(exitAction)

        self.setGeometry(300, 300, 300, 200)
        self.setWindowTitle('Menubar')
        self.show()

    def OnChanged(self):
        self.statusBar().showMessage('Exit application')

def main():

    app = QtGui.QApplication(sys.argv)
    ex = Example4()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()