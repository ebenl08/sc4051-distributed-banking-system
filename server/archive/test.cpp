
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

// void marshalInt(char*  , int32_t val){
  
// }

// int32_t unmarshalInt(char* b){

// }

int main(){
  // u_int64_t port = (0x01234567);
  // printf("%#llx\n", port);
  // port = htonl(port);
  // printf("%#llx\n", port);
  // port = ntohl(port);
  // printf("%#llx\n", port);

  // char

  // u_int64_t newport = 0x0;
  // newport += port >> 24;
  // printf("%#llx\n", newport);
  // newport <<= 4; port >> 2
  // printf("%#llx\n", newport);
  // newport += port >> 16;
  // printf("%#llx\n", newport);
  // newport <<= 16;
  // printf("%#llx\n", newport);
  // newport += port >> 8;
  // printf("%#llx\n", newport);

  int32_t val = 0x1200e22c;
  char buff[1500];
  char *ptr = buff;
  memset(buff, 0, sizeof(buff));

  printf("%#x\n", val);
  val = htonl(val);
  printf("%#x\n", val);
  val = ntohl(val);
  printf("%#x\n", val);

  


  // printf("%lu\n", sizeof(buff));

  // printf("%s\n", buff);

  *ptr = val>>24; ptr+=1;
  *ptr = val>>16; ptr+=1;
  *ptr = val>>8; ptr+=1;
  *ptr = val>>0; ptr+=1;
  *ptr = '\0';
  printf("Hi %s", buff);

  // Unmarshal
  int32_t result = 0;
  for(int i = 0; i < 3; i++){
    if(i < 3) result <<= 8;
    result |= (unsigned char)buff[i];
    printf("%#x\n", result);
  }

  for(int i = 0; i < 4; i++){
    printf("%02x", (unsigned char) buff[i]);
  }

  memset(buff,0, sizeof(buff));
  

  return 0;
}