#ifndef SERVER_NET_MARSHALLING_H
#define SERVER_NET_MARSHALLING_H

#include <sys/types.h>      // System data type. e.g. size_t, ssize_t
#include <sys/socket.h>     // main library for creating sockets
#include <netdb.h>          // include byte-ordering, address lookup functions
#include <netinet/in.h>     // dictates how IP addresses and ports are formatted, e.g. struct sockaddr_in, etc.
#include <arpa/inet.h> 

#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <cstdint>          // for uint64_t, int64_t, etc.

#include "server/Constant.h"

using namespace std;

// 64-bit conversions
static inline uint64_t (htonll)(uint64_t value) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return ((uint64_t)htonl((uint32_t)(value & 0xFFFFFFFFULL)) << 32) |
            htonl((uint32_t)(value >> 32));
#else
    return value;
#endif
}

static inline uint64_t (ntohll)(uint64_t value) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return ((uint64_t)ntohl((uint32_t)(value & 0xFFFFFFFFULL)) << 32) |
            ntohl((uint32_t)(value >> 32));
#else
    return value;
#endif
}

class Marshalling{
  public:
    Marshalling();

    static void marshal(char* buffer, int32_t val);
    static void marshal(char* buffer, int64_t val);
    static void marshal(char* buffer, float val);
    static void marshal(char* buffer, char val);
    static void marshal(char* buffer, const char* val);

    static int32_t unmarshalInt32(char* buffer);
    static int64_t unmarshalInt64(char* buffer);
    static float unmarshalFloat(char* buffer);
    static char unmarshalChar(char* buffer);
    static std::string unmarshalString(char* buffer);

  private:
};

#endif