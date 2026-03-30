
#include "server/net/PacketBuilder.h"
#include "server/protocol/SchemaFactory.h"
#include "server/Constant.h"


Response PacketBuilder::parseCallbackResponsePacket(char* buffer, int& counter) {
    Response response; int32_t counter = 0;
    response.reqID = Marshalling::unmarshalInt32(buffer + counter); counter += 4;
    response.statusCode = Marshalling::unmarshalChar(buffer + counter++);
    response.args = parseArgs(buffer, counter, {TYPE_STRING});
    response.payloadLength = counter; // Set payload length to total bytes read
    return response;
}

Request PacketBuilder::parseRequestPacket(char* buffer, int& counter) {
    Request request; 
    ServiceSchema schema = SchemaFactory::generateSchema(request.serviceID);
    request.reqID = Marshalling::unmarshalInt32(buffer + counter); counter += INT32_SIZE;
    request.serviceID = Marshalling::unmarshalInt32(buffer + counter); counter += INT32_SIZE;
    request.args = parseArgs(buffer, counter, schema.inputTypes);
    return request;
}

int32_t PacketBuilder::buildCallbackPacket(char* buffer, const Request& request) {
    int32_t counter = 0;
    Marshalling::marshal(buffer + counter, request.reqID); counter += INT32_SIZE;
    Marshalling::marshal(buffer + counter, request.serviceID); counter += INT32_SIZE;
    buildArgs(buffer, counter, request.args);
    return counter;
}

int32_t PacketBuilder::buildResponsePacket(char* buffer, const Response& response) {
    int32_t counter = 0;
    Marshalling::marshal(buffer + counter, response.reqID); counter += INT32_SIZE;
    Marshalling::marshal(buffer + counter, response.statusCode); counter += CHAR_SIZE;
    buildArgs(buffer, counter, response.args);
    return counter;
}

void PacketBuilder::buildArgs(char* buffer, int& counter, const std::vector<Argument>& args) {
    for (const Argument& arg : args) {
        if (arg.type == TYPE_STRING) {
            Marshalling::marshal(buffer + counter, arg.value.stringValue); counter += INT32_SIZE + strlen(arg.value.stringValue);
        }
        else if (arg.type == TYPE_INT32) {
            Marshalling::marshal(buffer + counter, arg.value.int32Value); counter += INT32_SIZE;
        }
        else if (arg.type == TYPE_INT64) {
            Marshalling::marshal(buffer + counter, arg.value.int64Value); counter += INT64_SIZE;
        }
        else if (arg.type == TYPE_CHAR) {
            Marshalling::marshal(buffer + counter, arg.value.charValue); counter += CHAR_SIZE;
        }
    }
}

std::vector<Argument> PacketBuilder::parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema) {
    std::vector<Argument> args;
    args.reserve(schema.size());

    for (DataType type : schema) {
        Argument arg;
        arg.type = type;

        if (type == TYPE_STRING) {
            arg.value.stringValue = Marshalling::unmarshalString(buffer + counter);
            //std::cout<<arg.value.stringValue<<endl;
            counter += INT32_SIZE + strlen(arg.value.stringValue);
        }
        else if (type == TYPE_INT32) {
            arg.value.int32Value = Marshalling::unmarshalInt32(buffer + counter);
            //std::cout<<arg.value.int32Value<<endl;
            counter += INT32_SIZE;
        }
        else if (type == TYPE_INT64) {
            arg.value.int64Value = Marshalling::unmarshalInt64(buffer + counter);
            //std::cout<<arg.value.int64Value<<endl;
            counter += INT64_SIZE;
        }
        else if (type == TYPE_CHAR) {
            arg.value.charValue = buffer[counter];
            //std::cout<<arg.value.charValue<<endl;
            counter += CHAR_SIZE;
        }

        args.push_back(std::move(arg));
    }

    return args;
}
