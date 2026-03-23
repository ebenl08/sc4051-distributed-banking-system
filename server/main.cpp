#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <iostream>

#include "UDPServer.h"
#include "Marshalling.h"

enum DataType {
    TYPE_STRING,
    TYPE_INT32,
    TYPE_INT64
};

struct Argument {
    DataType type;
    std::string stringValue;
    int32_t int32Value = 0;
    int64_t int64Value = 0;
};

std::vector<Argument> parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema) {
    std::vector<Argument> args;

    for (DataType type : schema) {
        Argument arg;
        arg.type = type;

        if (type == TYPE_STRING) {
            char* value = Marshalling::unmarshalString(buffer + counter);
            arg.stringValue = value;
            counter += 4 + strlen(value);
            delete[] value;
        }
        else if (type == TYPE_INT32) {
            arg.int32Value = Marshalling::unmarshalInt32(buffer + counter);
            counter += 4;
        }
        else if (type == TYPE_INT64) {
            arg.int64Value = Marshalling::unmarshalInt64(buffer + counter);
            counter += 8;
        }

        args.push_back(arg);
    }

    return args;
}

int main(){
  int port = 8080;
  const char* hostname = "localhost";

  UDPServer server = UDPServer(port, hostname);
  
  do{
    char buffer[BUFFER_SIZE];

    sockaddr_in clientaddr = server.receive((char*) buffer);

    //format wil be reqID(4 bytes), serviceID(4 bytes), statusCode(4 bytes), payloadHeader, payload, payloadHeader, payload, ...
    int32_t reqID = unmarshalInt32(buffer);
    int32_t serviceID = unmarshalInt32(buffer + 4);
    int32_t statusCode = unmarshalInt32(buffer + 4)

    int counter = 12;
    if(serviceID == 0){
      // Open New Account
      std::vector<DataType> schema = {
        TYPE_STRING,
        TYPE_STRING,
        TYPE_STRING,
        TYPE_INT64
      };
      std::vector<Argument> args = parseArgs(buffer, counter, schema);
      
    } else if(serviceID == 1){
      // Close Account
      std::vector<DataType> schema = {
        TYPE_STRING,
        TYPE_INT32,
        TYPE_STRING
      };
      std::vector<Argument> args = parseArgs(buffer, counter, schema);

    } else if(serviceID == 2){
      // Deposit/Withdraw
      std::vector<DataType> schema = {
        TYPE_STRING,
        TYPE_INT32,
        TYPE_STRING,
        TYPE_STRING,
        TYPE_INT64
      };
      std::vector<Argument> args = parseArgs(buffer, counter, schema);

    } else if(serviceID == 3){
      // Monitor All Account

    } else if(serviceID == 4){
      // Idempotent additional service

    } else if(serviceID == 5){
      // Non-Idempotent additional service

    }

    printf("[Client %s port %d] Message: %s", 
      inet_ntoa(clientaddr.sin_addr), 
      ntohs(clientaddr.sin_port),
      buffer);

    const char* message = "Hello from Server";
    server.send(message, clientaddr);
    
    printf("Response message sent.\n");
  }while(true);
}