#include "BankAccount.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor
BankAccount::BankAccount(int accNum, string name, double initialBalance)
    : accountNumber(accNum), accountHolderName(name), balance(initialBalance) {
    if (initialBalance > 0) {
        addTransaction("Initial Deposit", initialBalance);
    }
}

// Getters
int BankAccount::getAccountNumber() const {
    return accountNumber;
}

string BankAccount::getAccountHolderName() const {
    return accountHolderName;
}

double BankAccount::getBalance() const {
    return balance;
}

// Deposit money into account
bool BankAccount::deposit(double amount) {
    if (amount <= 0) {
        cout << "Error: Deposit amount must be positive!" << endl;
        return false;
    }
    
    balance += amount;
    addTransaction("Deposit", amount);
    cout << "Successfully deposited $" << fixed << setprecision(2) << amount << endl;
    cout << "New balance: $" << balance << endl;
    return true;
}

// Withdraw money from account
bool BankAccount::withdraw(double amount) {
    if (amount <= 0) {
        cout << "Error: Withdrawal amount must be positive!" << endl;
        return false;
    }
    
    if (amount > balance) {
        cout << "Error: Insufficient funds!" << endl;
        cout << "Current balance: $" << fixed << setprecision(2) << balance << endl;
        return false;
    }
    
    balance -= amount;
    addTransaction("Withdrawal", amount);
    cout << "Successfully withdrew $" << fixed << setprecision(2) << amount << endl;
    cout << "New balance: $" << balance << endl;
    return true;
}

// Display account information
void BankAccount::displayAccountInfo() const {
    cout << "\n========================================" << endl;
    cout << "         ACCOUNT INFORMATION" << endl;
    cout << "========================================" << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Account Holder: " << accountHolderName << endl;
    cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
    cout << "========================================\n" << endl;
}

// Display transaction history
void BankAccount::displayTransactionHistory() const {
    cout << "\n========================================" << endl;
    cout << "       TRANSACTION HISTORY" << endl;
    cout << "========================================" << endl;
    
    if (transactionHistory.empty()) {
        cout << "No transactions yet." << endl;
    } else {
        for (size_t i = 0; i < transactionHistory.size(); i++) {
            const Transaction& trans = transactionHistory[i];
            cout << (i + 1) << ". " << trans.type << ": $" 
                 << fixed << setprecision(2) << trans.amount 
                 << " | Balance After: $" << trans.balanceAfter << endl;
        }
    }
    cout << "========================================\n" << endl;
}

// Add transaction to history
void BankAccount::addTransaction(string type, double amount) {
    Transaction trans;
    trans.type = type;
    trans.amount = amount;
    trans.balanceAfter = balance;
    trans.timestamp = time(0);
    transactionHistory.push_back(trans);
}
