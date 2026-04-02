#include "Handler.h"
#include "server/Helper.h"

#define argvec std::vector<Argument>

Handler::Handler(Socket& sock) : monitorManager(sock) {}

void Handler::notify_monitor_clients(const std::string& msg) {
    monitorManager.notifyAll(msg);
}

Response Handler::handle_service(Request request, const sockaddr_in& clientAddr) {
    int32_t statusCode = -1;
    argvec responseArgs;

    switch(request.serviceID) {
        case SERVICE_OPEN_ACCOUNT:
            responseArgs = open_account(request.args, statusCode); break;
        case SERVICE_CLOSE_ACCOUNT:
            responseArgs = close_account(request.args, statusCode); break;
        case SERVICE_DEPOSIT_WITHDRAW_MONEY:
            responseArgs = deposit_withdraw(request.args, statusCode); break;
        case SERVICE_VIEW_ACCOUNT:
            responseArgs = view_account(request.args, statusCode); break;
        case SERVICE_TRANSFER_MONEY:
            responseArgs = transfer(request.args, statusCode); break;  
        case SERVICE_CHANGE_PASSWORD:
            responseArgs = change_password(request.args, statusCode); break;
        case SERVICE_MONITOR_UPDATE:
            responseArgs = register_monitor_client(request.args, clientAddr, statusCode); break;
        default:
            statusCode = INVALID_SERVICE_ID;
    }
    
    Response res = Response();
    res.reqID = 0; 
    res.statusCode = statusCode;
    res.args = std::move(responseArgs);
    res.payloadLength = 0; // Must be calculated by the packet builder
    return res;
}

argvec Handler::open_account(const argvec& args, int32_t& statusCode) {
    argvec response;
    string username = args[0].value.stringValue;
    string password = args[1].value.stringValue;
    char currency = args[2].value.charValue;
    int64_t initialBalance = args[3].value.int64Value;
    int64_t newBalance = args[3].value.int64Value; // Reusing initialBalance to store new balance after deposit

    pinfo("CREATE NEW USER ACCOUNT");
    int32_t accountNumber;
    statusCode = accountManager.addAccount(username, password, currency, accountNumber);

    if (statusCode != SUCCESS) {
        if(statusCode == ACCOUNT_ALREADY_EXISTS){
            pinfo("ACCOUNT ALREADY EXISTS");
            response.push_back(Argument("Account with the same username and currency already exists!"));
        } else {
            pinfo("FAILED TO CREATE ACCOUNT");
            response.push_back(Argument("Failed to create account!"));
        }
        return response;
    } 

    if (initialBalance > 0) {
        accountManager.depositWithdraw(username, accountNumber, password, currency, initialBalance, newBalance); // Reusing accountNumber to store new balance
    }

    response.push_back(Argument(accountNumber));
    pdebug("Account created successfully with account number: %d", accountNumber);
    notify_monitor_clients("[CREATE ACCOUNT] Account ID: " + to_string(accountNumber));
    return response;
}

argvec Handler::close_account(const argvec& args, int32_t& statusCode) {
    argvec response;
    string name = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;

    statusCode = accountManager.closeAccount(name, accountNumber, password);

    if (statusCode == SUCCESS) {
        pinfo("CLOSE USER ACCOUNT");
        notify_monitor_clients("[CLOSE ACCOUNT] Account ID:" + to_string(accountNumber));
    } else if (statusCode == ACCOUNT_NOT_FOUND) {
        pinfo("ACCOUNT NOT FOUND");
        response.push_back(Argument("Account not found!"));
    } else if (statusCode == INVALID_CREDENTIALS) {
        pinfo("INVALID CREDENTIAL");
        response.push_back(Argument("Wrong password!"));
    } else {
        response.push_back(Argument("Internal error!"));
    }

    return response;
}

argvec Handler::deposit_withdraw(const argvec& args, int32_t& statusCode) {
    argvec response;
    string username = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;
    char currency = args[3].value.charValue;
    int64_t amount = args[4].value.int64Value;

    int64_t newBalance = 0;
    string action = amount < 0 ? "WITHDRAWAL" : "DEPOSIT";

    pinfo("BEGIN BALANCE %s", action.c_str());
    statusCode = accountManager.depositWithdraw(username, accountNumber, password, currency, amount, newBalance);

    if (statusCode == ACCOUNT_NOT_FOUND) {
        pinfo("ACCOUNT NOT FOUND");
        response.push_back(Argument("Account not found!"));
    } else if (statusCode == INSUFFICIENT_FUNDS) {
        pinfo("INSUFFICIENT BALANCE");
        response.push_back(Argument("Insufficient balance!"));
    } else if (statusCode == SUCCESS) {
        pinfo("NEW BALANCE: %lld", newBalance);
        response.push_back(Argument(newBalance));

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << (amount / 100.0f);
        notify_monitor_clients("[BALANCE " + action + "] Account ID: " + to_string(accountNumber) + " | AMOUNT: " + oss.str());
    } else {
        response.push_back(Argument("TRANSACTION FAILED!"));
    }
    
    return response;
}

