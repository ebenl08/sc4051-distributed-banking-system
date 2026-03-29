#include "Handler.h"

void Handler::setCurrentClientAddr(const sockaddr_in& clientaddr) {
    currentClientAddr = clientaddr;
    hasCurrentClientAddr = true;
}

int32_t Handler::service1(const vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    string password = args[1].stringValue;
    char currency = args[2].stringValue[0];
    int64_t initialBalance = args[3].int64Value;
    int32_t accountNumber;
    
    int result = accountManager.addAccount(username, password, currency, accountNumber);
    
    if(result != 0) {
        strcpy(respond, "Current balance: " + account.getBalanceString);
        length = strlen(respond);
        return INSUFFICIENT_FUNDS;
    }

    if(initialBalance > 0) {
        accountManager.deposit(username, password, currency, initialBalance);
    }

    string msg = + to_string(accountNumber);
    strcpy(respond, msg.c_str());
    length = msg.length();
    return SUCCESS;
}

int32_t Handler::service2(const std::vector<Argument>& args, char* respond, int& length){
    string name = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;

    int result = accountManager.closeAccount(name, accountNumber, password);

    if(result == ACCOUNT_NOT_FOUND){
        strcpy(respond, "");
        length = strlen(respond);
        return ACCOUNT_NOT_FOUND;
    }
    if(result == SUCCESS) {
        strcpy(respond, "");
        length = strlen(respond);
        return SUCCESS;
    } else if(result == INVALID_CREDENTIALS){
        strcpy(respond, "Wrong password!");
        length = strlen(respond);
        return INVALID_CREDENTIALS;
    } else {
        strcpy(respond, "ERROR: failed to close account");
        length = strlen(respond);
        return INTERNAL_ERROR;
    }
}

int32_t Handler::service3(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;
    char currency = args[3].stringValue;
    int64_t amount = args[4].int64Value;

    int result = accountManager.depositWithdraw(username, accountNumber, password, currency, amount);

    if(result == ACCOUNT_NOT_FOUND){
        strcpy(respond, "");
        length = strlen(respond);
        return ACCOUNT_NOT_FOUND;
    }
    if(result == INSUFFICIENT_FUNDS) {
        strcpy(respond, "Current balance: " + account.getBalanceString);
        length = strlen(respond);
        return INSUFFICIENT_FUNDS;
    } else {
        string msg;
        if(amount < 0){
            msg = "Amount of withdrawal: " + to_string(amount*-1);
        } else {
            msg = "Amount of deposit: " + to_string(amount);
        }
        strcpy(respond, msg.c_str());
        length = strlen(respond);
        return SUCCESS;
    }
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
    
    if(result == ACCOUNT_NOT_FOUND){
        strcpy(respond, "");
        length = strlen(respond);
        return ACCOUNT_NOT_FOUND;
    }
    if(result == SUCCESS) {
        string msg = "Amount of money send to " + to_string(recipientUsername) + ": " + to_string(amount);
        strcpy(respond, msg.c_str());
        length = msg.length();
        return SUCCESS;
    } else if(result == INVALID_CREDENTIALS) {
        strcpy(respond, "Wrong password!");
        length = strlen(respond);
        return INVALID_CREDENTIALS;
    }
}

int32_t Handler::service5(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;
    char currency;
    int64_t balance;

    int result = accountManager.viewAccount(username, accountNumber, password, currency, balance);
    if(result == ACCOUNT_NOT_FOUND){
        strcpy(respond, "");
        length = strlen(respond);
        return ACCOUNT_NOT_FOUND;
    }
    string msg = "Account Number: " + to string(accountNumber) + "
                \nUsername: " + username + "
                \nCurrency: " + to_string(currency) + "
                \nBalance: " + to_string(balance);
    strcpy(respond, msg.c_str());
    length = msg.length();
    return SUCCESS;
}

int32_t Handler::service6(const std::vector<Argument>& args, char* respond, int& length){
    int32_t interval = args[0].int32Value;

    if(interval <= 0){
        strcpy(respond, "ERROR: invalid monitor interval");
        length = strlen(respond);
        return -1;
    }

    if(!hasCurrentClientAddr){
        strcpy(respond, "ERROR: client address not set");
        length = strlen(respond);
        return -1;
    }

    monitorManager.registerClient(currentClientAddr, interval);

    strcpy(respond, "SUCCESS: monitor registered");
    length = strlen(respond);
    return SUCCESS;
}

int32_t Handler::service7(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;
    string newPassword = args[3].stringValue;
    
    result = accountManager.changePassword(username, accountNumber, password, newPassword);

    if(result == ACCOUNT_NOT_FOUND){
        strcpy(respond, "");
        length = strlen(respond);
        return ACCOUNT_NOT_FOUND;
    }
    if(result == SUCCESS) {
        strcpy(respond, "");
        length = strlen(respond);
        return SUCCESS;
    } else if (result == INVALID_CREDENTIALS) {
        strcpy(respond, "Wrong password!");
        length = strlen(respond);
        return INVALID_CREDENTIALS;
    }
}