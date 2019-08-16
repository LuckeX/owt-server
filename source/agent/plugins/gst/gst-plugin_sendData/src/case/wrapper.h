#ifndef PIPELINE_WRAPPER_H
#define PIPELINE_WRAPPER_H


#ifdef __cplusplus
extern "C" {
#endif


void* call_instance(int initBuf,int flag);
void call_listento(void* instance,int minPort,int maxPort);
unsigned short call_getListeningPort(void* instance);

void *call_getBuffer();

//send data
void call_connect(void* instance, char* ip,int port);
void call_sendData(void* instance, char *buf, int len);

void call_start(void* instance);

// void* call_instance();
// void call_connect(void* instance, char* ip,int port);
// void call_listento(void* instance,int minPort,int maxPort);
// unsigned short call_getListeningPort(void* instance);

// void *call_getBuffer();


//send data


// void call_storeData(void* instance,void *buf,int len);
// void call_doSend(void* instance);

// void call_sendData(void* instance,void* buf,int len);
// void call_sendData(void* instance);
// void call_start(void* instance);

// void call_syncWrite(void* instance,char* buf,int len);

#ifdef __cplusplus
}
#endif



#endif //PIPELINE_WRAPPER_H