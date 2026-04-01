#ifndef SERVER_PROTOCOL_PACKET_H
#define SERVER_PROTOCOL_PACKET_H

#include <cstdio>
#include <cstdlib>          
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector> 

#include "server/protocol/Args.h"

#include <string>
#include <sstream>

struct Request {
    int32_t reqID;
    int32_t serviceID;
    int32_t payloadLength; 
    std::vector<Argument> args;

    Request() : reqID(0), serviceID(0), payloadLength(0) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "REQUEST [ID: " << reqID << " | Service: " << serviceID << "] Args(" << args.size() << "): ";
        for (const auto& arg : args) ss << "{" << arg.toString() << "} ";
        return ss.str();
    }
};

struct Response {
    int32_t reqID;
    char statusCode;
    int32_t payloadLength;
    std::vector<Argument> args;

    Response() : reqID(0), statusCode(0), payloadLength(0) {}

    std::string toString() const {
        std::stringstream ss;
        ss << "RESPONSE [ID: " << reqID << " | Status: " << (int)statusCode << "] Args(" << args.size() << "): ";
        for (const auto& arg : args) ss << "{" << arg.toString() << "} ";
        return ss.str();
    }
};


#endif