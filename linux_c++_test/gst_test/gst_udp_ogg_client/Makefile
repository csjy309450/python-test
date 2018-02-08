GST_CFLAGS=`pkg-config --cflags gtk+-2.0 gstreamer-1.0`  
GST_LIBS=`pkg-config --libs gtk+-2.0 gstreamer-1.0`
target=client

default:
	gcc -o $(target) -ggdb client.c $(GST_CFLAGS) $(GST_LIBS) 
clean:
	rm *.o
