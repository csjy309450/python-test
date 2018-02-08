GST_FLAG=`pkg-config --cflags --libs gstreamer-1.0`

default:
#	gcc -fpermissive -ggdb -Wall main.c -o test $(GST_FLAG)
	gcc -ggdb -Wall mp3_player.c -o mp3_player $(GST_FLAG)
	gcc -ggdb -Wall mp4_player.c -o mp4_player $(GST_FLAG)
	gcc -ggdb -Wall test2.c -o test2 $(GST_FLAG)
	gcc -ggdb -Wall test3.c -o test3 $(GST_FLAG)