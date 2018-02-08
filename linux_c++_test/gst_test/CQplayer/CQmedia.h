#ifndef CQMEDIA_H
#define CQMEDIA_H

#include <string>
#include <iostream>
#include <gstreamer-1.0/gst/gst.h>
#include <CQplayerGUI.h>
#include <gst/video/videooverlay.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class CQplayerGUI;

enum PlayingState
{
    PLAYING,
    STOPPED,
    PAUSED,
};

class CQMedia
{
public:
    CQMedia(CQplayerGUI* player,int windowID=0);
    ~CQMedia();
    bool setFilePath(const std::string& URI);
    bool play();
    bool pause();
    bool stop();
    bool forward();
    bool backward();
    int getPlayingState();
    int getPlayingSpeed();
    void setVolume(double volume);
    double getVolume();
    void timeUpdate();
    void seekStart();
    void seekStop();
    void seek(int position);
public:
    GstElement* pipeline;
private:
    GMainLoop* loop;
    PlayingState state;
    int speed;
    int winID;
    double volume;
    std::string URI;
    CQplayerGUI* player;
    gint64 length, position;
    bool seeking;
};

void cb_need_data(GstElement *appsrc,guint unused_size, gpointer user_data);

#endif // CQMEDIA_H
