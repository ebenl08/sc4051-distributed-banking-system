
# include "AccountManager.h"

// class AccountManager{
//     public:
//         AccountManager();
//         int addAccount(ccptr username, ccptr password, char currency);
//         int closeAccount(ccptr username, ccptr password, char currency);
//         int deposit(ccptr username, ccptr password, char currency, int64_t amount);
//         int withdraw(ccptr username, ccptr password, char currency, int64_t amount);
//         int transfer(ccptr username, ccptr password, char currency, int64_t amount, ccptr recipient);
//     private:
//         std::vector<Account> accounts;
//         Account getAccount(ccptr username);
//         int64_t exchangeCurrency(ccptr fromCurrency, ccptr toCurrency, int64_t amount);
// };

AccountManager::AccountManager(){
    this->accounts = std::vector<Account>();
}

int AccountManager::addAccount(cchar* username, cchar* password, cchar currency){
    Account account = Account(username, password, currency);
    this->accounts.push_back(account);
    return 0;
} 

// TODO: Implement cleaner Error handling and status codes, instead of just exiting the program
Account AccountManager::getAccount(cchar* username, cchar currency){
    try{
        if(this->accounts.empty())
            throw std::runtime_error("No accounts found");
        for(Account account : this->accounts){
            if(strcmp(account.getUsername(), username) == 0 
                && strcmp(account.getCurrency(), &currency) == 0
            )
                return account;
        }
        throw std::runtime_error("Account not found");
    } catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

int AccountManager::closeAccount(cchar* username, cchar* password, cchar currency){
    Account account = this->getAccount(username, currency);
    Account* accountPtr = &account;
    this->accounts.erase(
        std::remove(
            this->accounts.begin(), 
            this->accounts.end(), 
            account
        ), 
        this->accounts.end());
    delete accountPtr;
    accountPtr = nullptr;
    return 0;
} 

int AccountManager::deposit(cchar* username, cchar* password, cchar currency, int64_t amount){
    Account account = this->getAccount(username, currency);
    Account* accountPtr = &account;
    int64_t newBalance = account.getBalance() + amount;
    accountPtr->updateBalance(newBalance);
    return 0;
}

int AccountManager::withdraw(cchar* username, cchar* password, cchar currency, int64_t amount){
    Account account = this->getAccount(username, currency);
    Account* accountPtr = &account;
    int64_t newBalance = account.getBalance() - amount;
    accountPtr->updateBalance(newBalance);
    return 0;
}

int AccountManager::transfer(cchar* username, cchar* password, cchar senderCurrency, int64_t amount, cchar* recipient, cchar recipientCurrency){
    Account sender = this->getAccount(username, senderCurrency);
    if(validateAccount(sender, password) == 0){
        Account receiver = this->getAccount(recipient, recipientCurrency);
        // not sure if this is the good practice but let's just do this for now
        sender.updateBalance(sender.getBalance() - amount);
        
        int64_t exchangedAmount;
        exchangeCurrency(senderCurrency, recipientCurrency, amount, exchangedAmount);
        receiver.updateBalance(receiver.getBalance() + exchangedAmount);
        return 0;
    }
    return -1;

}

// TODO: Implement exchangeCurrency function
int AccountManager::exchangeCurrency(cchar fromCurrency, cchar toCurrency, int64_t from_amount, int64_t& to_amount){
    double exchangeRate = 1.0;
    to_amount = from_amount * exchangeRate;
    return 0;
}

int AccountManager::validateAccount(Account account, cchar* password){
    if(strcmp(account.getPassword(), password) == 0)
        return 0;
    return -1;
}

int main(){
    cchar SGD = 0;
    cchar USD = 1;
    
    AccountManager accountManager = AccountManager();
    accountManager.addAccount("user1", "password1", SGD);
    accountManager.addAccount("user1", "password1", USD);
    accountManager.addAccount("user2", "password2", USD);

    // 10000  means 100.00 dollars, since we are using int64_t to represent the balance in cents
    accountManager.deposit("user1", "password1", SGD, 10000);
    accountManager.deposit("user1", "password1", USD, 10000);
    accountManager.deposit("user2", "password2", USD, 10000);
    
    accountManager.transfer("user1", "password1", SGD, 5000, "user2", USD);
    accountManager.closeAccount("user1", "password1", SGD);
    delete &accountManager;

}