#ifndef VideoGstAnalyzerWRAPPER_H
#define VideoGstAnalyzerWRAPPER_H

#include "VideoGstAnalyzer.h"
#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>

class VideoGstAnalyzer: public node::ObjectWrap{
  public:
  static void Init(v8::Handle<v8::Object>, v8::Handle<v8::Object>);
  mcu::VideoGstAnalyzer* me;

 private:
  VideoGstAnalyzer();
  ~VideoGstAnalyzer();
  static v8::Persistent<v8::Function> constructor;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void close(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getListeningPort(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void createPipeline(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void emit_ListenTo(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void emit_ConnectTo(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void play(const v8::FunctionCallbackInfo<v8::Value>& args);
};


#endif
