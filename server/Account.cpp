#include "Account.h"

// class Account{
    
//     public:
//         Account(const char* username, const char* password, const char currency);
//         const char* getUsername();
//         const char* getPassword();
//         const char* getCurrencyString();
//         const char getCurrency();
//         const int64_t getBalance();
//         const int64_t updateBalance(int64_t amount);
//     private:
//         const char* username;
//         const char* password;
//         const char currency;
//         const int64_t balance = 0;
        
// };

using namespace std;

Account::Account(int32_t accountNumber, string username, string password, char currency)
    : accountNumber(accountNumber), username(username), password(password), currency(currency) {
}

int32_t Account::getAccountNumber() const {
    return this->accountNumber;
}

const std::string Account::getUsername() const{
    return this->username;
}
const std::string Account::getPassword() const{
    return this->password;
}
const std::string Account::getCurrencyString() const{
    switch(this->currency){
        case SGD:
            return "SGD";
        case USD:
            return "USD";
        default:
            return "Unknown Currency";
    }
}
const char Account::getCurrency() const{
    return this->currency;
}
const int64_t Account::getBalance() const{
    return this->balance;
}
const std::string Account::getBalanceString() const{
    int64_t dollars = this->balance / 100;
    int64_t cents = this->balance % 100;
    return std::to_string(dollars) + "." + (cents < 10 ? "0" : "") + std::to_string(cents);
}
void Account::updateBalance(int64_t amount){
    // this is not the best way to do it but let's just do this for now
    this->balance = amount;
}
void Account::updatePassword(string newPassword){
    this->password = newPassword;
}


bool operator==(const Account& lhs, const Account& rhs){
    return lhs.username == rhs.username && lhs.currency == rhs.currency;
}

std::ostream& operator<<(std::ostream& os, const Account& account){
    
    os  << "Account Number: " << account.accountNumber
        << "Username: " << account.username 
        << ", Currency: " << account.getCurrencyString() 
        << ", Balance: " << account.getBalanceString();
    return os;
}
