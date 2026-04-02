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
    handler(config.handler),
    errRate(config.errRate)
{
    latest_request_map.reserve(1024); // Reserve space for 1000 clients to reduce rehashing
    request_client_map.reserve(1024); // Reserve space for 1000 requests to reduce rehashing
}


int32_t UDPServer::startServer(){
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    sockaddr_in clientaddr;
    

    while(true){
        // wait for request
        
        int32_t responseLength = serveRequest(buffer, response, clientaddr);
        
        if(responseLength > 0) {
            // extract reqID for logging and potential loss simulation
            int32_t requestId = Marshalling::unmarshalInt32(buffer); 

            // random loss simulation
            if (rand() % 100 < this->errRate) 
                pwarn("REQUEST PACKET LOSS FOR REQ ID %d", requestId);

            // send response
            else 
                socket.send(response, responseLength, clientaddr);
        }

        // clean buffers and client address for next request
        memset(buffer, 0, sizeof(buffer));
        memset(response, 0, sizeof(response));
        memset(&clientaddr, 0, sizeof(clientaddr));
    }
}


int32_t UDPServer::serveRequest(char* buffer, char* response, sockaddr_in& clientaddr){

    int32_t counter = 0;

    int n = socket.receive(buffer, clientaddr);

    pinfo("\n\n");
    
    
    // parse request packet
    Request request = PacketBuilder::parseRequestPacket(buffer, counter);
    pinfo("RECV FROM [CLIENT %s:%d]\n>>>>>>> REQUEST: %s ", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), request.toString().c_str());
    pdebug("RAW REQ: %s", hex_to_string(buffer, n).c_str());
    pdebug("PARSE REQ: %s", request.toString().c_str());

    // initialize response packet
    Response response_pkt = Response();

    if(semanticInvoc == AT_MOST_ONE_SEM_INVO 
        && isDuplicateRequest(clientaddr, request.reqID)
    ){
        // use cached response
        pwarn("DUPLICATE REQUEST FOR ID %d, LOADING CACHED RESPONSE...", request.reqID);
        MapKey mapKey = generateMapKey(clientaddr, request.reqID);
        response_pkt = request_client_map[mapKey];
    }
    else{
        // process request and generate response
        pinfo("PROCESSING SERVICE ID %d", request.serviceID);
        response_pkt = this->handler.handle_service(request, clientaddr);
        response_pkt.reqID = request.reqID;

        // cache response for at-most-one semantic invocation
        pinfo("CACHING RESPONSE FOR REQUEST ID %d", request.reqID);
        if(semanticInvoc == AT_MOST_ONE_SEM_INVO) {
            uint64_t networkKey = generateNetworkKey(clientaddr);
            latest_request_map[networkKey] = request.reqID;
            MapKey mapKey = generateMapKey(clientaddr, request.reqID);
            request_client_map[mapKey] = response_pkt;
        }

    }

    // build response packet
    int32_t responseLength = PacketBuilder::buildResponsePacket(response, response_pkt);
    response_pkt.payloadLength = responseLength; // Update payload length in the response packet (for internal use)

    std::stringstream debug_ss;
    debug_ss << "Response packet built with length " << responseLength << " bytes\n";
    debug_ss << "Sending response to client " << inet_ntoa(clientaddr.sin_addr) << " port " << ntohs(clientaddr.sin_port) << "\n";
    debug_ss << response_pkt.toString() << "\n";
    debug_ss << hex_to_string(response, responseLength);
    pdebug("%s", debug_ss.str().c_str());

    pinfo(">>>>>>> RESPONSE SENT TO [CLIENT %s:%d]\n%s", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), response_pkt.toString().c_str());

    return response_pkt.payloadLength;
}


// Generate a unique key for the client based on its IP and port
uint64_t UDPServer::generateNetworkKey(const sockaddr_in& clientaddr) {
    return (static_cast<uint64_t>(ntohl(clientaddr.sin_addr.s_addr)) << 16) | ntohs(clientaddr.sin_port);
}

// Generate a unique key for the request based on client address (network key) and request ID
MapKey UDPServer::generateMapKey(const sockaddr_in& clientaddr, int32_t requestId) {
    MapKey key;
    key.reqID = requestId;
    key.networkaddr = generateNetworkKey(clientaddr);
    return key;
}

// Check if the request is a duplicate (same reqID from the same client)
bool UDPServer::isDuplicateRequest(const sockaddr_in& clientaddr, int32_t reqID) {
    MapKey mapKey = generateMapKey(clientaddr, reqID);
    auto it = request_client_map.find(mapKey);
    pinfo("IS PACKET DUPLICATE? %d", it != request_client_map.end() && it->second.reqID == reqID);
    return it != request_client_map.end() && it->second.reqID == reqID;
}

// Check if the request is outdated (reqID is less than the latest reqID seen from the same client)
bool UDPServer::isOutdatedRequest(const sockaddr_in& clientaddr, int32_t reqID) {
    uint64_t networkKey = generateNetworkKey(clientaddr);
    auto it = latest_request_map.find(networkKey);
    pdebug("IS PACKET OUTDATED? %d", it != latest_request_map.end() && it->second < reqID);
    return it != latest_request_map.end() && it->second < reqID;
}





