#include "listInterfaces.h"

#include "unix.cpp"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "listInterfaces"),
              Napi::Function::New(env, ListInterfaces));
  return exports;
}

NODE_API_MODULE(listInterfaces, Init)
