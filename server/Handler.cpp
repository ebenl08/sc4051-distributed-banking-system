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
        strcpy(respond, "ERROR: account creation failed");
        length = strlen(respond);
        return -1;
    }

    if(initialBalance > 0) {
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

    if(result == -10){
        strcpy(respond, "No accounts found");
        length = strlen(respond);
        return -1
    } else if(result == -11){
        strcpy(respond, "Account not found");
        length = strlen(respond);
        return -1
    }
    if(result == 0) {
        strcpy(respond, "SUCCESS: account closed");
        length = strlen(respond);
        return 0;
    } else if(result == -1){
        strcpy(respond, "Wrong password!");
        length = strlen(respond);
        return -1;
    } else {
        strcpy(respond, "ERROR: failed to close account");
        length = strlen(respond);
        return -1;
    }
}

int32_t Handler::service3(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;
    char currency = args[3].stringValue;
    int64_t amount = args[4].int64Value;

    int result = accountManager.depositWithdraw(username, accountNumber, password, currency, amount);

    if(result == -10){
        strcpy(respond, "No accounts found");
        length = strlen(respond);
        return -1
    } else if(result == -11){
        strcpy(respond, "Account not found");
        length = strlen(respond);
        return -1
    }
    if(result == -1) {
        strcpy(respond, "Not sufficient fund!");
        length = strlen(respond);
        return -1;
    } else {
        string msg;
        if(amount < 0){
            msg = "Withdrawal successful!\nAmount of withdrawal: " + to_string(amount*-1);
        } else {
            msg = "Deposit successful!\nAmount of deposit: " + to_string(amount);
        }
        strcpy(respond, msg.c_str());
        length = strlen(respond);
        return 0;
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
    
    if(result == -10){
        strcpy(respond, "No accounts found");
        length = strlen(respond);
        return -1
    } else if(result == -11){
        strcpy(respond, "Account not found");
        length = strlen(respond);
        return -1
    }
    if(result == 0) {
        string msg = "Transaction successful!\nAmount of money send to " + to_string(recipientUsername) + ": " + to_string(amount);
        strcpy(respond, msg.c_str());
        length = msg.length();
        return 0;
    } else if(result == -1) {
        strcpy(respond, "Transaction failed!");
        length = strlen(respond);
        return -1;
    }
}

int32_t Handler::service5(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;
    char currency;
    int64_t balance;

    int result = accountManager.viewAccount(username, accountNumber, password, currency, balance);
    if(result == -10){
        strcpy(respond, "No accounts found");
        length = strlen(respond);
        return -1
    } else if(result == -11){
        strcpy(respond, "Account not found");
        length = strlen(respond);
        return -1
    }
    string msg = "Account Number: " + to string(accountNumber) + "
                \nUsername: " + username + "
                \nCurrency: " + to_string(currency) + "
                \nBalance: " + to_string(balance);
    strcpy(respond, msg.c_str());
    length = msg.length();
    return 0;
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
    return 0;
}

int32_t Handler::service7(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].stringValue;
    int32_t accountNumber = args[1].int32Value;
    string password = args[2].stringValue;
    string newPassword = args[3].stringValue;
    
    result = accountManager.changePassword(username, accountNumber, password, newPassword);

    if(result == -10){
        strcpy(respond, "No accounts found");
        length = strlen(respond);
        return -1
    } else if(result == -11){
        strcpy(respond, "Account not found");
        length = strlen(respond);
        return -1
    }
    if(result == 0) {
        strcpy(respond, "SUCCESS: password has been changed.");
        length = strlen(respond);
        return 0;
    } else if (result == -1) {
        strcpy(respond, "ERROR: password change failed");
        length = strlen(respond);
        return -1;
    }
}