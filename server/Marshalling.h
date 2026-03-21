#ifndef MARSHALLING_H
#define MARSHALLING_H

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

using namespace std;

#define BUFFER_SIZE 1500

class Marshalling{
  public:
    Marshalling();

    static void marshal(char* buffer, int32_t val);
    static void marshal(char* buffer, int64_t val);
    static void marshal(char* buffer, float val);
    static void marshal(char* buffer, const char* val);

    static int32_t unmarshalInt32(char* buffer);
    static int64_t unmarshalInt64(char* buffer);
    static float unmarshalFloat(char* buffer);
    static const char* unmarshalString(char* buffer);

  private:
};

#endif