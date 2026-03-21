
#include "Marshalling.h"


void Marshalling::marshal(char* buffer, int32_t value){
  value = htonl(value);
  char* ptr = buffer;
  memset(buffer, 0, sizeof(int32_t));
  *ptr |= value>>24; ++ptr;
  *ptr |= value>>16; ++ptr;
  *ptr |= value>>8; ++ptr;
  *ptr |= value>>0; ++ptr;
}

void Marshalling::marshal(char* buffer, int64_t value){
  value = htonll(value);
  char* ptr = buffer;
  memset(buffer, 0, sizeof(int32_t));
  *ptr |= value>>56; ++ptr;
  *ptr |= value>>48; ++ptr;
  *ptr |= value>>40; ++ptr;
  *ptr |= value>>32; ++ptr;
  *ptr |= value>>24; ++ptr;
  *ptr |= value>>16; ++ptr;
  *ptr |= value>>8; ++ptr;
  *ptr |= value>>0; ++ptr;
}

void Marshalling::marshal(char* buffer, float value){
  memcpy(buffer, &value, sizeof(value));
}

void Marshalling::marshal(char* buffer, const char* value){
  int32_t length = strlen(value);
  memcpy(buffer, &length, sizeof(int32_t));
  memcpy(buffer + 4, value, length);
}

int32_t Marshalling::unmarshalInt32(char* buffer){
  unsigned char* tmpbuffer = (unsigned char*) buffer;
  int32_t value = ((uint32_t) tmpbuffer[0] << 24 | 
                  (uint32_t) tmpbuffer[1] << 16 | 
                  (uint32_t) tmpbuffer[2] << 8 | 
                  (uint32_t) tmpbuffer[3]);
  return ntohl(value);
}

int64_t Marshalling::unmarshalInt64(char* buffer){
  const unsigned char* tmpbuffer = (unsigned char*) buffer;
  int64_t value = (((uint64_t)tmpbuffer[0] << 56) | ((uint64_t)tmpbuffer[1] << 48) |
                   ((uint64_t)tmpbuffer[2] << 40) | ((uint64_t)tmpbuffer[3] << 32) |
                   ((uint64_t)tmpbuffer[4] << 24) | ((uint64_t)tmpbuffer[5] << 16) |
                   ((uint64_t)tmpbuffer[6] << 8) | (uint64_t)tmpbuffer[7]);
  return ntohll(value);
}

float Marshalling::unmarshalFloat(char* buffer){
  float value;

  // Big endian (Network) to Little endian (host)
  // char tmpbuffer[4] = { buffer[3], buffer[2], buffer[1], buffer[0]};
  memcpy(&value, buffer, sizeof(value));
  return value;
}

const char* Marshalling::unmarshalString(char* buffer){
  char* ptr = buffer;
  int32_t length = Marshalling::unmarshalInt32(ptr);
  ptr += 4;

  char* value = new char[length + 1];
  memcpy(value, ptr, length);
  value[length] = '\0';

  return value;
}

// 10.50, 1050, 10$ 50cents

