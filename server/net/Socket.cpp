#include "server/net/Socket.h"


// Socket Constructor: Initializes the socket and binds it to the specified port and hostname
Socket::Socket(int16_t port, const char* hostname){

  this->servsocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(this->servsocket < 0){
    perror("Failed to create the socket");
    exit(EXIT_FAILURE);
  }

  makeAddress(&(this->servaddr), hostname, port);
  
  socklen_t len = sizeof(this->servaddr);

  if(bind(this->servsocket, (const struct sockaddr*) &(this->servaddr), len) < 0){
    perror("Failed to bind the socket");
    exit(EXIT_FAILURE);
  }
}


// Send a message to the specified client address
void Socket::send(const char* buffer, int length, const sockaddr_in clientaddr){
  socklen_t len = sizeof(clientaddr);
  sendto(this->servsocket, buffer, length, 0, (const sockaddr*) &clientaddr, len);
}

// Receive a message and populate the client address
int Socket::receive(char* buffer, sockaddr_in& clientaddr){
  socklen_t clientlen = sizeof(clientaddr);

  int n = recvfrom(this->servsocket, (void *) buffer, BUFFER_SIZE, 0, (sockaddr *) &clientaddr, &clientlen);
  if(n < 0){
    perror("Failed to receive message");
    return -1;
  }

  return n;
}

// Helper function to populate sockaddr_in structure
void Socket::makeAddress(sockaddr_in* addr, const char* hostname, int16_t port){
  struct hostent *host;
  if((host = gethostbyname(hostname)) == NULL){
    printf("Unknown Host Name\n");
    exit(-1);
  }
  addr->sin_family = AF_INET;
  addr->sin_addr = *(struct in_addr*) host->h_addr;
  addr->sin_port = htons(port);
}