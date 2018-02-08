#include <gst/gst.h>
 
static GMainLoop *loop;
 
static void
cb_need_data (GstElement *appsrc,
          guint       unused_size)
{
     
  static gboolean white = FALSE;
  static GstClockTime timestamp = 0;
  GstBuffer *buffer;
  guint size;
  GstFlowReturn ret;
 
  size = 1920 * 1080 * 3;
 
  buffer = gst_buffer_new_allocate (NULL, size, NULL);
  if(buffer==NULL)
  {
      cout <<"buffer allocate failed"<<endl;
  }
#if 0 //display black and white blocks 
  /* this makes the image black/white */
  gst_buffer_memset (buffer, 0, white ? 0xff : 0x0, size);
  white = !white;   
#else//display frame plus my overlay
  //Mat matTemp = Mat(Size(width, height),CV_8U, (char*)GST_BUFFER_DATA(buffer));
  //overlay.copyTo(matTemp);  
   //gst_buffer_fill (buffer,0,&(matTemp.data),size);// I want to put Mat data to gstBuffer, but not successful 
#endif
//std::cout<<"hh"<<temp.rows<<endl;
  GST_BUFFER_PTS (buffer) = timestamp;
  GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 2);
 
  timestamp += GST_BUFFER_DURATION (buffer);
 
  g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);
  gst_buffer_unref (buffer);
 
  if (ret != GST_FLOW_OK) {
    /* something wrong, stop pushing */
    g_main_loop_quit (loop);
  }
}
 
gint
main (gint   argc,
      gchar *argv[])
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
 
  /* setup */
  g_object_set (G_OBJECT (appsrc), "caps",
          gst_caps_new_simple ("video/x-raw",
                     "format", G_TYPE_STRING, "RGB16",
                     "width", G_TYPE_INT, 1920,
                     "height", G_TYPE_INT, 1080,
                     "framerate", GST_TYPE_FRACTION, 0, 1,
                     NULL), NULL);
  gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, videosink, NULL);
  gst_element_link_many (appsrc, conv, videosink, NULL);
 
  /* setup appsrc */
  g_object_set (G_OBJECT (appsrc),
        "stream-type", 0,
        "format", GST_FORMAT_TIME, NULL);
  g_signal_connect (appsrc, "need-data", G_CALLBACK (cb_need_data), NULL);
 
  /* play */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  g_main_loop_run (loop);
 
  /* clean up */
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline));
  g_main_loop_unref (loop);
 
  return 0;
}
