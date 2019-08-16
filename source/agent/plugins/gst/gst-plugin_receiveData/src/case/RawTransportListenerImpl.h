#ifndef RAWTRANSPORTLISTENERIMPL_H
#define RAWTRANSPORTLISTENERIMPL_H

#include <boost/thread/mutex.hpp>
#include <queue>
#include <iostream>

#include "RawTransport.h"

using namespace std;
using namespace owt_base;

typedef struct {
        boost::shared_array<char> buffer;
        int length;
} TransportData;

class RawTransportListenerImpl:public RawTransportListener{
    public:
        void onTransportData(char* buf,int len);
        void onTransportError(){printf("transport error\n");}
        void onTransportConnected(){printf("transport connected\n");}
        // ~RawTransportListenerImpl();

    	static std::queue<TransportData> m_receiveQueue;
        static boost::mutex m_receiveQueueMutex;
};

std::queue<TransportData> RawTransportListenerImpl::m_receiveQueue;

boost::mutex RawTransportListenerImpl::m_receiveQueueMutex;

void RawTransportListenerImpl::onTransportData(char* buf,int len){
    TransportData m_receiveData;
    // m_receiveData.buffer.reset(buf);
    m_receiveData.buffer.reset(new char[len]);
    memcpy(m_receiveData.buffer.get(),buf,len);
    m_receiveData.length = len;
    cout<<"m_receiveData.length="<<m_receiveData.length<<endl;
    boost::lock_guard<boost::mutex>lock(m_receiveQueueMutex);
    m_receiveQueue.push(m_receiveData);
}

#endif //
