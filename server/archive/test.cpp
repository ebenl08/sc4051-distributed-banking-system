
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

#include "../Marshalling.h"

// void marshalInt(char*  , int32_t val){
// }
// int32_t unmarshalInt(char* b){
// }

void printBuffer(char* buffer, int32_t length){
  printf("0x");
  for(int i = 0; i < length; i++){
    printf("%02x", buffer[i]);
  }printf("\n");
}

int main(){

  int32_t val32 = 0x180095AB;
  int64_t val64 = 0x180095AB180095AB;
  float valf = 3.14f;
  const char* valstr = "Hello I am a client";
  
  char buffer[1500];
  
  printf("%x\n", val32);
  Marshalling::marshal(buffer, val32);
  for (int i = 0; i < 4; i++) {
    printf("%02x ", (unsigned char)buffer[i]);
  }
  printf("\n");
  val32 = Marshalling::unmarshalInt32(buffer);
  printf("%x\n", val32);
  
  memset(buffer, 0, sizeof(buffer));
  
  printf("%llx\n", val64);
  Marshalling::marshal(buffer, val64);
  val64 = Marshalling::unmarshalInt64(buffer);
  printf("%llx\n", val64);

  memset(buffer, 0, sizeof(buffer));

  printf("%.2f\n", valf);
  Marshalling::marshal(buffer, valf);
  valf = Marshalling::unmarshalFloat(buffer);
  printf("%.2f\n", valf);

  memset(buffer, 0, sizeof(buffer));

  printf("%s\n", valstr);
  Marshalling::marshal(buffer, valstr);
  valstr = Marshalling::unmarshalString(buffer);
  printf("%s\n", valstr);

  return 0;
}

void test1(){
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
}