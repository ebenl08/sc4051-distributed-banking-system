#ifndef SERVER_NET_UDPSERVER_H
#define SERVER_NET_UDPSERVER_H

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
#include <unordered_map>

#include "server/net/Marshalling.h"
#include "server/Constant.h"
#include "server/protocol/Args.h"
#include "server/protocol/Packet.h"

#include "server/net/Socket.h"
#include "server/Handler.h"

typedef struct {
    int32_t semanticInvoc;
    int32_t maxRetry;
    int32_t errRate;
    Socket& socket;
    Handler& handler;
} serverConfig;

struct MapKey {
    int32_t reqID;
    uint64_t networkaddr;
    bool operator==(const MapKey& other) const {
        return reqID == other.reqID && networkaddr == other.networkaddr;
    }
};

struct MapKeyHasher {
    std::size_t operator()(const MapKey& k) const {
        return std::hash<int32_t>()(k.reqID) ^ (std::hash<uint64_t>()(k.networkaddr) << 1);
    }
};


class UDPServer{
  public:
    Handler handler;  

    UDPServer();
    UDPServer(serverConfig config);

    int32_t startServer();
    int32_t serveRequest(char* buffer, char* response, sockaddr_in& clientaddr);
    std::vector<Argument> parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema);

  private:
    // Network Session Layer
    int32_t semanticInvoc = AT_LEAST_ONE_SEM_INVO;
    int32_t requestIdCounter = 0;
    int32_t maxRetry = 3;
    int32_t errRate = 0;
    Socket& socket;
    
    std::unordered_map<MapKey, Response, MapKeyHasher> request_client_map = std::unordered_map<MapKey, Response, MapKeyHasher>(); // Map request ID to client address
    std::unordered_map<uint64_t, int32_t> latest_request_map = std::unordered_map<uint64_t, int32_t>(); // Map client address to latest request ID

    MapKey generateMapKey(const sockaddr_in& clientaddr, int32_t requestId);
    uint64_t generateNetworkKey(const sockaddr_in& clientaddr);

    bool isDuplicateRequest(const sockaddr_in& clientaddr, int32_t reqID);
    bool isOutdatedRequest(const sockaddr_in& clientaddr, int32_t reqID);
};

#endif

