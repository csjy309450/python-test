#include <string.h>
#include <math.h>

#include <gst/gst.h>
#define DEST_HOST "127.0.0.1"
#define AUDIO_CAPS "audio/x-raw-int,channels=1,depth=16,width=16, rate=44100"

int
main (int argc,char *argv[])
{

    if(argc!=2)
    {
        printf("...\n");
        return(-1);
    }

	GstElement *source,*maddecoder,*audioconv;
	GstElement *rtpbin,*rtpsink,*rtppay;
	GstElement *pipeline;
	GMainLoop *loop;
	GstCaps *caps;
	GstPad *srcpad,*sinkpad;


	gst_init(&argc,&argv);

	pipeline = gst_pipeline_new(NULL);
	g_assert(pipeline);

	source = gst_element_factory_make("filesrc","source");
	g_assert (pipeline);
	maddecoder=gst_element_factory_make("mad","maddecoder");
	g_assert (maddecoder);
	audioconv=gst_element_factory_make("audioconvert","audioconv");
	g_assert (audioconv);
/*
	caps=gst_caps_new_simple("audio/x-raw-int",
				"channels",G_TYPE_INT,1,
				"depth",G_TYPE_INT,16,
				"width",G_TYPE_INT,15,
				"rate",GST_TYPE_LIST,44100,NULL);

*/
	rtppay=gst_element_factory_make("rtpL16pay","rtppay");
	g_assert (rtppay);
	g_object_set(G_OBJECT(source),"location",argv[1],NULL);

	gst_bin_add_many (GST_BIN (pipeline),source,maddecoder,audioconv,rtppay,NULL);

	caps=gst_caps_from_string(AUDIO_CAPS);


	if(!gst_element_link_many(source,maddecoder,audioconv,NULL)){
		g_error("Failed to link ");
	}
	if(!gst_element_link_filtered(audioconv,rtppay,caps))
        {
                 g_error("Failed to link caps");

        }
        gst_caps_unref(caps);
	rtpbin=gst_element_factory_make("gstrtpbin","rtpbin");
	g_assert(rtpbin);

	gst_bin_add(GST_BIN(pipeline),rtpbin);

	rtpsink=gst_element_factory_make ("udpsink","rtpsink");
	g_assert(rtpsink);
	g_object_set(rtpsink,"port",5000,"host","127.0.0.1",NULL);
	gst_bin_add_many(GST_BIN(pipeline),rtpsink,NULL);


	sinkpad = gst_element_get_request_pad (rtpbin, "send_rtp_sink_0");
  	srcpad = gst_element_get_static_pad (rtppay, "src");
  	if (gst_pad_link (srcpad, sinkpad) != GST_PAD_LINK_OK)
    		g_error ("Failed to link audio payloader to rtpbin");
  	gst_object_unref (srcpad);

	srcpad = gst_element_get_static_pad (rtpbin, "send_rtp_src_0");
  	sinkpad = gst_element_get_static_pad (rtpsink, "sink");
  	if (gst_pad_link (srcpad, sinkpad) != GST_PAD_LINK_OK)
    		g_error ("Failed to link rtpbin to rtpsink");
  	gst_object_unref (srcpad);
  	gst_object_unref (sinkpad);


	g_print("starting sender pipeline\n");
	gst_element_set_state(pipeline,GST_STATE_PLAYING);
	loop=g_main_loop_new(NULL,FALSE);
	g_main_loop_run(loop);
	g_print("stopping sender pipeline\n");
	gst_element_set_state(pipeline,GST_STATE_NULL);
	return 0;

}
