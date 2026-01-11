#include "BankingSystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

// Constructor
BankingSystem::BankingSystem() 
    : nextAccountNumber(1001), dataFileName("bank_data.txt"), 
      usersFileName("users.txt"), currentUser(nullptr) {
    loadFromFile();
    loadUsers();
    if (users.empty()) {
        createDefaultUsers();
    }
}

// Find account index in vector
int BankingSystem::findAccountIndex(int accountNumber) {
    for (size_t i = 0; i < accounts.size(); i++) {
        if (accounts[i].getAccountNumber() == accountNumber) {
            return static_cast<int>(i);
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

// Save users to file
bool BankingSystem::saveUsers() {
    ofstream outFile(usersFileName);
    if (!outFile) {
        cerr << "Error: Could not open users file for saving!" << endl;
        return false;
    }
    
    outFile << users.size() << endl;
    for (const auto& user : users) {
        outFile << user.getUsername() << endl;
        outFile << user.getPasswordHash() << endl;
        outFile << static_cast<int>(user.getRole()) << endl;
        outFile << (user.getLocked() ? 1 : 0) << endl;
    }
    
    outFile.close();
    return true;
}

// Load users from file
bool BankingSystem::loadUsers() {
    ifstream inFile(usersFileName);
    if (!inFile) {
        return false;
    }
    
    int numUsers;
    inFile >> numUsers;
    inFile.ignore();
    
    for (int i = 0; i < numUsers; i++) {
        string username, passwordHash;
        int roleInt, lockedInt;
        
        getline(inFile, username);
        getline(inFile, passwordHash);
        inFile >> roleInt;
        inFile >> lockedInt;
        inFile.ignore();
        
        users.push_back(User(username, passwordHash, static_cast<UserRole>(roleInt), lockedInt == 1));
    }
    
    inFile.close();
    return true;
}

// Create default users
void BankingSystem::createDefaultUsers() {
    users.push_back(User("admin", User::hashPassword("admin123"), ADMIN, false));
    users.push_back(User("user1", User::hashPassword("pass123"), USER, false));
    users.push_back(User("guest", User::hashPassword("guest123"), GUEST, false));
    saveUsers();
    cout << "\n*** Default users created ***" << endl;
    cout << "Admin: username='admin', password='admin123'" << endl;
    cout << "User: username='user1', password='pass123'" << endl;
    cout << "Guest: username='guest', password='guest123'" << endl;
}

// Login system
User* BankingSystem::login() {
    string username, password;
    
    while (true) {
        cout << "\n======================================" << endl;
        cout << "           LOGIN SYSTEM" << endl;
        cout << "======================================" << endl;
        cout << "Username: ";
        cin >> username;
        
        // Find user
        User* user = nullptr;
        for (auto& u : users) {
            if (u.getUsername() == username) {
                user = &u;
                break;
            }
        }
        
        if (!user) {
            cout << "Error: Username not found!" << endl;
            cout << "Please try again.\n" << endl;
            continue;
        }
        
        // Check if account is locked
        if (user->getLocked()) {
            cout << "\n*** ACCOUNT LOCKED ***" << endl;
            cout << "This account has been locked due to too many failed login attempts." << endl;
            cout << "Please contact an administrator or try a different account.\n" << endl;
            continue;
        }
        
        cout << "Password: ";
        cin >> password;
        
        if (user->authenticate(password)) {
            user->resetFailedAttempts();
            saveUsers();
            cout << "\n*** Login Successful! ***" << endl;
            cout << "Welcome, " << username << " (" << user->getRoleName() << ")" << endl;
            return user;
        } else {
            user->incrementFailedAttempts();
            saveUsers();
            
            if (user->getLocked()) {
                cout << "\n*** ACCOUNT LOCKED ***" << endl;
                cout << "Too many failed attempts. Your account has been locked." << endl;
                cout << "Please contact an administrator or try a different account.\n" << endl;
            } else {
                cout << "\nInvalid password!" << endl;
                cout << "Failed attempts: " << user->getFailedAttempts() << "/3" << endl;
                cout << "Warning: Account will be locked after 3 failed attempts." << endl;
                cout << "Please try again.\n" << endl;
            }
        }
    }
}

// Register new user (Admin only)
void BankingSystem::registerUser() {
    string username, password;
    int roleChoice;
    
    cout << "\n--- Register New User ---" << endl;
    cout << "Enter username: ";
    cin >> username;
    
    // Check if username exists
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            cout << "Error: Username already exists!" << endl;
            return;
        }
    }
    
    cout << "Enter password: ";
    cin >> password;
    
    // Hash the password
    string hashedPassword = User::hashPassword(password);
    
    cout << "Select role:" << endl;
    cout << "1. Admin" << endl;
    cout << "2. User" << endl;
    cout << "3. Guest" << endl;
    cout << "Choice: ";
    cin >> roleChoice;
    
    UserRole role;
    switch (roleChoice) {
        case 1: role = ADMIN; break;
        case 2: role = USER; break;
        case 3: role = GUEST; break;
        default:
            cout << "Invalid choice! Defaulting to User role." << endl;
            role = USER;
    }
    
    users.push_back(User(username, hashedPassword, role, false));
    saveUsers();
    
    cout << "\n*** User registered successfully! ***" << endl;
    cout << "Username: " << username << endl;
    cout << "Password Hash: " << hashedPassword << endl;
    cout << "Role: ";
    switch (role) {
        case ADMIN: cout << "Admin"; break;
        case USER: cout << "User"; break;
        case GUEST: cout << "Guest"; break;
    }
    cout << endl;
}

// Public registration (available before login)
void BankingSystem::registerNewUser() {
    string username, password;
    
    cout << "\n======================================" << endl;
    cout << "        USER REGISTRATION" << endl;
    cout << "======================================" << endl;
    cout << "Enter username: ";
    cin >> username;
    
    // Duplicate Check: Verify username is unique
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            cout << "\nError: Username '" << username << "' already exists!" << endl;
            cout << "Please choose a different username.\n" << endl;
            return;
        }
    }
    
    cout << "Enter password: ";
    cin >> password;
    
    // Ask user to choose role
    cout << "\nSelect account type:" << endl;
    cout << "1. User (Full access - create accounts, deposit, withdraw)" << endl;
    cout << "2. Guest (View-only access)" << endl;
    cout << "Choice: ";
    int roleChoice;
    cin >> roleChoice;
    
    // Hash the password
    string hashedPassword = User::hashPassword(password);
    
    // Set role based on choice
    UserRole role;
    if (roleChoice == 2) {
        role = GUEST;
    } else {
        role = USER;  // Default to USER
    }
    
    // Create and save user
    users.push_back(User(username, hashedPassword, role, false));
    saveUsers();  // Persistence: Save immediately
    
    cout << "\n*** Registration Successful! ***" << endl;
    cout << "Username: " << username << endl;
    cout << "Password Hash: " << hashedPassword << endl;
    cout << "Role: " << (role == GUEST ? "Guest" : "User") << endl;
    cout << "You can now login with your credentials.\n" << endl;
}

