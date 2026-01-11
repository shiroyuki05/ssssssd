#ifndef BANKINGSYSTEM_H
#define BANKINGSYSTEM_H

#include "BankAccount.h"
#include "User.h"
#include <vector>
#include <map>

using namespace std;

class BankingSystem {
private:
    vector<BankAccount> accounts;
    vector<User> users;
    int nextAccountNumber;
    string dataFileName;
    string usersFileName;
    User* currentUser;
    
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
    bool saveUsers();
    bool loadUsers();
    
    // User management
    void createDefaultUsers();
    User* login();
    void registerUser();  // Admin only
    void registerNewUser();  // Public registration
    void manageUsers();
    void unlockAccount();
    void viewSystemLogs();
    
    // Role-based menus
    void displayMainMenu();
    void displayAdminMenu();
    void displayUserMenu();
    void displayGuestMenu();
    void runAdminSession();
    void runUserSession();
    void runGuestSession();
    
    // Main menu
    void displayMenu();
    void run();
};

#endif
