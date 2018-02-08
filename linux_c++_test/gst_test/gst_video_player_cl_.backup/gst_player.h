//
// Created by yangzheng on 17-6-15.
//

#ifndef GST_VIDEO_PLAYER_GST_PLAYER_H
#define GST_VIDEO_PLAYER_GST_PLAYER_H

#include <iostream>
#include <string>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gst/gstmemory.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *video_decoder;
    GstElement *video_convert;
    GstElement *video_sink;
} CustomData;

static void pad_added_handler (GstElement *src, GstPad *pad, CustomData *data);
int play(int argc, char *argv[], guintptr handle);
// test filter
static void
cb_need_data (GstElement    *appsrc,
              guint         unused_size,
              gpointer      user_data);
int filter_play(int argc, char *argv[]);


#endif //GST_VIDEO_PLAYER_GST_PLAYER_H
