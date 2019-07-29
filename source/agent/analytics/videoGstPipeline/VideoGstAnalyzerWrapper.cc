
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include "VideoGstAnalyzerWrapper.h"
#include <iostream>

using namespace v8;

Persistent<Function> VideoGstAnalyzer::constructor;
VideoGstAnalyzer::VideoGstAnalyzer() {};
VideoGstAnalyzer::~VideoGstAnalyzer() {};

void VideoGstAnalyzer::Init(Handle<Object> exports, Handle<Object> module) {
  Isolate* isolate = exports->GetIsolate();
  printf("Wrapper init");
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "VideoGstAnalyzer"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getListeningPort", getListeningPort);
  NODE_SET_PROTOTYPE_METHOD(tpl, "createPipeline", createPipeline);
  NODE_SET_PROTOTYPE_METHOD(tpl, "emit_ListenTo", emit_ListenTo);
  NODE_SET_PROTOTYPE_METHOD(tpl, "emit_ConnectTo", emit_ConnectTo);
  NODE_SET_PROTOTYPE_METHOD(tpl, "play", play);

  constructor.Reset(isolate, tpl->GetFunction());
  module->Set(String::NewFromUtf8(isolate, "exports"), tpl->GetFunction());
}

void VideoGstAnalyzer::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  printf("Wrapper new");
  // unsigned int minPort = 0, maxPort = 0;

  // if (args.Length() >= 2) {
  //   minPort = args[0]->Uint32Value();
  //   maxPort = args[1]->Uint32Value();
  // }

  VideoGstAnalyzer* obj = new VideoGstAnalyzer();
  obj->me = new mcu::VideoGstAnalyzer();

  obj->Wrap(args.This());
  args.GetReturnValue().Set(args.This());
}

void VideoGstAnalyzer::close(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;
  delete me;
}

void VideoGstAnalyzer::getListeningPort(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  uint32_t port = me->getListeningPort();

  args.GetReturnValue().Set(Number::New(isolate, port));
}

void VideoGstAnalyzer::createPipeline(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  me->createPipeline();
}

void VideoGstAnalyzer::emit_ListenTo(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  unsigned int minPort = 0, maxPort = 0;

    minPort = args[0]->Uint32Value();
    maxPort = args[1]->Uint32Value();

  me->emit_ListenTo(minPort,maxPort);
}

void VideoGstAnalyzer::emit_ConnectTo(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;

  unsigned int remotePort;
  remotePort = args[0]->Uint32Value();
  me->emit_ConnectTo(remotePort);
}

void VideoGstAnalyzer::play(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  VideoGstAnalyzer* obj = ObjectWrap::Unwrap<VideoGstAnalyzer>(args.Holder());
  mcu::VideoGstAnalyzer* me = obj->me;
  
  me->play();
}
