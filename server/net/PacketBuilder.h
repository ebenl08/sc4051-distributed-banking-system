#ifndef SERVER_NET_PACKETBUILDER_H
#define SERVER_NET_PACKETBUILDER_H

#include <cstdio>
#include <cstdlib>       
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector> 

#include "server/protocol/Packet.h"

#include "server/net/Marshalling.h"
#include "server/Handler.h"


class PacketBuilder {
    public:
        static int32_t buildResponsePacket(char* buffer, const Response& response);
        static int32_t buildCallbackPacket(char* buffer, const Request& request);
        static Request parseRequestPacket(char* buffer, int& counter);
        static Response parseCallbackResponsePacket(char* buffer, int& counter);

    private:
        static std::vector<Argument> parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema);
        static void buildArgs(char* buffer, int& counter, const std::vector<Argument>& args);
    };


#endif