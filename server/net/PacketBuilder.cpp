
#include "PacketBuilder.h"
#include "server/protocol/SchemaFactory.h"
#include "server/Constant.h"
#include "server/Helper.h"


Response PacketBuilder::parseCallbackResponsePacket(char* buffer, int& counter) {
    Response response = Response();
    response.reqID = Marshalling::unmarshalInt32(buffer + counter); counter += 4;
    response.statusCode = Marshalling::unmarshalChar(buffer + counter++);
    response.args = PacketBuilder::parseArgs(buffer, counter, {TYPE_STRING});
    response.payloadLength = counter; // Set payload length to total bytes read
    return response;
}

Request PacketBuilder::parseRequestPacket(char* buffer, int& counter) {
    Request request = Request(); 
    request.reqID = Marshalling::unmarshalInt32(buffer + counter); counter += INT32_SIZE;
    request.serviceID = Marshalling::unmarshalInt32(buffer + counter); counter += INT32_SIZE;

    ServiceSchema schema = SchemaFactory::generateSchema(request.serviceID);
    request.args = PacketBuilder::parseArgs(buffer, counter, schema.inputTypes);
    return request;
}

int32_t PacketBuilder::buildCallbackPacket(char* buffer, const Request& request) {
    int32_t counter = 0;
    Marshalling::marshal(buffer + counter, request.reqID); counter += INT32_SIZE;
    Marshalling::marshal(buffer + counter, request.serviceID); counter += INT32_SIZE;
    PacketBuilder::buildArgs(buffer, counter, request.args);
    return counter;
}

int32_t PacketBuilder::buildResponsePacket(char* buffer, const Response& response) {
    int32_t counter = 0;
    Marshalling::marshal(buffer + counter, response.reqID); counter += INT32_SIZE;
    Marshalling::marshal(buffer + counter, (char) response.statusCode); counter += CHAR_SIZE;
    PacketBuilder::buildArgs(buffer, counter, response.args);
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
            Marshalling::marshal(buffer + counter, (char)arg.value.charValue); counter += CHAR_SIZE;
        }
        else if (arg.type == TYPE_FLOAT) {
            Marshalling::marshal(buffer + counter, arg.value.floatValue); counter += FLOAT_SIZE;
        }
    }
}

std::vector<Argument> PacketBuilder::parseArgs(char* buffer, int& counter, const std::vector<DataType>& schema) {
    std::vector<Argument> args;
    args.reserve(schema.size());

    pdebug("Parsing arguments based on schema with %zu parameters", schema.size());

    for (DataType type : schema) {
        Argument arg;
        arg.type = type;

        if (type == TYPE_STRING) {
            pdebug("Parsing string argument at buffer offset %d", counter);
            std::string str = Marshalling::unmarshalString(buffer + counter);
            arg.value.stringValue = new char[str.size() + 1];
            std::memcpy(arg.value.stringValue, str.c_str(), str.size() + 1);
            counter += INT32_SIZE + str.size();
        }
        else if (type == TYPE_INT32) {
            pdebug("Parsing int32 argument at buffer offset %d", counter);
            arg.value.int32Value = Marshalling::unmarshalInt32(buffer + counter);
            counter += INT32_SIZE;
        }
        else if (type == TYPE_INT64) {
            pdebug("Parsing int64 argument at buffer offset %d", counter);
            arg.value.int64Value = Marshalling::unmarshalInt64(buffer + counter);
            counter += INT64_SIZE;
        }
        else if (type == TYPE_CHAR) {
            pdebug("Parsing char argument at buffer offset %d", counter);
            arg.value.charValue = Marshalling::unmarshalChar(buffer + counter);
            counter += CHAR_SIZE;
        }
        else if (type == TYPE_FLOAT) {
            pdebug("Parsing float argument at buffer offset %d", counter);
            arg.value.floatValue = Marshalling::unmarshalFloat(buffer + counter);
            counter += FLOAT_SIZE;
        }

        args.push_back(std::move(arg));
    }

    return args;
}
