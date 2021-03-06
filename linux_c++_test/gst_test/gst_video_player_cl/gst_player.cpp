//
// Created by yangzheng on 17-6-15.
//

#include "gst_player.h"

const int g_width=500;
const int g_height=300;

static GMainLoop *loop;
const string dir_path("/home/yz/testData/");

int play(int argc, char *argv[], guintptr handle) {
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;
    char file_path[]="/home/yz/testData/hand.mp4";
    /* Initialize GStreamer */
    gst_init (&argc, &argv);
    /* Create the elements */
    data.source = gst_element_factory_make ("filesrc", "source");
    data.video_decoder = gst_element_factory_make("decodebin","video_decoder");
    data.video_convert = gst_element_factory_make("videoconvert","video_convert");
    //g_signal_connect (data.video_convert->pads->data, "need-data", G_CALLBACK(cb_need_data), NULL);
    data.video_sink = gst_element_factory_make("xvimagesink","video_sink");
    //制定播放窗口
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(data.video_sink),handle);

    data.pipeline = gst_pipeline_new ("test-pipeline");
    if (!data.pipeline || !data.source || !data.video_decoder || !data.video_convert || !data.video_sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    gst_bin_add_many (GST_BIN (data.pipeline), data.source,data.video_decoder,data.video_convert,data.video_sink, NULL);
    if(!gst_element_link_many(data.source,data.video_decoder,NULL)){
        g_printerr("video Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }
    //连接视频分支pad
    if (!gst_element_link_many(data.video_convert, data.video_sink,NULL)) {
        g_printerr("video Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }
    /* Set the file to play */
    g_object_set (data.source, "location", file_path, NULL);

    g_signal_connect (data.video_decoder, "pad-added", G_CALLBACK (pad_added_handler), &data);
    /* Start playing */
    ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }
    /* Listen to the bus */
    bus = gst_element_get_bus (data.pipeline);
    do {
        msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

        if (msg != NULL) {
            GError *err;
            gchar *debug_info;
            switch (GST_MESSAGE_TYPE (msg)) {
                case GST_MESSAGE_ERROR:
                    gst_message_parse_error (msg, &err, &debug_info);
                    g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                    g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
                    g_clear_error (&err);
                    g_free (debug_info);
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_EOS:
                    g_print ("End-Of-Stream reached.\n");
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_STATE_CHANGED:
                    if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data.pipeline)) {
                        GstState old_state, new_state, pending_state;
                        gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
                        g_print ("Pipeline state changed from %s to %s:\n",
                                 gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
                    }
                    break;
                default:
                    g_printerr ("Unexpected message received.\n");
                    break;
            }
            gst_message_unref (msg);
        }
    } while (!terminate);

    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);
    return 0;
}

/* This function will be called by the pad-added signal
 * 目前自动处理当分流器分流出"audio/x-vorbis"和"video/x-theora"媒体流时,自动连接新产生的源pad和对应接受pad
*/
static void pad_added_handler (GstElement *src, GstPad *new_pad, CustomData *data) {
    g_print("Inside the pad_added_handler method \n");
    GstPad *sink_pad_video = gst_element_get_static_pad (data->video_convert, "sink");

    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;
    g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

    new_pad_caps = gst_pad_get_current_caps(new_pad);
    new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
    new_pad_type = gst_structure_get_name (new_pad_struct);


    if (g_str_has_prefix (new_pad_type,"video/x-raw" /*"audio/mpeg"*/))
    {
        ret = gst_pad_link (new_pad, sink_pad_video);
        if (GST_PAD_LINK_FAILED (ret))
        {
            g_print (" Type is '%s' but link failed.\n", new_pad_type);
        }
        else
        {
            g_print (" Link succeeded (type '%s').\n", new_pad_type);
        }
    }
    else {
        g_print (" It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
        goto exit;
    }
    exit:
    if (new_pad_caps != NULL)
        gst_caps_unref (new_pad_caps);
    gst_object_unref (sink_pad_video);
}

static void
cb_need_data (GstElement    *appsrc,
              guint         unused_size,
              gpointer      user_data)
{
//    std::cout<<*(int*)user_data<<std::endl;
    static gboolean white = FALSE;
    static GstClockTime timestamp = 0;
    static int n=0;
    GstBuffer *buffer;
//    GstMapInfo info;
    guint size;
    string img_path;
    if(n%2==0)
        img_path = dir_path+"a.jpg";
    else
        img_path = dir_path+"crown.jpg";
    Mat img=imread(img_path), disimg;
    resize(img,disimg,Size(g_width, g_height));
    GstFlowReturn ret;

    size = g_width*g_height*3;
    // 为GstBuffer申请内存, 用于与Mat对象交互数据
    buffer = gst_buffer_new_allocate (NULL, size, NULL);
//    gst_buffer_map(buffer, &info, GST_MAP_WRITE);
    // 将一片Mat内存区域的数据写入GstBuffer
    gst_buffer_fill(buffer,0,(gpointer*)disimg.data,size);


    if(buffer==NULL)
    {
        cout <<"buffer allocate failed"<<endl;
    }
    GST_BUFFER_PTS (buffer) = timestamp;
    GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 2);

    timestamp += GST_BUFFER_DURATION (buffer);
    // 将buffer 缓冲的数据推入appsrc元件的src pad缓冲区中
    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);
    gst_buffer_unref (buffer);

    if (ret != GST_FLOW_OK) {
        /* something wrong, stop pushing */
        g_main_loop_quit (loop);
    }
    std::cout<<n++<<std::endl;
}

int 
filter_play(int argc, char *argv[], guintptr handle)
{
    GstElement *pipeline, *appsrc, *conv, *videosink;

    /* init GStreamer */
    gst_init (&argc, &argv);
    loop = g_main_loop_new (NULL, FALSE);

    /* setup pipeline */
    pipeline = gst_pipeline_new ("pipeline");
    appsrc = gst_element_factory_make ("appsrc", "source");
    conv = gst_element_factory_make ("videoconvert", "conv");
    videosink = gst_element_factory_make ("xvimagesink", "videosink");
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(videosink), handle);
    /* setup */
    g_object_set (G_OBJECT (appsrc), "caps",
                  gst_caps_new_simple ("video/x-raw",
                                       "format", G_TYPE_STRING, "BGR",
                                       "width", G_TYPE_INT, g_width,
                                       "height", G_TYPE_INT, g_height,
                                       "framerate", GST_TYPE_FRACTION, 0, 1,
                                       NULL), NULL);
    gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, videosink, NULL);
    gst_element_link_many (appsrc, conv, videosink, NULL);

    /* setup appsrc */
    g_object_set (G_OBJECT (appsrc),
                  "stream-type", 0,
                  "format", GST_FORMAT_TIME, NULL);
    int _data=10;
    g_signal_connect (appsrc, "need-data", G_CALLBACK (cb_need_data), (gpointer)&_data);

    /* play */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);

    /* clean up */
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
    g_main_loop_unref (loop);
    return 0;
}
