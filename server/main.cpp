#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <iostream>

#include "UDPServer.h"
#include "Marshalling.h"
#include "Communication.h"

int main(){
  int port = 8080;
  const char* hostname = "localhost";

  UDPServer server = UDPServer(port, hostname);
  
  do{
    char buffer[BUFFER_SIZE];

    sockaddr_in clientaddr = server.receive((char*) buffer);


    printf("Message received\n");
    //format wil be reqID(4 bytes), serviceID(4 bytes), statusCode(4 bytes), payloadHeader, payload, payloadHeader, payload, ...
    Communication::communication_layer(buffer);

    printf("[Client %s port %d] Packet received\n", 
      inet_ntoa(clientaddr.sin_addr), 
      ntohs(clientaddr.sin_port));

    const char* message = "Hello from Server";
    server.send(message, clientaddr);
    
    printf("Response message sent.\n");
  }while(true);
}