// Manage users (Admin only)
void BankingSystem::manageUsers() {
    cout << "\n========================================" << endl;
    cout << "        USER MANAGEMENT" << endl;
    cout << "========================================" << endl;
    cout << left << setw(20) << "Username" << setw(15) << "Role" << setw(10) << "Status" << endl;
    cout << "----------------------------------------" << endl;
    
    for (const auto& user : users) {
        cout << left << setw(20) << user.getUsername() 
             << setw(15) << user.getRoleName()
             << setw(10) << (user.getLocked() ? "LOCKED" : "Active") << endl;
    }
    cout << "========================================\n" << endl;
}

// Unlock user account (Admin only)
void BankingSystem::unlockAccount() {
    string username;
    
    cout << "\n--- Unlock User Account ---" << endl;
    
    // Display all users first
    cout << "\n========================================" << endl;
    cout << "         CURRENT USERS" << endl;
    cout << "========================================" << endl;
    cout << left << setw(20) << "Username" << setw(15) << "Role" << setw(10) << "Status" << endl;
    cout << "----------------------------------------" << endl;
    
    for (const auto& user : users) {
        cout << left << setw(20) << user.getUsername() 
             << setw(15) << user.getRoleName()
             << setw(10) << (user.getLocked() ? "LOCKED" : "Active") << endl;
    }
    cout << "========================================\n" << endl;
    
    cout << "Enter username to unlock: ";
    cin >> username;
    
    // Find user
    User* user = nullptr;
    for (auto& u : users) {
        if (u.getUsername() == username) {
            user = &u;
            break;
        }
    }
    
    if (!user) {
        cout << "Error: User '" << username << "' not found!" << endl;
        return;
    }
    
    if (!user->getLocked()) {
        cout << "Account '" << username << "' is not locked." << endl;
        return;
    }
    
    user->setLocked(false);
    user->resetFailedAttempts();
    saveUsers();
    
    cout << "\n*** Account Unlocked Successfully! ***" << endl;
    cout << "User '" << username << "' can now log in." << endl;
}

