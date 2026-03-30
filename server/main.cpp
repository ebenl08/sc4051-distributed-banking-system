#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <arpa/inet.h>

#include "UDPServer.h"
#include "Marshalling.h"
#include "Socket.h"
#include "Handler.h"

using namespace std;

struct CachedReply {
    int32_t reqID;
    string clientIP;
    uint16_t clientPort;
    char response[BUFFER_SIZE];
    int length;
};

int findCachedReply(const vector<CachedReply>& cache, int32_t reqID, const sockaddr_in& clientaddr) {
    string ip = inet_ntoa(clientaddr.sin_addr);
    uint16_t port = ntohs(clientaddr.sin_port);

    for(int i = 0; i < (int)cache.size(); i++){
        if(cache[i].reqID == reqID &&
            cache[i].clientIP == ip &&
            cache[i].clientPort == port){
            return i;
        }
    }
    return -1;
}

int main(){
    int port = 8080;
    const char* hostname = "localhost";

    Socket sock = Socket(port, hostname);

    Handler handler;

    vector<CachedReply> cache;

    do {
        char buffer[BUFFER_SIZE];

        sockaddr_in clientaddr = sock.receive(buffer);

        printf("Message received\n");

        int32_t reqID = Marshalling::unmarshalInt32(buffer);
        int32_t serviceID = Marshalling::unmarshalInt32(buffer + 4);
        handler.setCurrentClientAddr(clientaddr);

        printf("[Client %s port %d] Packet received\n",
               inet_ntoa(clientaddr.sin_addr),
               ntohs(clientaddr.sin_port));

        if(serviceID == 1 || serviceID == 2 || serviceID == 3 || serviceID == 4){
            int idx = findCachedReply(cache, reqID, clientaddr);

            if(idx != -1){
                printf("Duplicate request detected. Resending cached response.\n");
                sock.send(cache[idx].response, cache[idx].length, clientaddr);
                continue;
            }
        }

        char respond[BUFFER_SIZE];
        int length = 0;

        int32_t statusCode = UDPServer::Server(buffer, respond, length);

        if(serviceID == 1 || serviceID == 2 || serviceID == 3 || serviceID == 4){
            CachedReply entry;
            entry.reqID = reqID;
            entry.clientIP = inet_ntoa(clientaddr.sin_addr);
            entry.clientPort = ntohs(clientaddr.sin_port);
            memcpy(entry.response, respond, length);
            entry.length = length;
            cache.push_back(entry);
        }

        char sendBuffer[BUFFER_SIZE];
        int counter = 0;

        // reqID
        Marshalling::marshal(sendBuffer + counter, reqID);
        counter += 4;

        // serviceID
        Marshalling::marshal(sendBuffer + counter, serviceID);
        counter += 4;

        // statusCode
        Marshalling::marshal(sendBuffer + counter, statusCode);
        counter += 4;

        // payload
        memcpy(sendBuffer + counter, respond, length);
        counter += length;

        sock.send(sendBuffer, counter, clientaddr);

        printf("Response message sent.\n");

    } while (true);
}