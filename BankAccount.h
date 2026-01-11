#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Transaction structure to store transaction history
struct Transaction {
    string type = "";        // "Deposit", "Withdrawal", "Transfer"
    double amount = 0.0;
    double balanceAfter = 0.0;
    time_t timestamp = 0;
};

class BankAccount {
private:
    int accountNumber;
    string accountHolderName;
    double balance;
    vector<Transaction> transactionHistory;

public:
    // Constructor
    BankAccount(int accNum, string name, double initialBalance = 0.0);
    
    // Getters
    int getAccountNumber() const;
    string getAccountHolderName() const;
    double getBalance() const;
    
    // Banking operations
    bool deposit(double amount);
    bool withdraw(double amount);
    void displayAccountInfo() const;
    void displayTransactionHistory() const;
    
    // Helper function to add transaction to history
    void addTransaction(string type, double amount);
};

#endif
