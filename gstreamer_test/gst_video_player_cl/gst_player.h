//
// Created by yangzheng on 17-6-15.
//

#ifndef GST_VIDEO_PLAYER_GST_PLAYER_H
#define GST_VIDEO_PLAYER_GST_PLAYER_H

#include <gstreamer-1.0/gst/gst.h>
#include <gstreamer-1.0/gst/video/videooverlay.h>

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *video_decoder;
    GstElement *video_convert;
    GstElement *video_sink;
} CustomData;

static void pad_added_handler (GstElement *src, GstPad *pad, CustomData *data);
int play(int argc, char *argv[], guintptr handle);


#endif //GST_VIDEO_PLAYER_GST_PLAYER_H
