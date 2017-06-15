GST_FLAG=`pkg-config --cflags --libs gstreamer-1.0`
BUILD_FLAG=-fpermissive -ggdb -Wall
target=main
src_target=$(target).cpp
src_gst_player=gst_player.cpp
src_build=$(src_gst_player) $(src_target)

INCLUDE_QT=-I/usr/include/qt4
LIBS_QT=-lQtCore -lQtGui

main:
#	g++ -o $(target) $(BUILD_FLAG) $(src_build) $(GST_FLAG)
	g++ main.cpp mydialog.cpp -DQT_CORE_LIB -DQT_GUI_LIB $(INCLUDE_QT) -o main $(LIBS_QT)

