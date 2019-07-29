//
// Created by xyk on 19-7-9.
//

#ifndef CPPGSTREAMER_VideoGstAnalyzer_H
#define CPPGSTREAMER_VideoGstAnalyzer_H

#include <gst/gst.h>
#include <glib-object.h>

namespace mcu {

class VideoGstAnalyzer{
public:
    VideoGstAnalyzer();
    virtual ~VideoGstAnalyzer();
    int createPipeline();
    int getListeningPort();
    void emit_ListenTo(int minPort,int maxPort);
    int play();

    void setConnectPort(int port){connectPort = port;}
    void emit_ConnectTo(int remotePort);

private:
    GstElement *pipeline, *source, *receive,*send,*sink;
    GMainLoop *loop;
    GstStateChangeReturn ret;

    // int minPort,maxPort;
    int listeningPort;

    int connectPort;
};

}

#endif //CPPGSTREAMER_VideoGstAnalyzer_H
