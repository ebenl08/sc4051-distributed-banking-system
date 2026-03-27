#include "Handler.h"

int32_t Handler::service1(const std::vector<Argument>& args, char* respond, int& length){
    std::string username = args[0].stringValue;
    std::string password = args[1].stringValue;
    char currency = args[2].stringValue[0];
    int64_t initialBalance = args[3].int64Value;

    int result = accountManager.addAccount(username, password, currency);

    if (result != 0) {
        strcpy(respond, "ERROR: account creation failed");
        length = strlen(respond);
        return 1;
    }

    // ✅ deposit initial balance
    if (initialBalance > 0) {
        accountManager.deposit(username, password, currency, initialBalance);
    }

    strcpy(respond, "SUCCESS: account created");
    length = strlen(respond);
    return 0;
}

int32_t Handler::service2(const std::vector<Argument>& args, char* respond, int& length){
    std::string username = args[0].stringValue;
    std::string password = args[1].stringValue;
    char currency = args[2].stringValue[0];

    int result = accountManager.closeAccount(username, password, currency);

    if (result == 0) {
        strcpy(respond, "SUCCESS: account closed");
        length = strlen(respond);
        return 0;
    } else if (result == -1) {
        strcpy(respond, "ERROR: account not found");
        length = strlen(respond);
        return 1;
    } else if (result == -2) {
        strcpy(respond, "ERROR: wrong password");
        length = strlen(respond);
        return 1;
    } else if (result == -3) {
        strcpy(respond, "ERROR: balance must be zero before closing");
        length = strlen(respond);
        return 1;
    }

    strcpy(respond, "ERROR: unknown failure");
    length = strlen(respond);
    return 1;
}

int32_t Handler::service3(const std::vector<Argument>& args, char* respond, int& length){
    
}

int32_t Handler::service4(const std::vector<Argument>& args, char* respond, int& length){
    
}

int32_t Handler::service5(const std::vector<Argument>& args, char* respond, int& length){
    
}

int32_t Handler::service6(const std::vector<Argument>& args, char* respond, int& length){
    
}

int32_t Handler::service7(const std::vector<Argument>& args, char* respond, int& length){
    
}