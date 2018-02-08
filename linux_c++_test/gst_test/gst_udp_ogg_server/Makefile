GST_CFLAGS=`pkg-config --cflags gtk+-2.0 gstreamer-0.10`  
GST_LIBS=`pkg-config --libs gtk+-2.0 gstreamer-0.10`
target=server

default:
	gcc -o $(target) -ggdb server.c $(GST_CFLAGS) $(GST_LIBS) 
clean:
	rm *.o
