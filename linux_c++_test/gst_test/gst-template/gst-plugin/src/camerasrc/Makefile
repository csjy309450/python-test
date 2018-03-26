CXX=gcc
PKG_NAME=camerasrc
PKG_VERSION='"0.10.23"'
CXXFLAGS=`pkg-config --cflags --libs gstreamer-1.0`
CXXD=-DPACKAGE='"GStreamer"' -DVERSION=$(PKG_VERSION) -Wimplicit -fpic -shared -ggdb -fpermissive -Wall
SRC=gst$(PKG_NAME).c compute.cpp
TARGET=libgst$(PKG_NAME).so
CXXFLAGS+=$(CXXD)
CXXFLAGS+=$(SRC)

default:
	#echo $(CXXFLAGS)
	$(CXX) $(CXXFLAGS) -o $(TARGET)

install:
	sudo cp $(TARGET) /usr/lib/x86_64-linux-gnu/gstreamer-1.0
