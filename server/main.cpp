#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <iostream>

#include "UDPServer.h"
#include "Marshalling.h"
#include "Socket.h"

int main(){
  int port = 8080;
  const char* hostname = "localhost";

  Socket sock = Socket(port, hostname);
  
  do{
    char buffer[BUFFER_SIZE];
    //format wil be reqID(4 bytes), serviceID(4 bytes), statusCode(4 bytes), payloadHeader, payload, payloadHeader, payload, ...
    sockaddr_in clientaddr = sock.receive((char*) buffer);

    printf("Message received\n");
    
    char respond[BUFFER_SIZE];
    int length = 0;
    UDPServer::Server(buffer, respond, length);
    cout << length << endl;
    printf("[Client %s port %d] Packet received\n", 
      inet_ntoa(clientaddr.sin_addr), 
      ntohs(clientaddr.sin_port));

    sock.send(respond, length, clientaddr);
    
    printf("Response message sent.\n");
  }while(true);
}