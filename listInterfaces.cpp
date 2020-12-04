#include "listInterfaces.h"

#ifdef _WIN32
#include "windows.cpp"
#else
#include "unix.cpp"
#endif

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "listInterfaces"),
              Napi::Function::New(env, ListInterfaces));
  return exports;
}

NODE_API_MODULE(listInterfaces, Init)
