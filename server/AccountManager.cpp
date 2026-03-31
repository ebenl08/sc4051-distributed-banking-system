
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
    this->accounts = vector<Account>();
}

int AccountManager::accountNumberCheck(int32_t accountNumber){
    for(Account& account : this->accounts){
        if(account.getAccountNumber() == accountNumber){
            return 1;
        }
    }
    return 0;
}

int AccountManager::addAccount(string username, string password, char currency, int32_t& accountN){
    int32_t accountNumber = 0;
    do{
        for(int i=0;i<6;i++){
            int x = rand() % 10;
            accountNumber += x;
            if(i != 5){accountNumber = accountNumber*10;}
        }
    } while(accountNumberCheck(accountNumber));
    accountN = accountNumber;
    Account* account = new Account(accountNumber, username, password, currency);
    this->accounts.push_back(*account);
    delete account;
    return SUCCESS;
} 

Account& AccountManager::getAccount(std::string username, int32_t accountNumber) {
    if (this->accounts.empty()) {
        throw ACCOUNT_NOT_FOUND;
    }

    for (Account& account : this->accounts) {
        if (account.getUsername() == username && account.getAccountNumber() == accountNumber) {
            return account;
        }
    }

    throw ACCOUNT_NOT_FOUND;
}

int AccountManager::closeAccount(string username, int32_t accountNumber, string password){
    try{
        Account& account = this->getAccount(username, accountNumber);
        if(account.getPassword() == password){
            this->accounts.erase(
                std::remove(this->accounts.begin(), this->accounts.end(), account),
                this->accounts.end()
            );
        } else{return INVALID_CREDENTIALS;}
        return SUCCESS;
    } catch (int err) {
        return err;
    }
} 

int AccountManager::depositWithdraw(string username, int32_t accountNumber, string password, char currency, int64_t amount){
    try {
        Account& account = this->getAccount(username, accountNumber);
        int64_t newBalance = account.getBalance() + amount;
        if(newBalance < 0){
            return INSUFFICIENT_FUNDS;
        }
        account.updateBalance(newBalance);
        return SUCCESS;
    } catch (int err) {
        return err;
    }
}

int AccountManager::transfer(string senderUsername, int32_t senderAccountNumber, string recipientUsername, int32_t recipientAccountNumber,
            char senderCurrency, char recipientCurrency, string password, int64_t amount){
    try {
        Account& sender = this->getAccount(senderUsername, senderAccountNumber);
        if(sender.getBalance() < amount){
            return INSUFFICIENT_FUNDS;
        }
        if(validateAccount(sender, password) == 0){
            Account& receiver = this->getAccount(recipientUsername, recipientAccountNumber);
            // not sure if this is the good practice but let's just do this for now
            sender.updateBalance(sender.getBalance() - amount);
            
            int64_t exchangedAmount;
            exchangeCurrency(senderCurrency, recipientCurrency, amount, exchangedAmount);
            receiver.updateBalance(receiver.getBalance() + exchangedAmount);
            return SUCCESS;
        }
        return INVALID_CREDENTIALS;
    } catch (int err) {
        return err;
    }
}

int AccountManager::changePassword(string username, int32_t accountNumber, string password, string newPassword){
    try {
        Account& account = this->getAccount(username, accountNumber);
        if(account.getPassword() == password){
            account.updatePassword(newPassword);
            return SUCCESS;
        } else {
            return INVALID_CREDENTIALS;
        }
    } catch (int err) {
        return err;
    }
}

int AccountManager::viewAccount(string username, int32_t accountNumber, string password, char& currency, int64_t& balance){
    try {
        Account& account = this->getAccount(username, accountNumber);
        currency = account.getCurrency();
        balance = account.getBalance();
        return SUCCESS;
    } catch (int err) {
        return err;
    }
}

int64_t AccountManager::getBalance(string username, int32_t accountNumber, string password, char currency){
    Account& account = getAccount(username, accountNumber);

    if (account.getPassword() != password) {
        return INVALID_CREDENTIALS; // or whatever you use
    }

    return account.getBalance();
}

// TODO: Implement exchangeCurrency function
int64_t AccountManager::exchangeCurrency(char fromCurrency, char toCurrency, int64_t from_amount, int64_t& to_amount){
    int64_t numerator = 0;
    int64_t denominator = 1;

    if(fromCurrency == toCurrency){
        to_amount = from_amount;
        return SUCCESS;
    }

    // SGD ↔ USD
    if(fromCurrency == 'S' && toCurrency == 'U'){
        numerator = 74;
        denominator = 100;
    } else if(fromCurrency == 'U' && toCurrency == 'S'){
        numerator = 135;
        denominator = 100;
    }

    // SGD ↔ EUR
    else if(fromCurrency == 'S' && toCurrency == 'E'){
        numerator = 68;
        denominator = 100;
    } else if(fromCurrency == 'E' && toCurrency == 'S'){
        numerator = 147;
        denominator = 100;
    }

    // USD ↔ EUR
    else if(fromCurrency == 'U' && toCurrency == 'E'){
        numerator = 92;
        denominator = 100;
    } else if(fromCurrency == 'E' && toCurrency == 'U'){
        numerator = 109;
        denominator = 100;
    }

    else{
        return INVALID_CREDENTIALS;
    }

    to_amount = (from_amount * numerator) / denominator;
    return SUCCESS;
}

int AccountManager::validateAccount(Account account, string password){
    if(account.getPassword() == password)
        return 0;
    return -1;
}

//int main(){
//   AccountManager* accountManager = new AccountManager();
//    accountManager->addAccount("user1", "password1", SGD);
//    accountManager->addAccount("user1", "password1", USD);
//   accountManager->addAccount("user2", "password2", USD);

    // 10000  means 100.00 dollars, since we are using int64_t to represent the balance in cents
//    accountManager->deposit("user1", "password1", SGD, 10000);
//    accountManager->deposit("user1", "password1", USD, 10000);
//    accountManager->deposit("user2", "password2", USD, 10000);
    
//    accountManager->transfer("user1", "password1", SGD, 5000, "user2", USD);
//    accountManager->closeAccount("user1", "password1", SGD);
//    delete accountManager;
//}