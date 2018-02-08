# -*- coding: utf-8 -*-  
from PyQt4.QtGui import *  
from PyQt4.QtCore import *  
import sys  
QTextCodec.setCodecForTr(QTextCodec.codecForName("utf8"))  
  
class KeyEvent(QWidget):  
    def __init__(self,parent=None):  
        super(KeyEvent,self).__init__(parent)  
        self.setWindowTitle(self.tr("获得键盘事件"))  
          
        self.pix = QPixmap()  
        self.pix.load("crow5.jpg")  
        self.image = QImage("crow5.jpg")  
        self.width = 400  
        self.height = 300 
        self.step = 2  
        self.startX = 0  
        self.startY = 0   
        self.resize(self.width,self.height)  
          
    def drawPix(self):  
        self.pix.fill(Qt.white)  
        painter = QPainter(self.pix)  
        pen = QPen(Qt.DotLine)  
        painter.setPen(pen)  
        for i in xrange(self.step,self.width,self.step):  
            painter.drawLine(QPoint(i,0),QPoint(i,self.height))  
        for j in xrange(self.step,self.height,self.step):  
            painter.drawLine(QPoint(0,j),QPoint(self.width,j))  
        painter.drawImage(QPoint(self.startX,self.startY),self.image)  
      
    def keyPressEvent(self,event):  
        if event.modifiers() == Qt.ControlModifier:  
            if event.key() == Qt.Key_Left:  
                self.startX = self.startX - 1 < 0 and self.startX or self.startX - 1  
            if event.key() == Qt.Key_Right:  
                self.startX = self.startX + 1 + self.image.width() > self.width and self.startX or self.startX + 1  
            if event.key() == Qt.Key_Up:  
                self.startY = self.startY - 1 < 0 and self.startY or self.startY - 1  
            if event.key() == Qt.Key_Down:  
                self.startY = self.startY + 1 + self.image.height() > self.height and self.startY or self.startY + 1  
        else:  
            self.startX = self.startX - self.startX%self.step  
            self.startY = self.startY - self.startY%self.step  
              
            if event.key() == Qt.Key_Left:  
                self.startX = self.startX - self.step < 0 and self.startX or self.startX - self.step  
            if event.key() == Qt.Key_Right:  
                self.startX = (self.startX + self.step + self.image.width() > self.width ) and self.startX or self.startX + self.step  
            if event.key() == Qt.Key_Up:  
                self.startY = (self.startY - self.step < 0) and self.startY or self.startY - self.step  
            if event.key() == Qt.Key_Down:  
                self.startY = (self.startY + self.step + self.image.height() > self.height) and self.startY or self.startY + self.step   
            if event.key() == Qt.Key_Home:  
                self.startX = 0  
                self.startY = 0  
            if event.key() == Qt.Key_End:  
                self.startX = self.width - self.image.width()  
                self.startY = self.height - self.image.height()  
        self.drawPix()  
        self.update()  
      
    def paintEvent(self,event):  
        painter = QPainter(self)  
        painter.drawPixmap(QPoint(0,0),self.pix)  
      
app=QApplication(sys.argv)  
dialog=KeyEvent()  
dialog.show()  
app.exec_()  
