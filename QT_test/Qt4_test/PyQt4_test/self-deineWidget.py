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

        fname = QtGui.QFileDialog.getOpenFileName(self, 'Open file', '/home')
        if fname != '':
            print type(fname), fname

            f = open(fname, 'r')
            with f:
                data = f.read()
                self.textEdit.setText(data)

class Example11(QtGui.QWidget):

    def __init__(self):
        super(Example11, self).__init__()

        self.initUI()

    def initUI(self):

        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Checkbox')

        self.cb = QtGui.QCheckBox('Show title', self)
        self.cb.setFocusPolicy(QtCore.Qt.NoFocus)
        self.cb.move(10, 10)
        self.cb.toggle()
        # self.connect(self.cb, QtCore.SIGNAL('stateChanged(int)'), self.changeTitle)
        self.cb.stateChanged.connect(self.changeTitle)
        self.show()

    def changeTitle(self, value):

        if self.cb.isChecked():
            self.setWindowTitle('Checkbox')
        else:
            self.setWindowTitle('changed')

class Example12(QtGui.QWidget):

    def __init__(self):
        super(Example12, self).__init__()

        self.initUI()
        self.show()

    def initUI(self):

        self.color = QtGui.QColor(0, 0, 0)

        self.red = QtGui.QPushButton('Red', self)
        self.red.setCheckable(True)
        self.red.move(10, 10)

        self.connect(self.red, QtCore.SIGNAL('clicked()'), self.setColor)

        self.green = QtGui.QPushButton('Green', self)
        self.green.setCheckable(True)
        self.green.move(10, 60)

        self.connect(self.green, QtCore.SIGNAL('clicked()'), self.setColor)

        self.blue = QtGui.QPushButton('Blue', self)
        self.blue.setCheckable(True)
        self.blue.move(10, 110)

        self.connect(self.blue, QtCore.SIGNAL('clicked()'), self.setColor)

        self.square = QtGui.QWidget(self)
        self.square.setGeometry(150, 20, 100, 100)
        self.square.setStyleSheet("QWidget { background-color: %s }" %
            self.color.name())

        self.setWindowTitle('ToggleButton')
        self.setGeometry(300, 300, 280, 170)


    def setColor(self):

        source = self.sender()

        if source.text() == "Red":
            if self.red.isChecked():
                self.color.setRed(255)
            else: self.color.setRed(0)

        elif source.text() == "Green":
            if self.green.isChecked():
                self.color.setGreen(255)
            else: self.color.setGreen(0)

        else:
            if self.blue.isChecked():
                self.color.setBlue(255)
            else: self.color.setBlue(0)

        self.square.setStyleSheet("QWidget { background-color: %s }" %
            self.color.name())

class Example13(QtGui.QWidget):

    def __init__(self):
        super(Example13, self).__init__()

        self.initUI()
        self.show()

    def initUI(self):

        slider = QtGui.QSlider(QtCore.Qt.Horizontal, self)
        slider.setFocusPolicy(QtCore.Qt.NoFocus)
        slider.setGeometry(30, 40, 100, 30)
        # self.connect(slider, QtCore.SIGNAL('valueChanged(int)'), self.changeValue)
        slider.valueChanged.connect(self.changeValue)

        self.label = QtGui.QLabel(self)
        self.label.setPixmap(QtGui.QPixmap('1.bmp'))
        self.label.setGeometry(160, 40, 80, 30)

        self.setWindowTitle('Slider')
        self.setGeometry(300, 300, 250, 150)


    def changeValue(self, value):

        if value == 0:
            self.label.setPixmap(QtGui.QPixmap('1.bmp'))
        elif value > 0 and value <= 30:
            self.label.setPixmap(QtGui.QPixmap('2.bmp'))
        elif value > 30 and value < 80:
            self.label.setPixmap(QtGui.QPixmap('3.bmp'))
        else:
            self.label.setPixmap(QtGui.QPixmap('4.bmp'))

