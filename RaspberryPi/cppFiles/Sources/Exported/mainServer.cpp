#include "../../Headers/Exported/ServerFunctions.h"
#include "../../Headers/Exported/PointWrapper.h"
#include <napi.h>

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    serverFunctions::Init(env, exports);
    return PointWrapper::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)