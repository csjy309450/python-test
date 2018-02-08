#! /bin/sh
# 发送mp3文件
file=./test1.mp3
gst-launch-1.0 filesrc location=./test1.mp3 ! tcpclientsink host="127.0.0.1" port=5000
