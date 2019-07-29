//
// Created by xyk on 19-7-9.
//

#include <zconf.h>
#include "VideoGstAnalyzer.h"
#include <iostream>


namespace mcu {

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

    if (!pipeline || !source || !sink) {
        printf("pipeline or source or sink elements could not be created.\n");
        return -1;
    }

    return 0;
};

int VideoGstAnalyzer::play() {
    printf("pipeline play");
    gst_bin_add_many(GST_BIN (pipeline), source, receive, send, sink, NULL);

    if (gst_element_link_many(source, receive, send, sink, NULL) != TRUE) {
        printf("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        printf("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}

void VideoGstAnalyzer::emit_ListenTo(int minPort, int maxPort) {
	printf("min,max=");
    g_signal_emit_by_name(receive, "notifyListenTo", minPort, maxPort, &this->listeningPort);
    printf("signal ret port = %d\n", listeningPort);
}

void VideoGstAnalyzer::emit_ConnectTo(int remotePort){
    g_signal_emit_by_name(send,"notifyConnectTo",remotePort);
}

int VideoGstAnalyzer::getListeningPort() { return 54443; }

}
