#! /bin/sh
# 接收并播放，需先启动
gst-launch-1.0 tcpserversrc host="127.0.0.1" port=5000 ! decodebin ! autoaudiosink

