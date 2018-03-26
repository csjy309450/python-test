//
// Created by yz on 18-3-21.
//

#ifndef PROJECT_MP3_PLAYER_H
#define PROJECT_MP3_PLAYER_H

#include <gst/gst.h>
#include <unistd.h>

//定义消息处理函数,
static gboolean bus_call(GstBus *bus,GstMessage *msg,gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;//这个是主循环的指针，在接受EOS消息时退出循环
    switch (GST_MESSAGE_TYPE(msg))
    {
        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            g_main_loop_quit(loop);
            break;
        case GST_MESSAGE_ERROR:
        {
            gchar *debug;
            GError *error;

            gst_message_parse_error(msg,&error,&debug);
            g_free(debug);
            g_printerr("ERROR:%s\n",error->message);
            g_error_free(error);
            g_main_loop_quit(loop);
            break;
        }
        default:
            break;
    }
    return TRUE;
}

int run_music_app(int argc,char *argv[])
{
    GMainLoop *loop;
    GstElement *pipeline,*source,*decoder,*sink;//定义组件
    GstBus *bus;

    gst_init(&argc,&argv);
    loop = g_main_loop_new(NULL,FALSE);//创建主循环，在执行 g_main_loop_run后正式开始循环

    if(argc != 2)
    {
        g_printerr("Usage:%s <mp3 filename>\n",argv[0]);
        return -1;
    }
    //创建管道和组件
    pipeline = gst_pipeline_new("audio-player");
    source = gst_element_factory_make("filesrc","file-source");
    decoder = gst_element_factory_make("mad","mad-decoder");
    sink = gst_element_factory_make("autoaudiosink","audio-output");

    if(!pipeline||!source||!decoder||!sink){
        g_printerr("One element could not be created.Exiting.\n");
        return -1;
    }
    //设置 source的location 参数。即 文件地址.
    g_object_set(G_OBJECT(source),"location",argv[1],NULL);
    //得到 管道的消息总线
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    //添加消息监视器
    gst_bus_add_watch(bus,bus_call,loop);
    gst_object_unref(bus);
    //把组件添加到管道中.管道是一个特殊的组件，可以更好的让数据流动
    gst_bin_add_many(GST_BIN(pipeline),source,decoder,sink,NULL);
    //依次连接组件
    gst_element_link_many(source,decoder,sink,NULL);
    //开始播放
    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    g_print("Running\n");
    //开始循环
    g_main_loop_run(loop);
    g_print("Returned,stopping playback\n");
    gst_element_set_state(pipeline,GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}

void playbin2_test(int argc,char *argv[]){
    GstElement *playbin,*fakesink,*mfw_v4lsink;
    GMainLoop *loop;
    gst_init(&argc,&argv);
    loop=g_main_loop_new(NULL,FALSE);
//    mfw_v4lsink=gst_element_factory_make("mfw_v4lsink","mfw_v4lsink");//我用的是imx6 所有显示插件用mfw_v4lsink，也可以不用mfw_v4lsink，不选的话默认我auto模式
    //pipeline=gst_pipeline_new("pipeline");
    gchar * str = g_new(gchar,100);
    playbin=gst_element_factory_make("playbin","playbin");
    g_object_set(G_OBJECT(playbin),"uri","file:////home/yz/testData/hand.mp4",NULL);
    g_object_get(G_OBJECT(playbin),"uri",&str,NULL);
    g_print("%s",str);
//    g_object_set(playbin,"video-sink",mfw_v4lsink,NULL);
//    gst_element_set_state(playbin,GST_STATE_PAUSED);
//    sleep(2);
    gst_element_set_state(playbin,GST_STATE_PLAYING);
    g_main_loop_run(loop);
}

void myfilter_test(int argc,char *argv[]){
    GstElement *pipeline,*filesink0,*filesink1,*filesrc,*myfilter;
    GMainLoop *loop;
    gst_init(&argc,&argv);
    loop=g_main_loop_new(NULL,FALSE);
    pipeline = gst_pipeline_new("_pipeline");
    filesrc = gst_element_factory_make("filesrc","_filesrc");
    myfilter = gst_element_factory_make("myfilter","_myfilter");
    filesink0 = gst_element_factory_make("filesink","_filesink0");
    filesink1 = gst_element_factory_make("filesink","_filesink1");
    g_object_set(G_OBJECT(filesrc),"location","./test.txt",NULL);
    g_object_set(G_OBJECT(myfilter),"silent",TRUE,NULL);
    g_object_set(G_OBJECT(filesink0),"location","./ret0.txt",NULL);
    g_object_set(G_OBJECT(filesink1),"location","./ret1.txt",NULL);
//    g_object_set(playbin,"video-sink",mfw_v4lsink,NULL);
//    gst_element_set_state(playbin,GST_STATE_PAUSED);
//    sleep(2);
    if(!pipeline||!filesrc||!myfilter||!filesink0||!filesink1){
        return;
    }
    gst_bin_add_many(GST_BIN(pipeline),filesrc,myfilter,filesink0,/*filesink1,*/NULL);
    //依次连接组件
    gst_element_link_many(filesrc,myfilter,filesink0,NULL);
//    gst_element_link_many(myfilter,filesink1,NULL);
    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    g_main_loop_run(loop);
}

void camerasrc_test(int argc,char *argv[]){
    GstElement *pipeline,*camerasrc,*filesink;
    GMainLoop *loop;
    gst_init(&argc,&argv);
    loop=g_main_loop_new(NULL,FALSE);
    pipeline = gst_pipeline_new("_pipeline");
    camerasrc = gst_element_factory_make("camerasrc","_camerasrc");
    filesink = gst_element_factory_make("filesink","_filesink");
    g_object_set(G_OBJECT(filesink),"location","./camerasrc_ret.txt",NULL);
//    g_object_set (G_OBJECT (camerasrc), "caps",
//                  gst_caps_new_simple ("video/x-raw",
//                                       "format", G_TYPE_STRING, "GRAY8"/*"BGR"*/,
//                                       "width", G_TYPE_INT, 10,
//                                       "height", G_TYPE_INT, 10,
//                                       "framerate", GST_TYPE_FRACTION, 1, 1,
//                                       NULL), NULL);
//    g_object_set(playbin,"video-sink",mfw_v4lsink,NULL);
//    gst_element_set_state(playbin,GST_STATE_PAUSED);
//    sleep(2);
    if(!pipeline||!camerasrc||!filesink){
        return;
    }
    gst_bin_add_many(GST_BIN(pipeline),camerasrc,filesink,NULL);
    //依次连接组件
    gst_element_link_many(camerasrc,filesink,NULL);
//    gst_element_link_many(myfilter,filesink1,NULL);
    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    g_main_loop_run(loop);
}

#endif //PROJECT_MP3_PLAYER_H
