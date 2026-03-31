
#ifndef SERVER_ACCOUNT_MANAGER_H
#define SERVER_ACCOUNT_MANAGER_H


#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

# include "server/Account.h"
# include "server/Constant.h"


using namespace std;


// The output is int, which denotes the status code of the operation.
// TODO: Define the status codes in the Constants file
class AccountManager{
    public:
        AccountManager();
        int addAccount(string username, string password, char currency, int32_t& accountN);
        int closeAccount(string username, int32_t accountNumber, string password);
        int validateAccount(Account account, string password);
        int depositWithdraw(string username, int32_t accountNumber, string password, char currency, int64_t amount, int64_t& newBalance);
        int transfer(string senderUsername, int32_t senderAccountNumber, string recipientUsername, int32_t recipientAccountNumber,
            char senderCurrency, char recipientCurrency, string password, int64_t amount, float& exchangeRate, int64_t& remainingBalance);
        int changePassword(string username, int32_t accountNumber, string password, string newPassword);
        int viewAccount(string username, int32_t accountNumber, string password, char& currency, int64_t& balance);
    
    private:
        int32_t accountIdCounter = 0; 
        std::vector<Account> accountStore;
        
        float exchangeRates[3][3] = {
            {1.0, 0.74, 0.67}, // SGD to SGD, USD, EUR
            {1.35, 1.0, 0.90}, // USD to SGD, USD, EUR
            {1.49, 1.11, 1.0}  // EUR to SGD, USD, EUR
        };

        Account& getAccount(string username, int32_t accountNumber);
        int accountNumberCheck(int32_t accountNumber);
        int exchangeCurrency(char fromCurrency, char toCurrency, int64_t from_amount, int64_t& to_amount, float& rate);
        

        string generateAccountNumber(size_t length);
};

#endif