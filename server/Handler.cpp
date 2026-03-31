#include "Handler.h"

Handler::Handler() {}

void Handler::setCurrentClientAddr(const sockaddr_in& clientaddr) {
    currentClientAddr = clientaddr;
    hasCurrentClientAddr = true;
}

void Handler::setSocket(Socket* sock) {
    serverSocket = sock;
}

void Handler::notifyMonitorClients(const std::string& message) {
    if (serverSocket != nullptr) {
        monitorManager.notifyAll(*serverSocket, message);
    }
}

int32_t Handler::service1(const vector<Argument>& args, char* respond, int& length){
    string username = args[0].value.stringValue;
    string password = args[1].value.stringValue;
    char currency = args[2].value.charValue;
    int64_t initialBalance = args[3].value.int64Value;
    int32_t accountNumber;
    
    int result = accountManager.addAccount(username, password, currency, accountNumber);

    int counter = 0;

    if(result != SUCCESS){
        const char* msg = "Account creation failed!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return result;
    }

    if(initialBalance > 0){
        accountManager.depositWithdraw(username, accountNumber, password, currency, initialBalance);
    }
    
    string msg = to_string(accountNumber);
    Marshalling::marshal(respond + counter, msg.c_str());
    counter += 4 + msg.length();
    length = counter;

    string msgMonitor = "Account created: " + to_string(accountNumber);
    notifyMonitorClients(msgMonitor);

    cout << "[service1] accountNumber = " << accountNumber << endl;
    cout << "[service1] msg = " << msg << endl;
    cout << "[service1] length = " << length << endl;

    return SUCCESS;
}

int32_t Handler::service2(const std::vector<Argument>& args, char* respond, int& length){
    string name = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;

    int result = accountManager.closeAccount(name, accountNumber, password);

    int counter = 0;

    if(result == SUCCESS){
        length = 0;
        string msgMonitor = "Account closed: " + to_string(accountNumber);
        notifyMonitorClients(msgMonitor);
        return SUCCESS;
    } else if(result == ACCOUNT_NOT_FOUND){
        const char* msg = "Account not found!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return ACCOUNT_NOT_FOUND;
    } else if(result == INVALID_CREDENTIALS){
        const char* msg = "Wrong password!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return INVALID_CREDENTIALS;
    }

    const char* msg = "Internal error!";
    Marshalling::marshal(respond + counter, msg);
    counter += 4 + strlen(msg);
    length = counter;
    return INTERNAL_ERROR;
}

int32_t Handler::service3(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;
    char currency = args[3].value.charValue;
    int64_t amount = args[4].value.int64Value;

    int result = accountManager.depositWithdraw(username, accountNumber, password, currency, amount);

    int counter = 0;

    if(result == ACCOUNT_NOT_FOUND){
        const char* msg = "Account not found!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return ACCOUNT_NOT_FOUND;
    } else if(result == INSUFFICIENT_FUNDS){
        const char* msg = "Insufficient balance!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return INSUFFICIENT_FUNDS;
    }

    string msg;
    string msgMonitor;
    if(amount < 0){
        msg = "Amount of withdrawal: " + to_string(-amount);
        msgMonitor = "Account " + to_string(accountNumber) + " withdrawed: " + to_string(amount);
    } else {
        msg = "Amount of deposit: " + to_string(amount);
        msgMonitor = "Account " + to_string(accountNumber) + " deposited: " + to_string(amount);
    }

    int64_t newBalance = accountManager.getBalance(username, accountNumber, password, currency);

    Marshalling::marshal(respond + counter, msg.c_str());
    counter += 4 + msg.length();

    Marshalling::marshal(respond + counter, newBalance);
    counter += 8;

    length = counter;
    
    notifyMonitorClients(msgMonitor);

    return SUCCESS;
}

