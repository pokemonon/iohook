// 使用线程异步处理事件 有点问题
// [Deadlock - Darwin - Keyboard Event](https://github.com/kwhat/libuiohook/issues/2)
#include <napi.h>
#include <iostream>
#include <uiohook.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <CoreFoundation/CoreFoundation.h>
#include "index.h"

using namespace std;
using namespace Napi;

Napi::ThreadSafeFunction tsEmitFn;
mutex controlMtx;
mutex runningMtx;
condition_variable cond;

Object createEvent(Env &env, uiohook_event *event) {
  Object evt = Object::New(env);
  cout << event->reserved << endl;

  switch (event->type) {
    case EVENT_HOOK_ENABLED:
      runningMtx.lock();
      evt.Set("type", "enabled");
      controlMtx.unlock();
      break;
    case EVENT_HOOK_DISABLED:
      controlMtx.lock();
      evt.Set("type", "disabled");
      runningMtx.unlock();
      break;
    case EVENT_KEY_PRESSED:
      evt.Set("type", "keydown");
      break;
    case EVENT_KEY_RELEASED:
      evt.Set("type", "keyup");
      break;
    case EVENT_KEY_TYPED:
      evt.Set("type", "keytyped");
      break;
    case EVENT_MOUSE_PRESSED:
      evt.Set("type", "mousedown");
      break;
    case EVENT_MOUSE_RELEASED:
      evt.Set("type", "mouseup");
      break;
    case EVENT_MOUSE_CLICKED:
      evt.Set("type", "click");
      break;
    case EVENT_MOUSE_MOVED:
      evt.Set("type", "mousemove");
      break;
    case EVENT_MOUSE_DRAGGED:
      evt.Set("type", "mousedragged");
      break;
    case EVENT_MOUSE_WHEEL:
      evt.Set("type", "mousewheel");
      break;
    default:
      break;
  }

  return evt;
}

void dispatch_proc(uiohook_event * const event) {
  auto callback = [event]( Napi::Env env, Napi::Function jsCallback) {
    Object evt = createEvent(env, event);

    jsCallback.Call({
      evt.Get("type").As<String>(),
      evt
    });
  };
  tsEmitFn.BlockingCall(callback);
}

bool logger_proc(unsigned int level, const char *format, ...) {
    bool status = false;
    
    va_list args;
    switch (level) {
        case LOG_LEVEL_INFO:
            va_start(args, format);
            status = vfprintf(stdout, format, args) >= 0;
            va_end(args);
            break;
        case LOG_LEVEL_WARN:
        case LOG_LEVEL_ERROR:
            va_start(args, format);
            status = vfprintf(stderr, format, args) >= 0;
            va_end(args);
            break;
    }
    
    return status;
}

void hook_thread_proc() {

  hook_run();
  // cond.notify_one();
  // controlMtx.un
}

int hook_enable() {
  // auto th = thread(hook_thread_proc);
  // th.detach();
  hook_run();
}

void Run(const Napi::CallbackInfo &info) {
  // unique_lock<mutex> uniqueControlMtx(controlMtx);

  // hook_set_logger_proc(logger_proc);
  // hook_set_dispatch_proc(dispatch_proc);
  // auto start = []() {
  //   hook_run();
  //   cond.notify_one();
  //   controlMtx.unlock();
  // };
  // auto th = thread(start);


  // // controlMtx.lock();
  // // cond.wait(unique_lock(controlMtx));
  // cond.wait(uniqueControlMtx);

  // cond.notify_one();
  // controlMtx.unlock();

  // th.detach();

  CFRunLoopRun();
  hook_set_logger_proc(logger_proc);
  hook_set_dispatch_proc(dispatch_proc);
  hook_enable();
}

void Stop(const CallbackInfo &info) {
  hook_stop();
}

void SetEmit(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Function fn = info[0].As<Napi::Function>();
  tsEmitFn = Napi::ThreadSafeFunction::New(
    env,
    fn,
    "tsEmitFn",
    0,
    1
  );
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(
    Napi::String::New(env, "run"),
    Napi::Function::New(env, Run)
  );

  exports.Set(
    Napi::String::New(env, "setEmit"),
    Napi::Function::New(env, SetEmit)
  );
  
  return exports;
}

NODE_API_MODULE(iohook, Init)
