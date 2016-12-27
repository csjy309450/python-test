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
        btn = QtGui.QPushButton('OK', self)
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

class Example5(QtGui.QWidget):

    def __init__(self):
        super(Example5, self).__init__()

        self.initUI()

    def initUI(self):

        okButton = QtGui.QPushButton("OK")
        cancelButton = QtGui.QPushButton("Cancel")

        hbox = QtGui.QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(okButton)
        hbox.addWidget(cancelButton)

        vbox = QtGui.QVBoxLayout()
        vbox.addStretch(1)
        vbox.addLayout(hbox)

        self.setLayout(vbox)

        self.setGeometry(300, 300, 300, 150)
        self.setWindowTitle('Buttons')
        self.show()

class Example6(QtGui.QWidget):

    def __init__(self):
        super(Example6, self).__init__()

        self.initUI()

    def initUI(self):

        grid = QtGui.QGridLayout()
        self.setLayout(grid)

        names = ['Cls', 'Bck', '', 'Close',
                 '7', '8', '9', '/',
                '4', '5', '6', '*',
                 '1', '2', '3', '-',
                '0', '.', '=', '+']

        positions = [(i,j) for i in range(5) for j in range(4)]

        for position, name in zip(positions, names):

            if name == '':
                continue
            button = QtGui.QPushButton(name)
            grid.addWidget(button, *position)

        self.move(300, 150)
        self.setWindowTitle('Calculator')
        self.show()

class Example7(QtGui.QWidget):

    def __init__(self):
        super(Example7, self).__init__()

        self.initUI()

    def initUI(self):

        lcd = QtGui.QLCDNumber(self)
        sld = QtGui.QSlider(QtCore.Qt.Horizontal, self)

        vbox = QtGui.QVBoxLayout()
        vbox.addWidget(lcd)
        vbox.addWidget(sld)

        self.setLayout(vbox)
        sld.valueChanged.connect(lcd.display)

        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Signal & slot')
        self.show()

class Example8(QtGui.QWidget):

    def __init__(self):
        super(Example8, self).__init__()

        self.initUI()

    def initUI(self):

        col = QtGui.QColor(0, 0, 0)

        self.btn = QtGui.QPushButton('Dialog', self)
        self.btn.move(20, 20)

        self.btn.clicked.connect(self.showDialog)

        self.frm = QtGui.QFrame(self)
        self.frm.setStyleSheet("QWidget { background-color: %s }"
            % col.name())
        self.frm.setGeometry(130, 22, 100, 100)

        self.setGeometry(300, 300, 250, 180)
        self.setWindowTitle('Color dialog')
        self.show()

    def showDialog(self):

        col = QtGui.QColorDialog.getColor()

        if col.isValid():
            self.frm.setStyleSheet("QWidget { background-color: %s }"
                % col.name())

class Example9(QtGui.QWidget):

    def __init__(self):
        super(Example9, self).__init__()

        self.initUI()

    def initUI(self):

        vbox = QtGui.QVBoxLayout()

        btn = QtGui.QPushButton('Dialog', self)
        btn.setSizePolicy(QtGui.QSizePolicy.Fixed,
            QtGui.QSizePolicy.Fixed)

        btn.move(20, 20)

        vbox.addWidget(btn)

        btn.clicked.connect(self.showDialog)

        self.lbl = QtGui.QLabel('Knowledge only matters', self)
        self.lbl.move(130, 20)

        vbox.addWidget(self.lbl)
        self.setLayout(vbox)

        self.setGeometry(300, 300, 250, 180)
        self.setWindowTitle('Font dialog')
        self.show()

    def showDialog(self):

        font, ok = QtGui.QFontDialog.getFont()
        if ok:
            self.lbl.setFont(font)

class Example10(QtGui.QMainWindow):

    def __init__(self):
        super(Example10, self).__init__()

        self.initUI()

    def initUI(self):

        self.textEdit = QtGui.QTextEdit()
        self.setCentralWidget(self.textEdit)
        self.statusBar()

        openFile = QtGui.QAction(QtGui.QIcon('open.png'), 'Open', self)
        openFile.setShortcut('Ctrl+O')
        openFile.setStatusTip('Open new File')
        openFile.triggered.connect(self.showDialog)

        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(openFile)

        self.setGeometry(300, 300, 350, 300)
        self.setWindowTitle('File dialog')
        self.show()

    def showDialog(self):

        fname = QtGui.QFileDialog.getOpenFileName(self, 'Open file',
                '/home')
        print type(fname), fname

        f = open(fname, 'r')
        with f:
            data = f.read()
            self.textEdit.setText(data)

def main():

    app = QtGui.QApplication(sys.argv)
    ex = Example10()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()