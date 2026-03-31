#include "server/Constant.h"
#include "server/Handler.h"
#include "server/Helper.h"

#include "server/net/UDPServer.h"
#include "server/net/PacketBuilder.h"

#include "server/protocol/SchemaFactory.h"
#include "server/protocol/Packet.h"
#include "server/protocol/Args.h"

using namespace std;

UDPServer::UDPServer(serverConfig config) : 
    socket(config.socket), 
    semanticInvoc(config.semanticInvoc), 
    maxRetry(config.maxRetry), 
    handler(config.handler)
{
}


int32_t UDPServer::startServer(){
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    sockaddr_in clientaddr;
    

    while(true){
        // wait for request
        int32_t responseLength = serveRequest(buffer, response, clientaddr);
        if(responseLength > 0) {
            socket.send(response, responseLength, clientaddr);
        }

        memset(buffer, 0, sizeof(buffer));
        memset(response, 0, sizeof(response));
        memset(&clientaddr, 0, sizeof(clientaddr));
    }
}


int32_t UDPServer::serveRequest(char* buffer, char* response, sockaddr_in& clientaddr){

    int32_t counter = 0;

    int n = socket.receive(buffer, clientaddr);
    
    pdebug("[Client %s port %d] Packet received\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    
    // parse request packet
    Request request = PacketBuilder::parseRequestPacket(buffer, counter);
    pdebug("Parsed request: %s\n", request.toString().c_str());

    // initialize response packet
    Response response_pkt;

    if(semanticInvoc == AT_MOST_ONE_SEM_INVO 
        && (isOutdatedRequest(clientaddr, request.reqID)
         || isDuplicateRequest(clientaddr, request.reqID)
        )
    ){
        // use cached response
        pdebug("Duplicate or outdated request detected. Retrieving cached response.\n");
        MapKey mapKey = generateMapKey(clientaddr, request.reqID);
        response_pkt = request_client_map[mapKey];
    }
    else{
        // process request and generate response
        pdebug("Processing request with service ID %d\n", request.serviceID);
        response_pkt = this->handler.handle_service(request, clientaddr);

        // cache response for at-most-one semantic invocation
        pdebug("Caching response for request ID %d\n", request.reqID);
        if(semanticInvoc == AT_MOST_ONE_SEM_INVO) {
            uint64_t networkKey = generateNetworkKey(clientaddr);
            latest_request_map[networkKey] = request.reqID;
            MapKey mapKey = generateMapKey(clientaddr, request.reqID);
            request_client_map[mapKey] = response_pkt;
        }
    }

    response_pkt.reqID = request.reqID; // Ensure response has the same reqID as the request

    // build response packet
    int32_t responseLength = PacketBuilder::buildResponsePacket(response, response_pkt);
    response_pkt.payloadLength = responseLength; // Update payload length in the response packet (for internal use)

    pdebug("Response packet built with length %d bytes\n", responseLength);
    pdebug("Sending response to client %s port %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    pdebug("Response: %s\n", response_pkt.toString().c_str());
    return response_pkt.payloadLength;
}


uint64_t UDPServer::generateNetworkKey(const sockaddr_in& clientaddr) {
    return (static_cast<uint64_t>(ntohl(clientaddr.sin_addr.s_addr)) << 16) | ntohs(clientaddr.sin_port);
}

MapKey UDPServer::generateMapKey(const sockaddr_in& clientaddr, int32_t requestId) {
    MapKey key;
    key.reqID = requestId;
    key.networkaddr = generateNetworkKey(clientaddr);
    return key;
}

bool UDPServer::isDuplicateRequest(const sockaddr_in& clientaddr, int32_t reqID) {
    MapKey mapKey = generateMapKey(clientaddr, reqID);
    auto it = request_client_map.find(mapKey);
    return it != request_client_map.end() && it->second.reqID == reqID;
}

bool UDPServer::isOutdatedRequest(const sockaddr_in& clientaddr, int32_t reqID) {
    uint64_t networkKey = generateNetworkKey(clientaddr);
    auto it = latest_request_map.find(networkKey);
    return it != latest_request_map.end() && it->second < reqID;
}





