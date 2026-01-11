#include "BankingSystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

// Constructor
BankingSystem::BankingSystem() : nextAccountNumber(1001), dataFileName("bank_data.txt") {
    loadFromFile();
}

// Find account index in vector
int BankingSystem::findAccountIndex(int accountNumber) {
    for (size_t i = 0; i < accounts.size(); i++) {
        if (accounts[i].getAccountNumber() == accountNumber) {
            return i;
        }
    }
    return -1;
}

// Create a new account
void BankingSystem::createAccount(string name, double initialDeposit) {
    if (initialDeposit < 0) {
        cout << "Error: Initial deposit cannot be negative!" << endl;
        return;
    }
    
    BankAccount newAccount(nextAccountNumber, name, initialDeposit);
    accounts.push_back(newAccount);
    
    cout << "*** Account Created Successfully! ***" << endl;
    cout << "Account Number:" << nextAccountNumber << endl;
    cout << "Account Holder:" << name << endl;
    cout << "Initial Balance: $" << fixed << setprecision(2) << initialDeposit << endl;
    
    nextAccountNumber++;
    saveToFile();
}

// Find and return pointer to account
BankAccount* BankingSystem::findAccount(int accountNumber) {
    int index = findAccountIndex(accountNumber);
    if (index != -1) {
        return &accounts[index];
    }
    return nullptr;
}

// Delete an account
void BankingSystem::deleteAccount(int accountNumber) {
    int index = findAccountIndex(accountNumber);
    if (index == -1) {
        cout << "Error: Account not found!" << endl;
        return;
    }
    
    cout << "Deleting account for: " << accounts[index].getAccountHolderName() << endl;
    accounts.erase(accounts.begin() + index);
    cout << "Account deleted successfully!" << endl;    saveToFile();}

// List all accounts
void BankingSystem::listAllAccounts() const {
    cout << "\n========================================" << endl;
    cout << "         ALL BANK ACCOUNTS" << endl;
    cout << "========================================" << endl;
    
    if (accounts.empty()) {
        cout << "No accounts in the system." << endl;
    } else {
        cout << left << setw(15) << "Account #" 
             << setw(25) << "Account Holder" 
             << right << setw(15) << "Balance" << endl;
        cout << "----------------------------------------" << endl;
        
        for (const auto& account : accounts) {
            cout << left << setw(15) << account.getAccountNumber()
                 << setw(25) << account.getAccountHolderName()
                 << right << setw(15) << fixed << setprecision(2) 
                 << "$" << account.getBalance() << endl;
        }
    }
    cout << "========================================\n" << endl;
}

// Save all accounts to file
bool BankingSystem::saveToFile() {
    ofstream outFile(dataFileName);
    if (!outFile) {
        cerr << "Error: Could not open file for saving!" << endl;
        return false;
    }
    
    // Save next account number
    outFile << nextAccountNumber << endl;
    
    // Save number of accounts
    outFile << accounts.size() << endl;
    
    // Save each account
    for (const auto& account : accounts) {
        outFile << account.getAccountNumber() << endl;
        outFile << account.getAccountHolderName() << endl;
        outFile << fixed << setprecision(2) << account.getBalance() << endl;
    }
    
    outFile.close();
    return true;
}

// Load all accounts from file
bool BankingSystem::loadFromFile() {
    ifstream inFile(dataFileName);
    if (!inFile) {
        // File doesn't exist yet, not an error
        return false;
    }
    
    accounts.clear();
    
    // Load next account number
    inFile >> nextAccountNumber;
    
    // Load number of accounts
    int numAccounts;
    inFile >> numAccounts;
    inFile.ignore(); // Clear newline
    
    // Load each account
    for (int i = 0; i < numAccounts; i++) {
        int accNum;
        string name;
        double balance;
        
        inFile >> accNum;
        inFile.ignore(); // Clear newline
        getline(inFile, name);
        inFile >> balance;
        inFile.ignore(); // Clear newline
        
        BankAccount account(accNum, name, balance);
        accounts.push_back(account);
    }
    
    inFile.close();
    
    if (numAccounts > 0) {
        cout << "\n*** Loaded " << numAccounts << " account(s) from file ***\n" << endl;
    }
    
    return true;
}

