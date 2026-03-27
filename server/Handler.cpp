#include "Handler.h"

int32_t Handler::service1(const vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    string password = args[1].stringValue;
    char currency = args[2].stringValue[0];
    int64_t initialBalance = args[3].int64Value;
    int32_t accountNumber;
    
    int result = accountManager.addAccount(username, password, currency, accountNumber);

    if (result != 0) {
        strcpy(respond, "ERROR: account creation failed");
        length = strlen(respond);
        return -1;
    }

    if (initialBalance > 0) {
        accountManager.deposit(username, password, currency, initialBalance);
    }

    string msg = "SUCCESS: account created\nYour account number: " + to_string(accountNumber);
    strcpy(respond, msg.c_str());
    length = msg.length();
    return 0;
}

int32_t Handler::service2(const std::vector<Argument>& args, char* respond, int& length){
    string name = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;

    int result = accountManager.closeAccount(name, accountNumber, password);

    if (result == 0) {
        strcpy(respond, "SUCCESS: account closed");
        length = strlen(respond);
        return 0;
    } else if (result == -1){
        strcpy(respond, "ERROR: wrong password");
        length = strlen(respond);
        return -1;
    } else {
        strcpy(respond, "ERROR: failed to close account");
        length = strlen(respond);
        return -1;
    }
}

int32_t Handler::service3(const std::vector<Argument>& args, char* respond, int& length){
    
}

int32_t Handler::service4(const std::vector<Argument>& args, char* respond, int& length){
    string senderUsername = args[0].stringValue;
    int32_t senderAccountNumber = args[2].int32Value;
    string recipientUsername = args[2].stringValue;
    int32_t recipientAccountNumber = args[3].int32Value;
    char senderCurrency = args[4].stringValue;
    char recipientCurrency = args[5].stringValue;
    string password = args[6].stringValue;
    int64_t amount = args[7].int64Value;

    int result = accountManager.transfer(senderUsername, senderAccountNumber, recipientUsername, recipientAccountNumber, senderCurrency, 
    recipientCurrency, password, amount);
    
    if (result == 0) {
        strcpy(respond, "SUCCESS: transaction is succesful");
        length = strlen(respond);
        return 0;
    } else if (result == -1){
        strcpy(respond, "ERROR: failed transaction");
        length = strlen(respond);
        return -1;
    }
}

int32_t Handler::service5(const std::vector<Argument>& args, char* respond, int& length){
    
}

int32_t Handler::service6(const std::vector<Argument>& args, char* respond, int& length){
    
}

int32_t Handler::service7(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;
    string newPassword = args[3].stringValue;
    
    result = accountManager.changePassword(username, accountNumber, password, newPassword);
    if (result == 0) {
        strcpy(respond, "SUCCESS: password has been changed.");
        length = strlen(respond);
        return 0;
    } else if (result == -1){
        strcpy(respond, "ERROR: password change failed");
        length = strlen(respond);
        return -1;
    }
}