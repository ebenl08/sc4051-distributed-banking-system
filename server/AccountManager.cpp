
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
//         std::vector<Account> accountStore;
//         Account getAccount(ccptr username);
//         int64_t exchangeCurrency(ccptr fromCurrency, ccptr toCurrency, int64_t amount);
// };

using namespace std;

AccountManager::AccountManager(){
    this->accountStore = std::vector<Account>();
}

Account& AccountManager::getAccount(std::string username, int32_t accountNumber) {
    try {
        if (this->accountStore.empty()) {
            throw ACCOUNT_NOT_FOUND;
        }
        for (Account& account : this->accountStore) {
            if (account.getUsername() == username && account.getAccountNumber() == accountNumber) {
                return account;
            }
        }throw ACCOUNT_NOT_FOUND;
    } catch (int err) {
        throw INTERNAL_ERROR;
    }
}

int32_t AccountManager::addAccount(string username, string password, char currency, int32_t& accountN){
    try {
        accountN = this->accountIdCounter++;
        Account* account = new Account(accountN, username, password, currency);
        this->accountStore.push_back(*account);
        delete account;
        return SUCCESS;
    } catch (int err) {
        return INTERNAL_ERROR;
    }
} 


int32_t AccountManager::closeAccount(string username, int32_t accountNumber, string password){
    try{
        Account& account = this->getAccount(username, accountNumber);
        if(account.getPassword() == password){
            this->accountStore.erase(
                std::remove(
                    this->accountStore.begin(), 
                    this->accountStore.end(), 
                    account
                ), 
                this->accountStore.end()
            );
        } else{return INVALID_CREDENTIALS;}

        return SUCCESS;

    } catch (int err) {
        return INTERNAL_ERROR;
    }
} 

int32_t AccountManager::depositWithdraw(string username, int32_t accountNumber, string password, char currency, int64_t amount, int64_t& newBalance){
    try {
        Account& account = this->getAccount(username, accountNumber);
        int64_t newBalance = account.getBalance() + amount;
        if(newBalance < 0) return INSUFFICIENT_FUNDS;
        account.updateBalance(newBalance);
        return SUCCESS;
    } catch (int err) {
        return INTERNAL_ERROR;
    }
}

int32_t AccountManager::transfer(string senderUsername, int32_t senderAccountNumber, string recipientUsername, int32_t recipientAccountNumber,
            char senderCurrency, char recipientCurrency, string password, int64_t amount, float& exchangeRate, int64_t& remaining_balance){
    
    exchangeRate = 1.0;
    try {
        Account& sender = this->getAccount(senderUsername, senderAccountNumber);
        if(sender.getBalance() < amount) return INSUFFICIENT_FUNDS;
        if(validateAccount(sender, password) == 0){
            Account& receiver = this->getAccount(recipientUsername, recipientAccountNumber);
            // not sure if this is the good practice but let's just do this for now
            sender.updateBalance(sender.getBalance() - amount);
            remaining_balance = sender.getBalance();
            int64_t exchangedAmount;
            exchangeCurrency(senderCurrency, recipientCurrency, amount, exchangedAmount, exchangeRate);
            receiver.updateBalance(receiver.getBalance() + exchangedAmount);
            return SUCCESS;
        }
        return INVALID_CREDENTIALS;
    } catch (int err) {
        return INTERNAL_ERROR;
    }
}

int32_t AccountManager::changePassword(string username, int32_t accountNumber, string password, string newPassword){
    try {
        Account& account = this->getAccount(username, accountNumber);
        if(account.getPassword() == password){
            account.updatePassword(newPassword);
            return SUCCESS;
        } else {
            return INVALID_CREDENTIALS;
        }
    } catch (int err) {
        return INTERNAL_ERROR;
    }
}

int32_t AccountManager::viewAccount(string username, int32_t accountNumber, string password, char& currency, int64_t& balance){
    try {
        Account& account = this->getAccount(username, accountNumber);
        if(account.getPassword() == password){
            currency = account.getCurrency();
            balance = account.getBalance();
            return SUCCESS;
        } else {
            return INVALID_CREDENTIALS;
        }
    } catch (int err) {
        return INTERNAL_ERROR;
    }
}

// TODO: Implement exchangeCurrency function
int32_t AccountManager::exchangeCurrency(char fromCurrency, char toCurrency, int64_t from_amount, int64_t& to_amount, float& rate){
    try {   
        double exchangeRate = exchangeRates[fromCurrency][toCurrency];
        double convertedAmount = from_amount * exchangeRate;
        to_amount = static_cast<int64_t>(convertedAmount);
        rate = static_cast<float>(exchangeRate);
        return SUCCESS;
    } catch (int err) {
        return INTERNAL_ERROR;
    }
}


int32_t AccountManager::validateAccount(Account account, string password){
    if(account.getPassword() == password)
        return 0;  
    return -1;
}

int32_t AccountManager::accountNumberCheck(int32_t accountNumber){
    for(Account& account : this->accountStore){
        if(account.getAccountNumber() == accountNumber){
            return 1;
        }
    }
    return 0;
}

int32_t main(){
    // AccountManager* accountManager = new AccountManager();
    // accountManager->addAccount("user1", "password1", SGD);
    // accountManager->addAccount("user1", "password1", USD);
    // accountManager->addAccount("user2", "password2", USD);

    // // 10000  means 100.00 dollars, since we are using int64_t to represent the balance in cents
    // accountManager->depositWithdraw("user1", 1, "password1", SGD, 10000);
    // accountManager->depositWithdraw("user1", 2, "password1", USD, 10000);
    // accountManager->depositWithdraw("user2", 3, "password2", USD, 10000);

    // accountManager->transfer("user1", 1, "password1", SGD, 5000, "user2", 3, USD);
    // accountManager->closeAccount("user1", 1, "password1", SGD);
    // delete accountManager;
}