// Export accounts to JSON format
void BankingSystem::exportToJSON(string filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error: Could not create JSON file!" << endl;
        return;
    }
    
    outFile << "{\n";
    outFile << "  \"bankingSystem\": {\n";
    outFile << "    \"nextAccountNumber\": " << nextAccountNumber << ",\n";
    outFile << "    \"accounts\": [\n";
    
    for (size_t i = 0; i < accounts.size(); i++) {
        outFile << "      {\n";
        outFile << "        \"accountNumber\": " << accounts[i].getAccountNumber() << ",\n";
        outFile << "        \"accountHolder\": \"" << accounts[i].getAccountHolderName() << "\",\n";
        outFile << "        \"balance\": " << fixed << setprecision(2) << accounts[i].getBalance() << "\n";
        outFile << "      }";
        if (i < accounts.size() - 1) {
            outFile << ",";
        }
        outFile << "\n";
    }
    
    outFile << "    ]\n";
    outFile << "  }\n";
    outFile << "}\n";
    
    outFile.close();
    cout << "\n*** Data exported to " << filename << " ***\n" << endl;
}

// Display main menu
void BankingSystem::displayMenu() {
    cout << "\n======================================" << endl;
    cout << "   AUTOMATED BANKING SYSTEM" << endl;
    cout << "======================================" << endl;
    cout << "1. Create New Account" << endl;
    cout << "2. Deposit Money" << endl;
    cout << "3. Withdraw Money" << endl;
    cout << "4. Check Balance" << endl;
    cout << "5. View Transaction History" << endl;
    cout << "6. List All Accounts" << endl;
    cout << "7. Delete Account" << endl;
    cout << "8. Export to JSON" << endl;
    cout << "9. Exit" << endl;
    cout << "======================================" << endl;
    cout << "Enter your choice: ";
}

// Run the banking system
void BankingSystem::run() {
    int choice;
    int accountNumber;
    double amount;
    string name;
    
    cout << "\n*** Welcome to Automated Banking System ***\n" << endl;
    
    while (true) {
        displayMenu();
        cin >> choice;
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                cout << "\n--- Create New Account ---" << endl;
                cout << "Enter account holder name: ";
                getline(cin, name);
                cout << "Enter initial deposit (0 for none): $";
                cin >> amount;
                createAccount(name, amount);
                break;
            }
            
            case 2: {
                cout << "\n--- Deposit Money ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist! Please create an account first (Option 1)." << endl;
                    break;
                }
                cout << "Enter account number (e.g., 1001): ";
                cin >> accountNumber;
                
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    cout << "Account Holder: " << account->getAccountHolderName() << endl;
                    cout << "Current Balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
                    cout << "Enter amount to deposit: $";
                    cin >> amount;
                    account->deposit(amount);
                    saveToFile();
                } else {
                    cout << "Error: Account #" << accountNumber << " not found!" << endl;
                    cout << "Tip: Use option 6 to see all account numbers." << endl;
                }
                break;
            }
            
            case 3: {
                cout << "\n--- Withdraw Money ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist! Please create an account first (Option 1)." << endl;
                    break;
                }
                cout << "Enter account number (e.g., 1001): ";
                cin >> accountNumber;
                
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    cout << "Account Holder: " << account->getAccountHolderName() << endl;
                    cout << "Current Balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
                    cout << "Enter amount to withdraw: $";
                    cin >> amount;
                    account->withdraw(amount);
                    saveToFile();
                } else {
                    cout << "Error: Account #" << accountNumber << " not found!" << endl;
                    cout << "Tip: Use option 6 to see all account numbers." << endl;
                }
                break;
            }
            
            case 4: {
                cout << "\n--- Check Balance ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist! Please create an account first (Option 1)." << endl;
                    break;
                }
                cout << "Enter account number (e.g., 1001): ";
                cin >> accountNumber;
                
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    account->displayAccountInfo();
                } else {
                    cout << "Error: Account #" << accountNumber << " not found!" << endl;
                    cout << "Tip: Use option 6 to see all account numbers." << endl;
                }
                break;
            }
            
            case 5: {
                cout << "\n--- Transaction History ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist! Please create an account first (Option 1)." << endl;
                    break;
                }
                cout << "Enter account number (e.g., 1001): ";
                cin >> accountNumber;
                
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    account->displayTransactionHistory();
                } else {
                    cout << "Error: Account #" << accountNumber << " not found!" << endl;
                    cout << "Tip: Use option 6 to see all account numbers." << endl;
                }
                break;
            }
            
            case 6: {
                listAllAccounts();
                break;
            }
            
            case 7: {
                cout << "\n--- Delete Account ---" << endl;
                cout << "Enter account number: ";
                cin >> accountNumber;
                deleteAccount(accountNumber);
                break;
            }
            
            case 8: {
                cout << "\n--- Export to JSON ---" << endl;
                string jsonFile = "bank_export.json";
                exportToJSON(jsonFile);
                break;
            }
            
            case 9: {
                saveToFile();
                cout << "\n*** Thank you for using Automated Banking System! ***" << endl;
                cout << "Goodbye!" << endl;
                return;
            }
            
            default: {
                cout << "Invalid choice! Please try again." << endl;
                break;
            }
        }
    }
}
