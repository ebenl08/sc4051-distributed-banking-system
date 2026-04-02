

#ifndef SERVER_PROTOCOL_SCHEMA_H
#define SERVER_PROTOCOL_SCHEMA_H

#include <cstdio>
#include <cstdlib>          
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector> 

#include "server/Constant.h"
#include "server/protocol/Args.h"

// Service schema struct to define the expected input and output argument types for each service ID

struct ServiceSchema {
    std::vector<DataType> inputTypes;
    std::vector<DataType> outputTypes;
    std::vector<DataType> errorTypes = {
        TYPE_STRING // error message
    };
};

// build schema based on service ID for request parsing and response building

class SchemaFactory {
    public:
        static ServiceSchema generateSchema(int serviceID) {
            switch(serviceID) {
                case SERVICE_OPEN_ACCOUNT:
                    return ServiceSchema{
                        std::vector<DataType>{
                            TYPE_STRING, // username
                            TYPE_STRING, // password
                            TYPE_CHAR,   // currency
                            TYPE_INT64   // initial balance
                        },
                        std::vector<DataType>{
                            TYPE_INT32  // account number
                        }
                    };
                case SERVICE_CLOSE_ACCOUNT:
                    return ServiceSchema{
                        std::vector<DataType>{
                            TYPE_STRING, // username
                            TYPE_INT32,  // account number
                            TYPE_STRING  // password
                        },
                        std::vector<DataType>{}
                    };
                case SERVICE_DEPOSIT_WITHDRAW_MONEY:
                    return ServiceSchema{
                        std::vector<DataType>{
                            TYPE_STRING, // username
                            TYPE_INT32,  // account number
                            TYPE_STRING, // password
                            TYPE_CHAR,   // currency
                            TYPE_INT64   // amount
                        },
                        std::vector<DataType>{
                            TYPE_INT64  // new balance
                        }
                    };
                case SERVICE_VIEW_ACCOUNT:
                    return ServiceSchema{
                        std::vector<DataType>{
                            TYPE_STRING, // username
                            TYPE_INT32,  // account#
                            TYPE_STRING  // password
                        },
                        std::vector<DataType>{
                            TYPE_STRING, // account#
                            TYPE_STRING, // name
                            TYPE_CHAR,   // currency
                            TYPE_INT64   // balance
                        }
                    };
                case SERVICE_TRANSFER_MONEY:
                    return ServiceSchema{
                        std::vector<DataType>{
                            TYPE_STRING, // sender username
                            TYPE_INT32,  // sender account number
                            TYPE_STRING, // recipient username
                            TYPE_INT32,  // recipient account number
                            TYPE_CHAR,   // sender currency
                            TYPE_CHAR,   // recipient currency
                            TYPE_STRING, // password
                            TYPE_INT64   // amount
                        },
                        std::vector<DataType>{
                            TYPE_STRING, // recipient name
                            TYPE_INT32,  // recipient account#
                            TYPE_CHAR,   // recipient currency
                            TYPE_FLOAT, // exchange rate
                            TYPE_INT64   // recipient new balance
                        }
                    };
                case SERVICE_CHANGE_PASSWORD:
                    return ServiceSchema{
                        std::vector<DataType>{
                            TYPE_STRING, // username
                            TYPE_INT32,   // account number
                            TYPE_STRING, // old password
                            TYPE_STRING  // new password
                        },
                        std::vector<DataType>{}
                    };
                case SERVICE_MONITOR_UPDATE:
                        return ServiceSchema{
                            std::vector<DataType>{
                                TYPE_INT32  // monitor interval
                            },
                            std::vector<DataType>{}
                        };
                default:
                    return ServiceSchema{
                        std::vector<DataType>{},
                        std::vector<DataType>{}
                    };
                }
        }
};

#endif