#ifndef BANKINGSYSTEM_H
#define BANKINGSYSTEM_H

#include "BankAccount.h"
#include <vector>
#include <map>

using namespace std;

class BankingSystem {
private:
    vector<BankAccount> accounts;
    int nextAccountNumber;
    string dataFileName;
    
    // Helper function to find account index
    int findAccountIndex(int accountNumber);

public:
    // Constructor
    BankingSystem();
    
    // System operations
    void createAccount(string name, double initialDeposit = 0.0);
    BankAccount* findAccount(int accountNumber);
    void deleteAccount(int accountNumber);
    void listAllAccounts() const;
    
    // File operations
    bool saveToFile();
    bool loadFromFile();
    void exportToJSON(string filename);
    
    // Main menu
    void displayMenu();
    void run();
};

#endif
