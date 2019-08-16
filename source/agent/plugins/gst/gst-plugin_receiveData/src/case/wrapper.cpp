//
// Created by webrtc on 19-7-22.
//

#include "wrapper.h"

#include "RawTransport.h"
#include "RawTransportListenerImpl.h"
#include <iostream>
#include <queue>
#include <memory>


using namespace std;
using namespace owt_base;

void* call_instance(int initBuf,int flag){
    bool tag;
    if(flag)tag = true;
    else tag = false;

    return new RawTransport<TCP>(new RawTransportListenerImpl(),initBuf,tag);
}

void call_listenTo(void* instance,int minPort,int maxPort){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    rt->listenTo(minPort,maxPort);
}

unsigned short call_getListeningPort(void* instance){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    return rt->getListeningPort();
}

char *call_getBuffer(int *len){
    if(!RawTransportListenerImpl::m_receiveQueue.empty()){
        boost::lock_guard<boost::mutex>lock(RawTransportListenerImpl::m_receiveQueueMutex);
        *len = RawTransportListenerImpl::m_receiveQueue.front().length;
        char *p = new char[*len];
        memcpy(p,RawTransportListenerImpl::m_receiveQueue.front().buffer.get(),*len);//copy
        RawTransportListenerImpl::m_receiveQueue.pop();
        return p;
    }
    return NULL;
}

void call_connect(void* instance,char* ip,int port){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    rt->createConnection(ip,port);
}