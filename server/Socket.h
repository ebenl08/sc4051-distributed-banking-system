#ifndef SOKET_H
#define SOKET_H

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

class Socket{
  public:
    Socket(int16_t port, const char* hostname);
    void send(const char* buffer, int size, const sockaddr_in clientaddr);
    sockaddr_in receive(char* buffer);
  private:
    void makeAddress(sockaddr_in* addr, const char* hostname, int16_t port);
    int servsocket;
    sockaddr_in servaddr;
    int semantic_invoc;
    int msg_id_counter;
};

#endif