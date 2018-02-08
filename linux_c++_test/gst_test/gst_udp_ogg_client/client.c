
#include <string.h>
#include <math.h>

#include <gst/gst.h>

#define AUDIO_CAPS "application/x-rtp,media=(string)audio, clock-rate=(int)44100, width=16, height=16,encoding-name=(string)L16, encoding-params=(string)1, channels=(int)1, channel-positions=(int)1, payload=(int)96"
//"application/x-rtp,media=(string)audio,clock-rate=(int)8000,encoding-name=(string)PCMA"

static void
print_source_stats (GObject * source)
{
  GstStructure *stats;
  gchar *str;

  g_return_if_fail (source != NULL);

  /* get the source stats */
  g_object_get (source, "stats", &stats, NULL);

  /* simply dump the stats structure */
  str = gst_structure_to_string (stats);
  g_print ("source stats: %s\n", str);

  gst_structure_free (stats);
  g_free (str);
}
static void
on_ssrc_active_cb (GstElement * rtpbin, guint sessid, guint ssrc,
    GstElement * depay)
{
  GObject *session, *isrc, *osrc;

  g_print ("got RTCP from session %u, SSRC %u\n", sessid, ssrc);

  /* get the right session */
  g_signal_emit_by_name (rtpbin, "get-internal-session", sessid, &session);

  /* get the internal source (the SSRC allocated to us, the receiver */
  g_object_get (session, "internal-source", &isrc, NULL);
  print_source_stats (isrc);

  /* get the remote source that sent us RTCP */
  g_signal_emit_by_name (session, "get-source-by-ssrc", ssrc, &osrc);
  print_source_stats (osrc);
}
static void
pad_added_cb (GstElement * rtpbin, GstPad * new_pad, GstElement * depay)
{
  GstPad *sinkpad;
  GstPadLinkReturn lres;

  g_print ("new payload on pad: %s\n", GST_PAD_NAME (new_pad));

  sinkpad = gst_element_get_static_pad (depay, "sink");
  g_assert (sinkpad);

  lres = gst_pad_link (new_pad, sinkpad);
  g_assert (lres == GST_PAD_LINK_OK);
  gst_object_unref (sinkpad);
}


int
main (int argc,char *argv[])
{
	GstElement *rtpbin,*rtpsrc,*buffer,*rtppay,*audioconver, *audiosink;
	GstElement *pipeline;
	GMainLoop *loop;
	GstCaps *caps;
	GstPadLinkReturn lres;
	GstPad *srcpad,*sinkpad;

	gst_init(&argc,&argv);

	pipeline=gst_pipeline_new(NULL);
	g_assert (pipeline);
	rtpsrc=gst_element_factory_make("udpsrc","rtpsrc");
	g_assert (rtpsrc);
	g_object_set (rtpsrc,"uri","udp://127.0.0.1:5000",NULL);
	caps=gst_caps_from_string(AUDIO_CAPS);
	g_object_set(rtpsrc,"caps",caps,NULL);
	gst_caps_unref(caps);

	gst_bin_add_many(GST_BIN (pipeline),rtpsrc,NULL);
	rtppay=gst_element_factory_make("rtpL16depay","rtppay");
	g_assert (rtppay);
	audioconver=gst_element_factory_make("audioconvert","audioconver");
	g_assert (audioconver);

	audiosink=gst_element_factory_make("alsasink","audiosink");
	g_assert (audiosink);


	gst_bin_add_many (GST_BIN(pipeline),rtppay,audioconver,audiosink,NULL);

	gboolean res=gst_element_link_many(rtppay,audioconver,audiosink,NULL);
	g_assert(res==TRUE);
	g_object_set (audiosink, "sync", FALSE, NULL);

	rtpbin=gst_element_factory_make("gstrtpbin","rtpbin");
	g_assert(rtpbin);
	gst_bin_add(GST_BIN(pipeline),rtpbin);
	srcpad = gst_element_get_static_pad (rtpsrc, "src");
  	sinkpad = gst_element_get_request_pad (rtpbin, "recv_rtp_sink_0");
  	lres = gst_pad_link (srcpad, sinkpad);
  	g_assert (lres == GST_PAD_LINK_OK);
  	gst_object_unref (srcpad);

	g_signal_connect (rtpbin, "pad-added", G_CALLBACK (pad_added_cb), rtppay);

  /* give some stats when we receive RTCP */
  //	g_signal_connect (rtpbin, "on-ssrc-active", G_CALLBACK (on_ssrc_active_cb),
 //     rtppay);


	g_print ("starting receiver pipeline\n");
        gst_element_set_state (pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new (NULL, FALSE);
  	g_main_loop_run (loop);

	g_print ("stopping receiver pipeline\n");
  	gst_element_set_state (pipeline, GST_STATE_NULL);

  	gst_object_unref (pipeline);

  	return 0;
}
