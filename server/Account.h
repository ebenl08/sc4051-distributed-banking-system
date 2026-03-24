
#ifndef ACCOUNT_H
#define ACCOUNT_H


#include <cstdio>
#include <cstdlib>          // for exit() function
#include <cstring>
#include <unistd.h>
#include <iostream>



class Account{
    
    public:
        Account(const char* username, const char* password, const char currency);
        const char* getUsername();
        const char* getPassword();
        const char* getCurrencyString();
        const char* getCurrency();
        const int64_t getBalance();
        const int64_t updateBalance(int64_t amount);
    private:
        const char* username;
        const char* password;
        const char currency;
        const int64_t balance = 0;
        
};

#endif