#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Napi::Array ListInterfaces(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Array out = Napi::Array::New(env);

  struct ifaddrs *interfaces;
  int index = 0;
  if (getifaddrs(&interfaces) != 0) return out;

  for (struct ifaddrs *ifa=interfaces; ifa; ifa = ifa->ifa_next) {
    if (
      !ifa->ifa_addr ||
      ifa->ifa_addr->sa_family != AF_INET ||
      !(ifa->ifa_flags & IFF_BROADCAST)
    ) continue;
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(
      Napi::String::New(env, "name"),
      Napi::String::New(env, ifa->ifa_name)
    );
    obj.Set(
      Napi::String::New(env, "running"),
      Napi::Boolean::New(env, ifa->ifa_flags & IFF_RUNNING)
    );
    obj.Set(
      Napi::String::New(env, "internal"),
      Napi::Boolean::New(env, ifa->ifa_flags & (IFF_LOOPBACK | IFF_POINTOPOINT))
    );
    char ip_addr_buffer[INET_ADDRSTRLEN];
    inet_ntop(
      AF_INET,
      &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr,
      ip_addr_buffer,
      sizeof(ip_addr_buffer)
    );
    obj.Set(
      Napi::String::New(env, "address"),
      Napi::String::New(env, ip_addr_buffer)
    );
    inet_ntop(
      AF_INET,
      &((struct sockaddr_in*)ifa->ifa_broadaddr)->sin_addr,
      ip_addr_buffer,
      sizeof(ip_addr_buffer)
    );
    obj.Set(
      Napi::String::New(env, "broadcast"),
      Napi::String::New(env, ip_addr_buffer)
    );
    out[index] = obj;
    index++;
  }

  freeifaddrs(interfaces);

  return out;
}
