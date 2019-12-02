#include "../Point.h"
#include <napi.h>

#ifndef POINTWRAP
#define POINTWRAP

class PointWrapper : public Napi::ObjectWrap<PointWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    PointWrapper(const Napi::CallbackInfo &info);
private:
    static Napi::FunctionReference constructor;
    Napi::Value X(const Napi::CallbackInfo& info);
    Napi::Value Y(const Napi::CallbackInfo& info);
    Napi::Value Z(const Napi::CallbackInfo& info);

    Point *actualClass_;
};

#endif