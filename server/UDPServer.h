#ifndef UDPSERVER_H
#define UDPSERVER_H

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
#include <vector> 

#include "Marshalling.h"
#include "Handler.h"

#define BUFFER_SIZE 1500

enum DataType {
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_INT32,
    TYPE_INT64
};

struct Argument {
    DataType type;

    union Value {
        int32_t int32Value;
        int64_t int64Value;
        char*   stringValue;
        char    charValue;
    } value;

    ~Argument() {
        if (type == TYPE_STRING && value.stringValue) {
            delete[] value.stringValue;
        }
    }
    Argument(const Argument& other) : type(other.type) {
        if (type == TYPE_STRING && other.value.stringValue) {
            size_t len = strlen(other.value.stringValue) + 1;
            value.stringValue = new char[len];
            memcpy(value.stringValue, other.value.stringValue, len);
        } else {
            value = other.value;
        }
    }
    Argument(Argument&& other) noexcept : type(other.type), value(other.value) {
        if (type == TYPE_STRING) other.value.stringValue = nullptr;
    }
    Argument& operator=(const Argument& other) {
        if (this != &other) {
            if (type == TYPE_STRING && value.stringValue) delete[] value.stringValue;
            type = other.type;
            if (type == TYPE_STRING && other.value.stringValue) {
                size_t len = strlen(other.value.stringValue) + 1;
                value.stringValue = new char[len];
                memcpy(value.stringValue, other.value.stringValue, len);
            } else {
                value = other.value;
            }
        }
        return *this;
    }
    Argument() : type(TYPE_INT32), value{} {}
};

class UDPServer{
  public:
    UDPServer();

    static void Server(char* buffer, char* respond, int& length);
    static std::vector<Argument> parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema);

  private:
};

#endif