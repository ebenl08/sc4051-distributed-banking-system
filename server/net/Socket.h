#ifndef SERVER_NET_SOCKET_H
#define SERVER_NET_SOCKET_H

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

#include "server/Constant.h"

using namespace std;

class Socket{
  public:
    Socket(int16_t port, const char* hostname);
    void send(const char* buffer, int size, const sockaddr_in clientaddr);
    int receive(char* buffer, sockaddr_in& clientaddr);
  private:
    void makeAddress(sockaddr_in* addr, const char* hostname, int16_t port);
    int servsocket;
    sockaddr_in servaddr;
};

#endif