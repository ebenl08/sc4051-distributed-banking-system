#ifndef UDPSERVER_H
#define UDPSERVER_H

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
#include <vector> 

#include "Marshalling.h"
#include "Handler.h"
#include "Argument.h"

#define BUFFER_SIZE 1500

class UDPServer{
  public:
    UDPServer();

    static int32_t  Server(char* buffer, char* respond, int& length);
    static std::vector<Argument> parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema);

  private:
};

#endif