argvec Handler::transfer(const argvec& args, int32_t& statusCode) {
    argvec response;
    string senderUser = args[0].value.stringValue;
    int32_t senderAcc = args[1].value.int32Value;
    string recipientUser = args[2].value.stringValue;
    int32_t recipientAcc = args[3].value.int32Value;
    char senderCurr = args[4].value.charValue;
    char recipientCurr = args[5].value.charValue;
    string password = args[6].value.stringValue;
    int64_t amount = args[7].value.int64Value;

    float exchangeRate;
    int64_t remainingBalance;

    pinfo("TRANSFER MONEY");
    statusCode = accountManager.transfer(
            senderUser, senderAcc, 
            recipientUser, recipientAcc, 
            senderCurr, recipientCurr, 
            password, amount, 
            exchangeRate, remainingBalance
        );
    
    if (statusCode == SUCCESS) {
        response.push_back(Argument(recipientUser));
        response.push_back(Argument(recipientAcc));
        response.push_back(Argument(recipientCurr));
        response.push_back(Argument(exchangeRate));
        response.push_back(Argument((int64_t) remainingBalance));
    } else {
        response.push_back(Argument("TRANSFER FAILED"));
    }

    std::ostringstream oss_amount, oss_exchange, oss_final, oss_balance;
    oss_amount << std::fixed << std::setprecision(2) << (amount / 100.0f);
    oss_exchange << std::fixed << std::setprecision(2) << exchangeRate;
    oss_final << std::fixed << std::setprecision(2) << ((amount * exchangeRate) / 100.0f);
    oss_balance << std::fixed << std::setprecision(2) << (remainingBalance / 100.0f);

    std::string info = "[MONEY TRANSFER] FROM: " + to_string(senderAcc)
                 + " TO: " + to_string(recipientAcc)
                 + "\n | AMOUNT: " + currencyToString(senderCurr) + " " + oss_amount.str()
                 + "\n | EXCHANGE RATE: " + oss_exchange.str()
                 + "\n | FINAL AMOUNT: " + currencyToString(recipientCurr) + " " + oss_final.str()
                 + "\n | REMAINING BALANCE: " + currencyToString(senderCurr) + " " + oss_balance.str();

    notify_monitor_clients(info);
    return response;
}

argvec Handler::view_account(const argvec& args, int32_t& statusCode) {
    argvec response;
    string username = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;
    char currency;
    int64_t balance;

    pinfo("VIEW ACCOUNT & RETRIEVE DATA");
    statusCode = accountManager.viewAccount(username, accountNumber, password, currency, balance);

    if (statusCode == SUCCESS) {
        response.push_back(Argument(accountNumber));
        response.push_back(Argument(username));
        response.push_back(Argument(currency));
        response.push_back(Argument(balance));
    } else {
        response.push_back(Argument("Could not retrieve balance."));
    }

    return response;
}

argvec Handler::register_monitor_client(const argvec& args, const sockaddr_in& clientAddr, int32_t& statusCode) {
    argvec response;
    int32_t interval = args[0].value.int32Value;

    if (interval <= 0 || interval > MAX_MONITOR_INTERVAL) {
        statusCode = -1;
        response.push_back(Argument("Invalid monitor interval"));
        return response;
    }
    pinfo("MONITOR SESSION");
    monitorManager.registerClient(clientAddr, interval);
    statusCode = SUCCESS;
    return response;
}

argvec Handler::change_password(const argvec& args, int32_t& statusCode) {
    argvec response;
    string username = args[0].value.stringValue;
    int32_t accountNumber = args[1].value.int32Value;
    string password = args[2].value.stringValue;
    string newPassword = args[3].value.stringValue;

    pinfo("CHANGED PASSWORD");
    statusCode = accountManager.changePassword(username, accountNumber, password, newPassword);

    if (statusCode != SUCCESS) {
        response.push_back(Argument("Password change failed."));
    }

    notify_monitor_clients("[CHANGE PASSWORD] Account ID: " + to_string(accountNumber));

    return response;
}