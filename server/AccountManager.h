
#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H


#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>

# include "Account.h"
# include "Constant.h"

using namespace std;

// The output is int, which denotes the status code of the operation.
// TODO: Define the status codes in the Constants file
class AccountManager{
    public:
        AccountManager();
        int addAccount(string username, string password, char currency, int32_t& accountN);
        int closeAccount(string username, int32_t accountNumber string password);
        int validateAccount(Account account, string password);
        int deposit(string username, int32_t accountNumber, string password, char currency, int64_t amount);
        int withdraw(string username, string password, char currency, int64_t amount);
        int transfer(string senderUsername, int32_t senderAccountNumber, string recipientUsername, int32_t recipientAccountNumber,
            char senderCurrency, char recipientCurrency, string password, int64_t amount);

    private:
        std::vector<Account> accounts;
        Account& getAccount(string username, char currency);
        int AccountManager::accountNumberCheck(int32_t accountNumber);
        int exchangeCurrency(char fromCurrency, char toCurrency, int64_t from_amount, int64_t& to_amount);
};

#endif