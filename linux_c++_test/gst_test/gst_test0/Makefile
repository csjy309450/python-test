CFLAGS=`pkg-config --cflags gtk+-2.0 gstreamer-1.0`  
LIBS=`pkg-config --libs gtk+-2.0 gstreamer-1.0`
target=player

default:
	gcc -o $(target) -ggdb player.c player.h $(LIBS) $(CFLAGS)
clean:
	rm *.o
