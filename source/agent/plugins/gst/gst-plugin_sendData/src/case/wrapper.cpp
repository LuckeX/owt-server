#include "wrapper.h"
// #include "Server.h"
#include "RawTransport.h"
#include "RawTransportListenerImpl.h"
#include <iostream>
#include <queue>


using namespace std;

using namespace owt_base;

void* call_instance(int initBuf,int flag){
    bool tag;
    if(flag)tag = true;
    else tag = false;

    return new RawTransport<TCP>(new RawTransportListenerImpl(),initBuf,tag);
}

// void call_listenTo(void* instance,int minPort,int maxPort){
//     RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
//     rt->listenTo(minPort,maxPort);
// }

// unsigned short call_getListeningPort(void* instance){
//     RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
//     return rt->getListeningPort();
// }

// char *call_getBuffer(int *len){
//     if(!RawTransport<TCP>::m_receiveQueue.empty()){
//         boost::lock_guard<boost::mutex>lock(RawTransport<TCP>::m_receiveQueueMutex);

//         *len = RawTransport<TCP>::m_receiveQueue.front().length;
//         char *p = new char[*len];
//         memcpy(p,RawTransport<TCP>::m_receiveQueue.front().buffer.get(),*len);//copy

//         RawTransport<TCP>::m_receiveQueue.pop();
//         return p;
//     }
//     return NULL;
// }

void call_connect(void* instance,char* ip,int port){
    RawTransport<TCP>* rt = static_cast<RawTransport<TCP>*>(instance);
    rt->createConnection(ip,port);
}

void call_sendData(void* instance, char *buf, int len){
	printf("send data wrapper: call_senddata\n");
    RawTransport<TCP> *sc = static_cast<RawTransport<TCP>*>(instance);
    sc->sendData(buf,len);
}

