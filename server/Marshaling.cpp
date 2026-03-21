
#include "Marshalling.h";


void Marshalling::marshal(char* buffer, int32_t value){
  char* ptr = buffer;
  memset(buffer, 0, sizeof(int32_t));
  *ptr |= value>>24; ++ptr;
  *ptr |= value>>16; ++ptr;
  *ptr |= value>>8; ++ptr;
  *ptr |= value>>0; ++ptr;
}

void Marshalling::marshal(char* buffer, int64_t value){
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

}

void Marshalling::marshal(char* buffer, const char* value){

}

int32_t Marshalling::unmarshalInt32(char* buffer){

}

float Marshalling::unmarshalFloat(char* buffer){

}

char* Marshalling::unmarshalString(char* buffer){

}