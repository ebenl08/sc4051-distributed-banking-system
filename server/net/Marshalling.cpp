
#include "Marshalling.h"


// marshal int32_t in big-endian format
void Marshalling::marshal(char* buffer, int32_t value){
  uint32_t uValue = static_cast<uint32_t>(value);
  unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);

  // Manually place bytes in Big-Endian order
  ptr[0] = static_cast<unsigned char>((uValue >> 24) & 0xFF);
  ptr[1] = static_cast<unsigned char>((uValue >> 16) & 0xFF);
  ptr[2] = static_cast<unsigned char>((uValue >> 8)  & 0xFF);
  ptr[3] = static_cast<unsigned char>(uValue & 0xFF);
}

// marshal int64_t in big-endian format
void Marshalling::marshal(char* buffer, int64_t value){
  uint64_t uValue = static_cast<uint64_t>(value);
  unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);

  ptr[0] = static_cast<unsigned char>((uValue >> 56) & 0xFF);
  ptr[1] = static_cast<unsigned char>((uValue >> 48) & 0xFF);
  ptr[2] = static_cast<unsigned char>((uValue >> 40) & 0xFF);
  ptr[3] = static_cast<unsigned char>((uValue >> 32) & 0xFF);
  ptr[4] = static_cast<unsigned char>((uValue >> 24) & 0xFF);
  ptr[5] = static_cast<unsigned char>((uValue >> 16) & 0xFF);
  ptr[6] = static_cast<unsigned char>((uValue >> 8)  & 0xFF);
  ptr[7] = static_cast<unsigned char>(uValue & 0xFF);
}

// marshal float in big-endian format
void Marshalling::marshal(char* buffer, float value){

  uint32_t bits;
  // Copy the bit pattern of the float into an unsigned integer
  static_assert(sizeof(float) == sizeof(uint32_t), "Float and Uint32 size mismatch");
  
  std::memcpy(&bits, &value, sizeof(value));

  // Manually place bytes in Big-Endian order
  unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);
  ptr[0] = static_cast<unsigned char>((bits >> 24) & 0xFF);
  ptr[1] = static_cast<unsigned char>((bits >> 16) & 0xFF);
  ptr[2] = static_cast<unsigned char>((bits >> 8)  & 0xFF);
  ptr[3] = static_cast<unsigned char>(bits & 0xFF);

}

// marshal string as length-prefixed data in big-endian format
void Marshalling::marshal(char* buffer, const char* value){
  int32_t length = strlen(value);
  marshal(buffer, length); // First write the length of the string
  memcpy(buffer + 4, value, length);
}

// marshal single-byte char
void Marshalling::marshal(char* buffer, char value){
  *buffer = (uint8_t) value;
}


// unmarshal int32_t from big-endian format
int32_t Marshalling::unmarshalInt32(char* buffer){
  unsigned char* tmpbuffer = (unsigned char*) buffer;
  int32_t value = ((uint32_t) tmpbuffer[0] << 24 | 
                  (uint32_t) tmpbuffer[1] << 16 | 
                  (uint32_t) tmpbuffer[2] << 8 | 
                  (uint32_t) tmpbuffer[3]);
  return value;
}

// unmarshal int64_t from big-endian format
int64_t Marshalling::unmarshalInt64(char* buffer){
  const unsigned char* tmpbuffer = (unsigned char*) buffer;
  int64_t value = (((uint64_t)tmpbuffer[0] << 56) | ((uint64_t)tmpbuffer[1] << 48) |
                   ((uint64_t)tmpbuffer[2] << 40) | ((uint64_t)tmpbuffer[3] << 32) |
                   ((uint64_t)tmpbuffer[4] << 24) | ((uint64_t)tmpbuffer[5] << 16) |
                   ((uint64_t)tmpbuffer[6] << 8) | (uint64_t)tmpbuffer[7]);
  return value;
}

// unmarshal float from big-endian format
float Marshalling::unmarshalFloat(char* buffer){
  unsigned char* tmp = reinterpret_cast<unsigned char*>(buffer);
  uint32_t bits = (static_cast<uint32_t>(tmp[0]) << 24) |
                  (static_cast<uint32_t>(tmp[1]) << 16) |
                  (static_cast<uint32_t>(tmp[2]) << 8)  |
                  (static_cast<uint32_t>(tmp[3]));
  float value;
  std::memcpy(&value, &bits, sizeof(value));
  return value;
}

// unmarshal single-byte char
char Marshalling::unmarshalChar(char* buffer){
  return buffer[0];
}

// unmarshal length-prefixed string from big-endian format
std::string Marshalling::unmarshalString(char* buffer){
  char* ptr = buffer;
  int32_t length = Marshalling::unmarshalInt32(ptr);
  ptr += 4;

  std::string value(ptr, length);
  return value;
}

// 10.50, 1050, 10$ 50cents

