INCLUDE_QT=-I/usr/include/qt4
LIBS_QT=-lQtCore -lQtGui

defalt:main.cpp widget.cpp
	g++ main.cpp widget.cpp -DQT_CORE_LIB -DQT_GUI_LIB $(INCLUDE_QT) -o main $(LIBS_QT)
