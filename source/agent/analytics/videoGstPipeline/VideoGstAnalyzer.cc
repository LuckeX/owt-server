//
// Created by xyk on 19-7-9.
//

#include <zconf.h>
#include "VideoGstAnalyzer.h"
#include <iostream>
#include <string>


namespace mcu {

GMainLoop* VideoGstAnalyzer::loop = g_main_loop_new(NULL,FALSE);

VideoGstAnalyzer::VideoGstAnalyzer() {
    printf("video gst Analyzer constructor");
}

VideoGstAnalyzer::~VideoGstAnalyzer() {
    printf("video gst Analyzer deconstructor");
}

int VideoGstAnalyzer::createPipeline() {
    /* Initialize GStreamer */
    gst_init(NULL, NULL);
    printf("createPipeline");

    /* Create the elements */
    source = gst_element_factory_make("fakesrc", "source");
    receive = gst_element_factory_make("receivedata", "receive");
    decode = gst_element_factory_make("decodebin", "decode");
    analyzer = gst_element_factory_make("facerecognition","analyzer");
    encode = gst_element_factory_make("encodebin", "encode"); 
    send = gst_element_factory_make("senddata","send");
    sink = gst_element_factory_make("fakesink", "sink");

    loop = g_main_loop_new(NULL, FALSE);

    /* Create the empty VideoGstAnalyzer */
    pipeline = gst_pipeline_new("test-pipeline");

    if (!receive) {
        printf("receive element coule not be created\n");
        return -1;
    }

    if (!send) {
        printf("send element coule not be created\n");
        return -1;
    }

    if (!analyzer){
        printf("analyzer element coule not be created\n");
        return -1;
    }

    if (!pipeline || !source || !sink) {
        printf("pipeline or source or sink elements could not be created.\n");
        return -1;
    }

    return 0;
};

int VideoGstAnalyzer::addElementMany() {
    printf("add elements source,receive,");
    gst_bin_add_many(GST_BIN (pipeline), source, receive, sink, NULL);
    if (gst_element_link_many(source, receive, sink, NULL) != TRUE) {
        g_printerr("Elements source, receive, sink could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    return 0;
}

int VideoGstAnalyzer::addElement() {
    gst_element_set_state(pipeline,GST_STATE_READY);
    gst_element_unlink(receive,sink);
    gst_bin_add(GST_BIN(pipeline),send);
    if(gst_element_link_many(receive,send,sink,NULL) != TRUE){
        g_printerr("Elements receive,send,sink could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    return 0;
}

void VideoGstAnalyzer::stopLoop(){
    if(loop){
        printf("main loop quit\n");
        g_main_loop_quit(loop);
    }
    g_thread_join(m_thread);
}

void VideoGstAnalyzer::main_loop_thread(gpointer data){
    g_main_loop_run(loop);
    printf("first thread end\n");
    g_thread_exit(0);
}

int VideoGstAnalyzer::setPlaying() {
    printf("pipeline play");
    // gst_bin_add_many(GST_BIN (pipeline), source, receive, analyzer, send, sink, NULL);

    // if (gst_element_link_many(source, receive, analyzer, send, sink, NULL) != TRUE) {
    //     printf("Elements could not be linked.\n");
    //     gst_object_unref(pipeline);
    //     return -1;
    // }

    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        printf("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    m_thread = g_thread_create((GThreadFunc)main_loop_thread,NULL,TRUE,NULL);

    return 0;
}

void VideoGstAnalyzer::emit_ListenTo(int minPort, int maxPort) {
	printf("min,max=");
    pthread_t tid;
    tid = pthread_self();
    printf("VideoGstAnalyzer::emit_ListenTo in tid %u (0x%x)\n", (unsigned int)tid, (unsigned int)tid);
    g_signal_emit_by_name(receive, "notifyListenTo", minPort, maxPort, &this->listeningPort);
    printf("signal ret port = %d\n", listeningPort);
}

void VideoGstAnalyzer::emit_ConnectTo(int connectionID, int remotePort){
    printf("connect to remotePort:%d, connectionID:%d\n",remotePort, connectionID);
    g_signal_emit_by_name(send,"notifyConnectTo",connectionID, remotePort);
}

int VideoGstAnalyzer::getListeningPort() { return listeningPort; }

void VideoGstAnalyzer::setOutputParam(std::string codec, int width, int height, 
    int framerate, int bitrate, int kfi, std::string algo, std::string pluginName){

    this->codec = codec;
    this->width = width;
    this->height = height;
    this->framerate = framerate;
    this->bitrate = bitrate;
    this->kfi = kfi;
    this->algo = algo;
    this->pluginName = pluginName;

    std::cout<<"setting param,codec="<<this->codec<<",width="<<this->width<<",height="
        <<this->height<<",framerate="<<this->framerate<<",bitrate="<<this->bitrate<<",kfi="
        <<this->kfi<<",algo="<<this->algo<<",pluginName="<<this->pluginName<<std::endl;
}

}
