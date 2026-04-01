

#ifndef SERVER_PROTOCOL_ARGS_H
#define SERVER_PROTOCOL_ARGS_H

#include <cstdio>
#include <cstdlib>          
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector> 

enum DataType {
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_FLOAT
};

struct Argument {
    DataType type;

    union Value {
        int32_t int32Value;
        int64_t int64Value;
        float   floatValue;
        char*   stringValue;
        char    charValue;
    } value;


    // String Constructor (C-style)
    Argument(const char* str) : type(TYPE_STRING) {
        if (str) {
            size_t len = strlen(str) + 1;
            value.stringValue = new char[len];
            memcpy(value.stringValue, str, len);
        } else {
            value.stringValue = nullptr;
        }
    }
    // String Constructor (std::string)
    Argument(const std::string& str) : Argument(str.c_str()) {}
    // Integer Constructors
    Argument(int32_t val) : type(TYPE_INT32) { value.int32Value = val; }
    Argument(int64_t val) : type(TYPE_INT64) { value.int64Value = val; }
    // Float Constructor
    Argument(float val) : type(TYPE_FLOAT) { value.floatValue = val; }
    // Char Constructor
    Argument(char val) : type(TYPE_CHAR) { value.charValue = val; }
    // Default Constructor
    Argument() : type(TYPE_INT32) { value.int32Value = 0; }

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

    std::string toString() const {
        char buf[128];
        switch (type) {
            case TYPE_INT32:  snprintf(buf, sizeof(buf), "INT32: %d", value.int32Value); break;
            case TYPE_INT64:  snprintf(buf, sizeof(buf), "INT64: %lld", value.int64Value); break;
            case TYPE_FLOAT:  snprintf(buf, sizeof(buf), "FLOAT: %.2f", value.floatValue); break;
            case TYPE_CHAR:   snprintf(buf, sizeof(buf), "CHAR: %c %d", value.charValue, (uint8_t) value.charValue); break;
            case TYPE_STRING: return std::string("STRING: ") + value.stringValue;
            default:          return "UNKNOWN TYPE";
        }
        return std::string(buf);
    }
};

#endif