#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

Napi::Array ListInterfaces(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Array out = Napi::Array::New(env);

  PIP_ADAPTER_ADDRESSES interfaces;
  unsigned int index = 0;
  unsigned int iterations = 0;
  ULONG outBufLen = WORKING_BUFFER_SIZE;
  DWORD dwRetVal = 0;

  do {
    interfaces = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
    if (interfaces == NULL) {
      printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
      return out;
    }

    dwRetVal =
      GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, interfaces, &outBufLen);

    if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
      FREE(interfaces);
      interfaces = NULL;
    } else {
      break;
    }

    iterations++;
  } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (iterations < MAX_TRIES));

  if (dwRetVal != NO_ERROR) return out;

  for (PIP_ADAPTER_ADDRESSES ifa=interfaces; ifa; ifa = ifa->Next) {
    if (
      !(ifa->Flags & IP_ADAPTER_IPV4_ENABLED) ||
      !ifa->FirstPrefix ||
      !ifa->FirstPrefix->Next ||
      !ifa->FirstPrefix->Next->Next ||
      !ifa->FirstUnicastAddress
    ) continue;
    Napi::Object obj = Napi::Object::New(env);
    std::wstring wname(ifa->FriendlyName);
    std::u16string name(wname.begin(), wname.end());
    obj.Set(
      Napi::String::New(env, "name"),
      Napi::String::New(env, name)
    );
    obj.Set(
      Napi::String::New(env, "running"),
      Napi::Boolean::New(env, ifa->OperStatus == IfOperStatusUp)
    );
    obj.Set(
      Napi::String::New(env, "internal"),
      Napi::Boolean::New(env,
        ifa->IfType != IF_TYPE_IEEE80211 &&
        ifa->IfType != IF_TYPE_ETHERNET_CSMACD &&
        ifa->IfType != IF_TYPE_TUNNEL)
    );
    char ip_addr_buffer[INET_ADDRSTRLEN];
    getnameinfo(
      ifa->FirstUnicastAddress->Address.lpSockaddr,
      ifa->FirstUnicastAddress->Address.iSockaddrLength,
      ip_addr_buffer,
      sizeof(ip_addr_buffer),
      NULL,
      0,
      NI_NUMERICHOST
    );
    obj.Set(
      Napi::String::New(env, "address"),
      Napi::String::New(env, ip_addr_buffer)
    );
    getnameinfo(
      ifa->FirstPrefix->Next->Next->Address.lpSockaddr,
      ifa->FirstPrefix->Next->Next->Address.iSockaddrLength,
      ip_addr_buffer,
      sizeof(ip_addr_buffer),
      NULL,
      0,
      NI_NUMERICHOST
    );
    obj.Set(
      Napi::String::New(env, "broadcast"),
      Napi::String::New(env, ip_addr_buffer)
    );
    out[index] = obj;
    index++;
  }

  if (interfaces) FREE(interfaces);

  return out;
}