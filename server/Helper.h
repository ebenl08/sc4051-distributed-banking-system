
#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H

#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>

#include "server/Constant.h"


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#if DEBUG
    #define pdebug(fmt, ...) \
        do { \
            time_t now; \
            time(&now); \
            char buf[20]; \
            struct tm *t = localtime(&now); \
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t); \
            fprintf(stderr, ANSI_COLOR_CYAN "[%s] [DEBUG] %s:%d:%s():\n " ANSI_COLOR_RESET fmt "\n", \
            buf, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        } while (0)
#else
    #define pdebug(fmt, ...) do { } while (0)
#endif

#if INFO
    #define pinfo(fmt, ...) \
        do { \
            time_t now; \
            time(&now); \
            char buf[20]; \
            struct tm *t = localtime(&now); \
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t); \
            fprintf(stderr, ANSI_COLOR_GREEN "[%s] [INFO] %s:%d:%s():\n " ANSI_COLOR_RESET fmt "\n", \
            buf, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        } while (0)
#else
    #define pinfo(fmt, ...) do { } while (0)
#endif


#define pwarn(fmt, ...) \
    do { \
        time_t now; \
        time(&now); \
        char buf[20]; \
        struct tm *t = localtime(&now); \
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t); \
        fprintf(stderr, ANSI_COLOR_RED "[%s] [WARN] %s:%d:%s():\n " fmt "\n"  ANSI_COLOR_RESET, \
        buf, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    } while (0)


static inline std::string hex_to_string(const char* data, size_t len) {
    std::stringstream ss;
    ss << "[HEX] ";
    for (size_t i = 0; i < len; ++i) {
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') 
           << (static_cast<int>(data[i]) & 0xFF) << " ";
    }
    return ss.str();
}

static inline std::string currencyToString(char currency) {
    switch (currency) {
        case SGD: return "SGD";
        case USD: return "USD";
        case EUR: return "EUR";
        default: return "UNKNOWN";
    }
}


static inline std::string get_timestamp() {
    std::time_t now = std::time(nullptr);
    std::tm tm_buf;
    
    // Thread-safe version for POSIX (Linux/Mac)
    localtime_r(&now, &tm_buf); 

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

#endif