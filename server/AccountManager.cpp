
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

using namespace std;

AccountManager::AccountManager(){
    this->accounts = std::vector<Account>();
}

int AccountManager::addAccount(string username, string password, char currency){
    Account* account = new Account(username, password, currency);
    this->accounts.push_back(*account);
    delete account;
    return 0;
} 

// TODO: Implement cleaner Error handling and status codes, instead of just exiting the program
Account& AccountManager::getAccount(string username, char currency){
    try{
        if(this->accounts.empty())
            throw std::runtime_error("No accounts found");
        for(Account& account : this->accounts){
            if(account.getUsername() == username && account.getCurrency() == currency){
                if(DEBUG) cout << "Account found: " << account << "----> at" << &account << endl;
                return account;
            }
        }
        throw std::runtime_error("Account not found");
    } catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

int AccountManager::closeAccount(string username, string password, char currency){
    Account& account = this->getAccount(username, currency);
    if(DEBUG) cout << "Delete Account: " << account << "----> at" << &account << endl;
    this->accounts.erase(
        std::remove(
            this->accounts.begin(), 
            this->accounts.end(), 
            account
        ), 
        this->accounts.end());
    return 0;
} 

int AccountManager::deposit(string username, string password, char currency, int64_t amount){
    Account& account = this->getAccount(username, currency);
    int64_t newBalance = account.getBalance() + amount;
    account.updateBalance(newBalance);
    if(DEBUG) cout << "Deposit: " << account << "----> at" << &account << endl;
    return 0;
}

int AccountManager::withdraw(string username, string password, char currency, int64_t amount){
    Account& account = this->getAccount(username, currency);
    int64_t newBalance = account.getBalance() - amount;
    account.updateBalance(newBalance);
    if(DEBUG) cout << "Withdraw: " << account << "----> at" << &account << endl;
    return 0;
}

int AccountManager::transfer(string username, string password, char senderCurrency, 
    int64_t amount, string recipient, char recipientCurrency){

    Account& sender = this->getAccount(username, senderCurrency);
    if(validateAccount(sender, password) == 0){
        Account& receiver = this->getAccount(recipient, recipientCurrency);
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
int AccountManager::exchangeCurrency(char fromCurrency, char toCurrency, int64_t from_amount, int64_t& to_amount){
    double exchangeRate = 1.0;
    to_amount = from_amount * exchangeRate;
    return 0;
}

int AccountManager::validateAccount(Account account, string password){
    if(account.getPassword() == password)
        return 0;  
    return -1;
}

int main(){
    AccountManager* accountManager = new AccountManager();
    accountManager->addAccount("user1", "password1", SGD);
    accountManager->addAccount("user1", "password1", USD);
    accountManager->addAccount("user2", "password2", USD);

    // 10000  means 100.00 dollars, since we are using int64_t to represent the balance in cents
    accountManager->deposit("user1", "password1", SGD, 10000);
    accountManager->deposit("user1", "password1", USD, 10000);
    accountManager->deposit("user2", "password2", USD, 10000);
    
    accountManager->transfer("user1", "password1", SGD, 5000, "user2", USD);
    accountManager->closeAccount("user1", "password1", SGD);
    delete accountManager;
}