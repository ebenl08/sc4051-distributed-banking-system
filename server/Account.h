
#ifndef ACCOUNT_H
#define ACCOUNT_H


#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>

#include <string>

#include "Constant.h"

using namespace std;

class Account{
    
    public:
        Account(int32_t accountNumber, string username, string password, char currency);
        int32_t getAccountNumber() const;
        const string getUsername() const;
        const string getPassword() const;
        const string getCurrencyString() const;
        const char getCurrency() const;
        const int64_t getBalance() const;
        const string getBalanceString() const;
        void updateBalance(int64_t amount);
        void updatePassword(string newPassword);

        friend bool operator==(const Account& lhs, const Account& rhs);
        friend ostream& operator<<(ostream& os, const Account& account);
    private:
        int32_t accountNumber;
        string username;
        string password;
        char currency;
        int64_t balance = 0;
        
};

#endif