// View system logs (Admin only)
void BankingSystem::viewSystemLogs() {
    cout << "\n========================================" << endl;
    cout << "          SYSTEM LOGS" << endl;
    cout << "========================================" << endl;
    
    time_t now = time(0);
    tm timeInfo;
    localtime_s(&timeInfo, &now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    
    cout << "Current Time: " << buffer << endl;
    cout << "Total Accounts: " << accounts.size() << endl;
    cout << "Total Users: " << users.size() << endl;
    cout << "Next Account Number: " << nextAccountNumber << endl;
    
    double totalBalance = 0;
    for (const auto& account : accounts) {
        totalBalance += account.getBalance();
    }
    cout << "Total Bank Balance: $" << fixed << setprecision(2) << totalBalance << endl;
    cout << "========================================\n" << endl;
}

// Main menu (before login)
void BankingSystem::displayMainMenu() {
    cout << "\n======================================" << endl;
    cout << "   AUTOMATED BANKING SYSTEM" << endl;
    cout << "======================================" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register New Account" << endl;
    cout << "3. Exit" << endl;
    cout << "======================================" << endl;
    cout << "Enter your choice: ";
}

// Admin menu
void BankingSystem::displayAdminMenu() {
    cout << "\n======================================" << endl;
    cout << "      ADMIN - BANKING SYSTEM" << endl;
    cout << "======================================" << endl;
    cout << "1. Create New Account" << endl;
    cout << "2. Deposit Money" << endl;
    cout << "3. Withdraw Money" << endl;
    cout << "4. Check Balance" << endl;
    cout << "5. View Transaction History" << endl;
    cout << "6. List All Accounts" << endl;
    cout << "7. Delete Account" << endl;
    cout << "8. Export to JSON" << endl;
    cout << "9. View System Logs" << endl;
    cout << "10. User Management" << endl;
    cout << "11. Register New User" << endl;
    cout << "12. Unlock User Account" << endl;
    cout << "13. Logout" << endl;
    cout << "======================================" << endl;
    cout << "Enter your choice: ";
}

// User menu
void BankingSystem::displayUserMenu() {
    cout << "\n======================================" << endl;
    cout << "      USER - BANKING SYSTEM" << endl;
    cout << "======================================" << endl;
    cout << "1. Create New Account" << endl;
    cout << "2. Deposit Money" << endl;
    cout << "3. Withdraw Money" << endl;
    cout << "4. Check Balance" << endl;
    cout << "5. View Transaction History" << endl;
    cout << "6. List All Accounts" << endl;
    cout << "7. Export to JSON" << endl;
    cout << "8. Logout" << endl;
    cout << "======================================" << endl;
    cout << "Enter your choice: ";
}

// Guest menu (view-only)
void BankingSystem::displayGuestMenu() {
    cout << "\n======================================" << endl;
    cout << "     GUEST - BANKING SYSTEM" << endl;
    cout << "     (View-Only Access)" << endl;
    cout << "======================================" << endl;
    cout << "1. Check Balance" << endl;
    cout << "2. View Transaction History" << endl;
    cout << "3. List All Accounts" << endl;
    cout << "4. Logout" << endl;
    cout << "======================================" << endl;
    cout << "Enter your choice: ";
}

// Admin session
void BankingSystem::runAdminSession() {
    int choice;
    int accountNumber;
    double amount;
    string name;
    
    while (true) {
        displayAdminMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 13) {
            cout << "\n*** Logged out ***" << endl;
            return;
        }
        
        switch (choice) {
            case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
                // Reuse existing functionality by calling the old menu handler
                // We'll handle these inline
                break;
            case 9:
                viewSystemLogs();
                break;
            case 10:
                manageUsers();
                break;
            case 11:
                registerUser();
                break;
            case 12:
                unlockAccount();
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
        
        // Handle standard banking operations
        if (choice >= 1 && choice <= 8) {
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
                        cout << "No accounts exist!" << endl;
                        break;
                    }
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    BankAccount* account = findAccount(accountNumber);
                    if (account) {
                        cout << "Enter amount: $";
                        cin >> amount;
                        account->deposit(amount);
                        saveToFile();
                    } else {
                        cout << "Account not found!" << endl;
                    }
                    break;
                }
                case 3: {
                    cout << "\n--- Withdraw Money ---" << endl;
                    if (accounts.empty()) {
                        cout << "No accounts exist!" << endl;
                        break;
                    }
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    BankAccount* account = findAccount(accountNumber);
                    if (account) {
                        cout << "Enter amount: $";
                        cin >> amount;
                        account->withdraw(amount);
                        saveToFile();
                    } else {
                        cout << "Account not found!" << endl;
                    }
                    break;
                }
                case 4: {
                    cout << "\n--- Check Balance ---" << endl;
                    if (accounts.empty()) {
                        cout << "No accounts exist!" << endl;
                        break;
                    }
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    BankAccount* account = findAccount(accountNumber);
                    if (account) {
                        account->displayAccountInfo();
                    } else {
                        cout << "Account not found!" << endl;
                    }
                    break;
                }
                case 5: {
                    cout << "\n--- Transaction History ---" << endl;
                    if (accounts.empty()) {
                        cout << "No accounts exist!" << endl;
                        break;
                    }
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    BankAccount* account = findAccount(accountNumber);
                    if (account) {
                        account->displayTransactionHistory();
                    } else {
                        cout << "Account not found!" << endl;
                    }
                    break;
                }
                case 6:
                    listAllAccounts();
                    break;
                case 7: {
                    cout << "\n--- Delete Account ---" << endl;
                    cout << "Enter account number: ";
                    cin >> accountNumber;
                    deleteAccount(accountNumber);
                    break;
                }
                case 8: {
                    exportToJSON("bank_export.json");
                    break;
                }
            }
        }
    }
}