int32_t Handler::service4(const std::vector<Argument>& args, char* respond, int& length){
    string senderUsername = args[0].value.stringValue;
    int32_t senderAccountNumber = args[1].value.int32Value;
    string recipientUsername = args[2].value.stringValue;
    int32_t recipientAccountNumber = args[3].value.int32Value;
    char senderCurrency = args[4].value.charValue;
    char recipientCurrency = args[5].value.charValue;
    string password = args[6].value.stringValue;
    int64_t amount = args[7].value.int64Value;

    int result = accountManager.transfer(senderUsername, senderAccountNumber, recipientUsername, recipientAccountNumber, senderCurrency, 
    recipientCurrency, password, amount);
    
    int counter = 0;

    if(result == SUCCESS){
        string msg = "Amount of money sent to " + recipientUsername + ": " + to_string(amount);

        Marshalling::marshal(respond + counter, recipientUsername.c_str());
        counter += 4 + recipientUsername.length();

        Marshalling::marshal(respond + counter, recipientAccountNumber);
        counter += 4;

        Marshalling::marshal(respond + counter, amount);
        counter += 8;

        Marshalling::marshal(respond + counter, msg.c_str());
        counter += 4 + msg.length();

        length = counter;
        return SUCCESS;
    } else if(result == ACCOUNT_NOT_FOUND){
        const char* msg = "Account not found!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return ACCOUNT_NOT_FOUND;
    } else if(result == INVALID_CREDENTIALS){
        const char* msg = "Wrong password!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return INVALID_CREDENTIALS;
    } else if(result == INSUFFICIENT_FUNDS){
        const char* msg = "Insufficient balance!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return INSUFFICIENT_FUNDS;
    }

    const char* msg = "Internal error!";
    Marshalling::marshal(respond + counter, msg);
    counter += 4 + strlen(msg);
    length = counter;
    return INTERNAL_ERROR;
}

int32_t Handler::service5(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;
    char currency;
    int64_t balance;

    int result = accountManager.viewAccount(username, accountNumber, password, currency, balance);

    int counter = 0;

    if(result == SUCCESS){
        Marshalling::marshal(respond + counter, balance);
        counter += 8;

        length = counter;
        return SUCCESS;
    } else if(result == ACCOUNT_NOT_FOUND){
        const char* msg = "Account not found!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);

        length = counter;
        return ACCOUNT_NOT_FOUND;
    } else if(result == INVALID_CREDENTIALS){
        const char* msg = "Wrong password!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);

        length = counter;
        return INVALID_CREDENTIALS;
    }

    const char* msg = "Internal error!";
    Marshalling::marshal(respond + counter, msg);
    counter += 4 + strlen(msg);

    length = counter;
    return INTERNAL_ERROR;
}

int32_t Handler::service6(const std::vector<Argument>& args, char* respond, int& length) {
    int32_t interval = args[0].value.int32Value;
    int counter = 0;

    if (interval <= 0) {
        const char* msg = "ERROR: invalid monitor interval";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return -1;
    }

    if (!hasCurrentClientAddr) {
        const char* msg = "ERROR: client address not set";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return -1;
    }

    monitorManager.registerClient(currentClientAddr, interval);

    length = 0;
    return SUCCESS;
}

int32_t Handler::service7(const std::vector<Argument>& args, char* respond, int& length){
    string username = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;
    string newPassword = args[3].value.stringValue;
    
    int result = accountManager.changePassword(username, accountNumber, password, newPassword);

    int counter = 0;

    if(result == SUCCESS){
        length = 0;
        return SUCCESS;
    } else if(result == ACCOUNT_NOT_FOUND){
        const char* msg = "Account not found!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return ACCOUNT_NOT_FOUND;
    } else if(result == INVALID_CREDENTIALS){
        const char* msg = "Wrong password!";
        Marshalling::marshal(respond + counter, msg);
        counter += 4 + strlen(msg);
        length = counter;
        return INVALID_CREDENTIALS;
    }

    const char* msg = "Internal error!";
    Marshalling::marshal(respond + counter, msg);
    counter += 4 + strlen(msg);
    length = counter;
    return INTERNAL_ERROR;
}