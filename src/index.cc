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

FunctionReference callback;

Object createEvent(Env &env, uiohook_event *event) {
  Object evt = Object::New(env);
  Object data = Object::New(env);
  evt.Set("data", data);

  switch (event->type) {
    case EVENT_HOOK_ENABLED:
      evt.Set("type", "enabled");
      break;
    case EVENT_HOOK_DISABLED:
      evt.Set("type", "disabled");
      break;
    case EVENT_KEY_PRESSED:
      evt.Set("type", "keydown");
      break;
    case EVENT_KEY_RELEASED:
      evt.Set("type", "keyup");
      break;
    case EVENT_KEY_TYPED:
      evt.Set("type", "keypress");
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

  vector<int> hookEvents = { EVENT_HOOK_ENABLED, EVENT_HOOK_DISABLED };
  vector<int> keyboardEvents = { EVENT_KEY_PRESSED, EVENT_KEY_RELEASED, EVENT_KEY_TYPED };
  vector<int> mouseEvents = {
    EVENT_MOUSE_PRESSED,
    EVENT_MOUSE_RELEASED,
    EVENT_MOUSE_CLICKED,
    EVENT_MOUSE_MOVED,
    EVENT_MOUSE_DRAGGED,
  };
  vector<int> mousewheelEvents = {
    EVENT_MOUSE_WHEEL
  };

  if (find(keyboardEvents.begin(), keyboardEvents.end(), event->type) != keyboardEvents.end()) {
    auto eventData = event->data.keyboard;
    data.Set("keycode", eventData.keycode);
    data.Set("rawcode", eventData.rawcode);
  }
  if (find(mouseEvents.begin(), mouseEvents.end(), event->type) != mouseEvents.end()) {
    auto eventData = event->data.mouse;
    data.Set("button", eventData.button);
    data.Set("clicks", eventData.clicks);
    data.Set("x", eventData.x);
    data.Set("y", eventData.y);
  }
  if (find(mousewheelEvents.begin(), mousewheelEvents.end(), event->type) != mousewheelEvents.end()) {
    auto eventData = event->data.wheel;
    data.Set("clicks", eventData.clicks);
    data.Set("x", eventData.x);
    data.Set("y", eventData.y);
    data.Set("type", eventData.type);
    data.Set("amount", eventData.amount);
    data.Set("rotation", eventData.rotation);
    data.Set("direction", eventData.direction);
  }
  return evt;
}

void dispatch_proc(uiohook_event * const event) {
  Env env = callback.Env();
  Object evt = createEvent(env, event);
  callback.Call({ evt });
}

int hook_enable() {
  hook_run();
}

void Start(const CallbackInfo &info) {
  Function fn = info[0].As<Function>();
  callback = Persistent(fn);
  hook_set_dispatch_proc(dispatch_proc);
  hook_enable();
}

Object Init(Env env, Object exports) {
  exports.Set(
    String::New(env, "start"),
    Function::New(env, Start)
  );
  
  return exports;
}

NODE_API_MODULE(iohook, Init)
