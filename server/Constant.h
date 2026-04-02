
# ifndef SERVER_CONSTANT_H
# define SERVER_CONSTANT_H

#define DEBUG false
#define INFO true

#define ACCOUNT_LENGTH 8

#define SGD 0
#define USD 1
#define EUR 2

// Argument Constant
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 8080
#define DEFAULT_FAILURE_RATE 0
#define DEFAULT_TIMEOUT 1000
#define DEFAULT_NO_TIMEOUT 0
#define DEFAULT_MAX_TIMEOUT 0
#define DEFAULT_MAX_RETRY 3
#define MAX_MONITOR_INTERVAL 1800 // 30 minutes

// connection
#define ACK 1
#define NAK 0
#define RESPONSE_TYPE_SIZE 1

// #define INVALID_RESPONSE "server problem"
// #define TIMEOUT_MSG "timeout resending..(%d/%d)\n"
// #define MAX_TIMEOUT_MSG "TLE bro"

// semantic
#define AT_LEAST_ONE_SEM_INVO 0
#define AT_MOST_ONE_SEM_INVO 1

// type
#define INT32_SIZE 4
#define INT64_SIZE 8
#define FLOAT_SIZE 4
#define CHAR_SIZE 1
#define BUFFER_SIZE 2048

// service
#define SERVICE_OPEN_ACCOUNT 1
#define SERVICE_CLOSE_ACCOUNT 2
#define SERVICE_DEPOSIT_WITHDRAW_MONEY 3
#define SERVICE_MONITOR_UPDATE 4
#define SERVICE_VIEW_ACCOUNT 5
#define SERVICE_TRANSFER_MONEY 6
#define SERVICE_CHANGE_PASSWORD 7
#define SERVICE_EXIT 8
#define SERVICE_MONITOR_LIVE 9
#define SERVICE_MONITOR_END 10

// Status codes
#define SUCCESS 0
#define INVALID_CREDENTIALS 1
#define ACCOUNT_NOT_FOUND 2
#define INSUFFICIENT_FUNDS 3
#define INVALID_AMOUNT 4
#define ACCOUNT_ALREADY_EXISTS 5
#define INTERNAL_ERROR 6
#define INVALID_SERVICE_ID 7


# endif
