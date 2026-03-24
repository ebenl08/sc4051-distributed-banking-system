
#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H


#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector>

#include "Account.h"

#define cchar const char

// The output is int, which denotes the status code of the operation.
// TODO: Define the status codes in the Constants file
class AccountManager{
    public:
        AccountManager();
        int addAccount(cchar* username, cchar* password, cchar currency);
        int closeAccount(cchar* username, cchar* password, cchar currency);
        int validateAccount(Account account, cchar* password);
        int deposit(cchar* username, cchar* password, cchar currency, int64_t amount);
        int withdraw(cchar* username, cchar* password, cchar currency, int64_t amount);
        int transfer(cchar* username, cchar* password, cchar senderCurrency, 
            int64_t amount, 
            cchar* recipient, cchar recipientCurrency);

    private:
        std::vector<Account> accounts;
        Account getAccount(cchar* username, cchar currency);
        int exchangeCurrency(cchar fromCurrency, cchar toCurrency, int64_t from_amount, int64_t& to_amount);
};

#endif