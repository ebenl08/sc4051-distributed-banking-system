#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <iostream>

#include "UDPServer.h"

int main(){
  int port = 8080;
  const char* hostname = "localhost";

  UDPServer server = UDPServer(port, hostname);
  
  do{
    char buffer[BUFFER_SIZE];

    sockaddr_in clientaddr = server.receive((char*) buffer);

    printf("[Client %s port %d] Message: %s", 
      inet_ntoa(clientaddr.sin_addr), 
      ntohs(clientaddr.sin_port),
      buffer);
    
    const char* message = "Hello from Server";
    server.send(message, clientaddr);
    
    printf("Response message sent.\n");
  }while(true);
}