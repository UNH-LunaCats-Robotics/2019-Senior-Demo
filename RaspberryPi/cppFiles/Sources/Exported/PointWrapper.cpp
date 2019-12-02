#include "../../Headers/Exported/PointWrapper.h"

Napi::FunctionReference PointWrapper::constructor;

Napi::Object PointWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Point", {
        InstanceMethod("X", &PointWrapper::X),
        InstanceMethod("Y", &PointWrapper::Y),
        InstanceMethod("Z", &PointWrapper::Z)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Point", func);
    return exports;
}

PointWrapper::PointWrapper(const Napi::CallbackInfo& info):Napi::ObjectWrap<PointWrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();
    if(length != 0 && length != 3) {
        Napi::TypeError::New(env, "Wrong Number of Arguments").ThrowAsJavaScriptException();
    }

    if(length == 3 && (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber())) {
        Napi::TypeError::New(env, "Wrong Type of Arguments").ThrowAsJavaScriptException();
    }

    this->actualClass_ = new Point();
    if(length == 3) {
        Napi::Number x = info[0].As<Napi::Number>();
        Napi::Number y = info[1].As<Napi::Number>();
        Napi::Number z = info[2].As<Napi::Number>();

        actualClass_->set(
            x.FloatValue(),
            y.FloatValue(),
            z.FloatValue()
        );
    }
}

Napi::Value PointWrapper::X(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env(); Napi::HandleScope scope(env);
    float x = this->actualClass_->X();
    return Napi::Number::New(env, x);
}

Napi::Value PointWrapper::Y(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env(); Napi::HandleScope scope(env);
    float y = this->actualClass_->Y();
    return Napi::Number::New(env, y);
}

Napi::Value PointWrapper::Z(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env(); Napi::HandleScope scope(env);
    float z = this->actualClass_->Z();
    return Napi::Number::New(env, z);
}