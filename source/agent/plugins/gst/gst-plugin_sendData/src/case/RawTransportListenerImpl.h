#include "RawTransport.h"

using namespace owt_base;

class RawTransportListenerImpl:public RawTransportListener{
    public:
        void onTransportData(char* buf,int len){}
        void onTransportError(){printf("transport error\n");}
        void onTransportConnected(){}
        // ~RawTransportListenerImpl();
};

// void RawTransportListenerImpl::onTransportData(char* buf,int len){}