class Example14(QtGui.QWidget):

    def __init__(self):
        super(Example14, self).__init__()

        self.initUI()
        self.show()

    def initUI(self):

        self.pbar = QtGui.QProgressBar(self)
        self.pbar.setGeometry(30, 40, 200, 25)

        self.button = QtGui.QPushButton('Start', self)
        self.button.setFocusPolicy(QtCore.Qt.NoFocus)
        self.button.move(40, 80)

        self.connect(self.button, QtCore.SIGNAL('clicked()'),
            self.doAction)

        self.timer = QtCore.QBasicTimer()
        self.step = 0

        self.setWindowTitle('ProgressBar')
        self.setGeometry(300, 300, 250, 150)


    def timerEvent(self, event):

        if self.step >= 100:
            self.timer.stop()
            return

        self.step = self.step + 1
        self.pbar.setValue(self.step)

    def doAction(self):

        if self.timer.isActive():
            self.timer.stop()
            self.button.setText('Start')
        else:
            self.timer.start(100, self)
            self.button.setText('Stop')

class Example15(QtGui.QWidget):

    def __init__(self):
        super(Example15, self).__init__()

        self.initUI()
        self.show()

    def initUI(self):

        self.cal = QtGui.QCalendarWidget(self)
        self.cal.setGridVisible(True)
        self.cal.move(20, 20)
        self.connect(self.cal, QtCore.SIGNAL('selectionChanged()'),
            self.showDate)


        self.label = QtGui.QLabel(self)
        date = self.cal.selectedDate()
        self.label.setText(str(date.toPyDate()))
        self.label.move(130, self.cal.minimumSizeHint().height() + self.cal.height())

        self.setWindowTitle('Calendar')
        self.setGeometry(300, 300, 350, 300)

    def showDate(self):

        date = self.cal.selectedDate()
        self.label.setText(str(date.toPyDate()))

class Example16(QtGui.QWidget):

    def __init__(self):
        super(Example16, self).__init__()

        self.initUI()
        self.show()

    def initUI(self):

        # hbox = QtGui.QHBoxLayout(self)
        pixmap = QtGui.QPixmap("Icon.png")

        label = QtGui.QLabel(self)
        label.setPixmap(pixmap)

        # hbox.addWidget(label)
        # self.setLayout(hbox)

        self.setWindowTitle("Rotunda in Skalica")
        self.move(250, 200)

class Example17(QtGui.QWidget):

    def __init__(self):
        super(Example17, self).__init__()

        self.initUI()
        self.show()


    def initUI(self):

        hbox = QtGui.QHBoxLayout(self)

        topleft = QtGui.QFrame(self)
        topleft.setFrameShape(QtGui.QFrame.StyledPanel)

        topright = QtGui.QFrame(self)
        topright.setFrameShape(QtGui.QFrame.StyledPanel)

        bottom = QtGui.QFrame(self)
        bottom.setFrameShape(QtGui.QFrame.StyledPanel)

        splitter1 = QtGui.QSplitter(QtCore.Qt.Horizontal)
        splitter1.addWidget(topleft)
        splitter1.addWidget(topright)

        splitter2 = QtGui.QSplitter(QtCore.Qt.Vertical)
        splitter2.addWidget(splitter1)
        splitter2.addWidget(bottom)

        hbox.addWidget(splitter2)
        self.setLayout(hbox)

        self.setWindowTitle('QSplitter')
        QtGui.QApplication.setStyle(QtGui.QStyleFactory.create('Cleanlooks'))
        self.setGeometry(250, 200, 350, 250)

class Example18(QtGui.QWidget):

    def __init__(self):
        super(Example18, self).__init__()

        self.initUI()
        self.show()


    def initUI(self):

        self.label = QtGui.QLabel("Ubuntu", self)

        combo = QtGui.QComboBox(self)
        combo.addItem("Ubuntu")
        combo.addItem("Mandriva")
        combo.addItem("Fedora")
        combo.addItem("Red Hat")
        combo.addItem("Gentoo")

        combo.move(50, 50)
        self.label.move(50, 150)

        self.connect(combo, QtCore.SIGNAL('activated(QString)'),
            self.onActivated)

        self.setGeometry(250, 200, 350, 250)
        self.setWindowTitle('QComboBox')

    def onActivated(self, text):

        self.label.setText(text)
        self.label.adjustSize()

def main():

    app = QtGui.QApplication(sys.argv)
    ex = Example18()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()