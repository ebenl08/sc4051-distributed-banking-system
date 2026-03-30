#ifndef SERVER_PROTOCOL_PACKET_H
#define SERVER_PROTOCOL_PACKET_H

#include <cstdio>
#include <cstdlib>          
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector> 

#include "server/protocol/Args.h"

typedef struct {
    int32_t reqID;
    int32_t serviceID;
    int32_t payloadLength; // for internal use, not transmitted
    std::vector<Argument> args;
} Request;

typedef struct {
    int32_t reqID;
    char statusCode;
    int32_t payloadLength; // for internal use, not transmitted
    std::vector<Argument> args;
} Response;


#endif