#include "Communication.h"
#include "Marshalling.h"
using namespace std;

std::vector<Argument> Communication::parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema) {
    std::vector<Argument> args;
    args.reserve(schema.size());

    for (DataType type : schema) {
        Argument arg;
        arg.type = type;

        if (type == TYPE_STRING) {
            arg.value.stringValue = Marshalling::unmarshalString(buffer + counter);
            //std::cout<<arg.value.stringValue<<endl;
            counter += 4 + strlen(arg.value.stringValue);
        }
        else if (type == TYPE_INT32) {
            arg.value.int32Value = Marshalling::unmarshalInt32(buffer + counter);
            //std::cout<<arg.value.int32Value<<endl;
            counter += 4;
        }
        else if (type == TYPE_INT64) {
            arg.value.int64Value = Marshalling::unmarshalInt64(buffer + counter);
            //std::cout<<arg.value.int64Value<<endl;
            counter += 8;
        }
        else if (type == TYPE_CHAR) {
            arg.value.charValue = buffer[counter];
            //std::cout<<arg.value.charValue<<endl;
            counter += 1;
        }

        args.push_back(std::move(arg));
    }

    return args;
}

void Communication::communication_layer(char* buffer){
    int32_t reqID = Marshalling::unmarshalInt32(buffer);
    int32_t serviceID = Marshalling::unmarshalInt32(buffer + 4);
    int32_t statusCode = Marshalling::unmarshalInt32(buffer + 8);
    int counter = 12;

    if(serviceID == 0){
        // Open New Account
        vector<DataType> schema = {
            TYPE_STRING,
            TYPE_STRING,
            TYPE_CHAR,
            TYPE_INT64
        };
        vector<Argument> args = parseArgs(buffer, counter, schema);
    } else if(serviceID == 1){
        // Close Account
        vector<DataType> schema = {
            TYPE_STRING,
            TYPE_INT32,
            TYPE_STRING
        };
        vector<Argument> args = parseArgs(buffer, counter, schema);
    } else if(serviceID == 2){
        // Deposit/Withdraw
        vector<DataType> schema = {
            TYPE_STRING,
            TYPE_INT32,
            TYPE_STRING,
            TYPE_STRING,
            TYPE_INT64
        };
        vector<Argument> args = parseArgs(buffer, counter, schema);
    } else if(serviceID == 3){
        // Monitor All Account
    } else if(serviceID == 4){
        // Idempotent additional service
        vector<DataType> schema = {
            TYPE_STRING,
            TYPE_INT32,
            TYPE_STRING,
            TYPE_INT32,
            TYPE_CHAR,
            TYPE_CHAR,
            TYPE_STRING,
            TYPE_INT64
        };
        vector<Argument> args = parseArgs(buffer, counter, schema);
    } else if(serviceID == 5){
        // Non-Idempotent additional service
        vector<DataType> schema = {
            TYPE_STRING,
            TYPE_INT32,
            TYPE_STRING,
            TYPE_STRING,
            TYPE_INT64
        };
        vector<Argument> args = parseArgs(buffer, counter, schema);
    }
}