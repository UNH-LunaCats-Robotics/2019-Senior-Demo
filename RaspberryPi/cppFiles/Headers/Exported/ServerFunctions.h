#include <napi.h>
#include "./PointWrapper.h"
#include "../Point.h"
namespace serverFunctions {
    //currently we don't have any logic to do this,
    //but these are sample function examples

    //[var] function([param]);
    std::string helloWorld();
    int add(int a, int b);
    Point getPoint();

    //[JS equivalent] FunctionWrap(const Napi::CallbackInfo& info );
    Napi::String HelloWrapped(const Napi::CallbackInfo& info);
    Napi::Number AddWrapped(const Napi::CallbackInfo& info);
    Napi::String GetPointWrapped(const Napi::CallbackInfo& info);

    //returns functions
    Napi::Object Init(Napi::Env env, Napi::Object exports);
}