
#ifndef ACCOUNT_H
#define ACCOUNT_H


#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>

#include <string>

#include "server/Constant.h"

class Account{
    
    public:
        Account(int32_t accountNumber, std::string username, std::string password, char currency);
        int32_t getAccountNumber() const;
        const std::string getUsername() const;
        const std::string getPassword() const;
        const std::string getCurrencyString() const;
        const char getCurrency() const;
        const int64_t getBalance() const;
        const std::string getBalanceString() const;
        void updateBalance(int64_t amount);
        void updatePassword(std::string newPassword);

        friend bool operator==(const Account& lhs, const Account& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Account& account);
    private:
        int32_t accountNumber;
        std::string username;
        std::string password;
        char currency;
        int64_t balance = 0;
        
};

#endif