// User session
void BankingSystem::runUserSession() {
    int choice;
    int accountNumber;
    double amount;
    string name;
    
    while (true) {
        displayUserMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 8) {
            cout << "\n*** Logged out ***" << endl;
            return;
        }
        
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
                    cout << "No accounts exist!" << endl;
                    break;
                }
                cout << "Enter account number: ";
                cin >> accountNumber;
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    cout << "Enter amount: $";
                    cin >> amount;
                    account->deposit(amount);
                    saveToFile();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 3: {
                cout << "\n--- Withdraw Money ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist!" << endl;
                    break;
                }
                cout << "Enter account number: ";
                cin >> accountNumber;
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    cout << "Enter amount: $";
                    cin >> amount;
                    account->withdraw(amount);
                    saveToFile();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 4: {
                cout << "\n--- Check Balance ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist!" << endl;
                    break;
                }
                cout << "Enter account number: ";
                cin >> accountNumber;
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    account->displayAccountInfo();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 5: {
                cout << "\n--- Transaction History ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist!" << endl;
                    break;
                }
                cout << "Enter account number: ";
                cin >> accountNumber;
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    account->displayTransactionHistory();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 6:
                listAllAccounts();
                break;
            case 7:
                exportToJSON("bank_export.json");
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

// Guest session (view-only)
void BankingSystem::runGuestSession() {
    int choice;
    int accountNumber;
    
    while (true) {
        displayGuestMenu();
        cin >> choice;
        
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 4) {
            cout << "\n*** Logged out ***" << endl;
            return;
        }
        
        switch (choice) {
            case 1: {
                cout << "\n--- Check Balance ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist!" << endl;
                    break;
                }
                cout << "Enter account number: ";
                cin >> accountNumber;
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    account->displayAccountInfo();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 2: {
                cout << "\n--- Transaction History ---" << endl;
                if (accounts.empty()) {
                    cout << "No accounts exist!" << endl;
                    break;
                }
                cout << "Enter account number: ";
                cin >> accountNumber;
                BankAccount* account = findAccount(accountNumber);
                if (account) {
                    account->displayTransactionHistory();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            case 3:
                listAllAccounts();
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    }
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
    cout << "\n*** Welcome to Automated Banking System ***\n" << endl;
    
    User* loggedInUser = nullptr;
    
    while (true) {
        displayMainMenu();
        int choice;
        cin >> choice;
        
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                // Login
                currentUser = login();
                if (currentUser) {
                    // Route to appropriate session based on role
                    switch (currentUser->getRole()) {
                        case ADMIN:
                            runAdminSession();
                            break;
                        case USER:
                            runUserSession();
                            break;
                        case GUEST:
                            runGuestSession();
                            break;
                    }
                    currentUser = nullptr;
                }
                break;
            }
            
            case 2: {
                // Register
                registerNewUser();
                break;
            }
            
            case 3: {
                // Exit
                saveToFile();
                cout << "\n*** Thank you for using Automated Banking System! ***" << endl;
                cout << "Goodbye!" << endl;
                return;
            }
            
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    }
}
