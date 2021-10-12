#include <napi.h>
#include "uiohook.h"

struct Event {
    Napi::String type;
    Event(Napi::Env &env, uiohook_event *event);
};
