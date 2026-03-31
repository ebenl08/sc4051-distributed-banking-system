#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <cstdint>
#include <cstring>

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
            if (type == TYPE_STRING && value.stringValue)
                delete[] value.stringValue;

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

#endif