# AUTOMATED BANKING SYSTEM
## Technical Documentation

---

### Title Page

**Project Name:** Automated Banking System  
**Student Name:** [Your Name]  
**Course/Section:** ITEC 100  
**Date:** January 11, 2026  
**Programming Language:** C++

---

## 1. PROGRAM DESCRIPTION

### Overview
The Automated Banking System is a console-based C++ application that implements a secure banking platform with comprehensive user authentication, role-based access control, and financial transaction management. The system demonstrates advanced security features including password hashing, brute-force protection, and account locking mechanisms.

### Key Features
- **User Authentication System** with login and registration
- **Role-Based Access Control (RBAC)** supporting three user roles: Admin, User, and Guest
- **Password Hashing** using a custom hash algorithm (DJB2)
- **Brute-Force Protection** with automatic account locking after 3 failed login attempts
- **Bank Account Management** supporting deposits, withdrawals, and balance inquiries
- **Transaction History** tracking all financial operations
- **Data Persistence** saving user credentials and account data to local files
- **JSON Export** capability for data backup and analysis

### Security Features Applied
1. **Password Hashing:** Plain text passwords are never stored; only hashed values are saved to files
2. **Authentication & Brute-Force Protection:** Login attempts are tracked per user, with accounts automatically locking after 3 failed attempts
3. **Locked Status Persistence:** Account lock status is saved to file and persists across sessions
4. **Duplicate Username Prevention:** Registration validates usernames to ensure uniqueness
5. **Role-Based Authorization:** Different user roles have restricted access to system features

---

## 2. SYSTEM ARCHITECTURE

### A. Flowchart - Login and Registration Process

```
    START
    |
    v
    [Display Main Menu]
    |
    +---> [1. Login] -------> [Enter Username]
    |                              |
    |                              v
    |                         [User Exists?] --No--> [Error: Username not found]
    |                              |                         |
    |                             Yes                        |
    |                              |                         |
    |                              v                         |
    |                         [Account Locked?] --Yes--> [Display Lock Message]
    |                              |                         |
    |                             No                         |
    |                              |                         |
    |                              v                         |
    |                         [Enter Password]               |
    |                              |                         |
    |                              v                         |
    |                         [Hash Password]                |
    |                              |                         |
    |                              v                         |
    |                    [Compare with Stored Hash]          |
    |                              |                         |
    |                    +---------+---------+               |
    |                    |                   |               |
    |                   Match             No Match           |
    |                    |                   |               |
    |                    v                   v               |
    |            [Reset Failed Count] [Increment Failed Count]
    |                    |                   |               |
    |                    v                   v               |
    |            [Login Success]      [Failed Count >= 3?]   |
    |                    |                   |               |
    |                    v              +----+----+          |
    |            [Route to Role Menu]   |         |          |
    |            (Admin/User/Guest)    Yes       No          |
    |                                  |         |          |
    |                                  v         v          |
    |                            [Lock Account] [Retry] <---+
    |                                  |
    |                                  v
    |                            [Save to File]
    |
    +---> [2. Register] -------> [Enter Username]
    |                                  |
    |                                  v
    |                         [Check Duplicate]
    |                                  |
    |                         +--------+--------+
    |                         |                 |
    |                    Duplicate           Unique
    |                         |                 |
    |                         v                 v
    |                   [Error Msg]      [Enter Password]
    |                                           |
    |                                           v
    |                                    [Hash Password]
    |                                           |
    |                                           v
    |                                    [Select Role Type]
    |                                    (User or Guest)
    |                                           |
    |                                           v
    |                                    [Create User Object]
    |                                           |
    |                                           v
    |                                    [Save to File]
    |                                           |
    |                                           v
    |                                    [Display Hash & Success]
    |
    +---> [3. Exit] -------> END
```

### B. Data Structure - User Object

```
User Class
+------------------------+
| - username: string     |
| - passwordHash: string |
| - role: UserRole       |
| - isLocked: bool       |
| - failedAttempts: int  |
+------------------------+
| + getUsername()        |
| + getPasswordHash()    |
| + getRole()            |
| + getLocked()          |
| + setLocked()          |
| + authenticate()       |
| + hashPassword()       |
+------------------------+

UserRole Enum
+----------+
| ADMIN    | = 0
| USER     | = 1
| GUEST    | = 2
+----------+
```

### C. Data Structure - BankAccount Object

```
BankAccount Class
+---------------------------+
| - accountNumber: int      |
| - accountHolderName: str  |
| - balance: double         |
| - transactionHistory: vec |
+---------------------------+
| + deposit()               |
| + withdraw()              |
| + getBalance()            |
| + displayAccountInfo()    |
| + addTransaction()        |
+---------------------------+

Transaction Structure
+---------------------+
| - type: string      |
| - amount: double    |
| - balanceAfter: dbl |
| - timestamp: time_t |
+---------------------+
```

### D. File Storage Structure

**users.txt Format:**
```
[Number of Users]
[Username]
[Password Hash]
[Role (0=Admin, 1=User, 2=Guest)]
[Locked Status (0=Unlocked, 1=Locked)]
[Username]
[Password Hash]
...
```

**bank_data.txt Format:**
```
[Next Account Number]
[Number of Accounts]
[Account Number]
[Account Holder Name]
[Balance]
...
```

---

## 3. FUNCTION DICTIONARY

### User Management Functions

| Function Name | Parameters | Return Type | Description |
|--------------|------------|-------------|-------------|
| `User::hashPassword()` | `const string& password` | `string` | Static function that hashes password using DJB2 algorithm |
| `User::authenticate()` | `string pwd` | `bool` | Validates password by comparing hash with stored hash |
| `User::incrementFailedAttempts()` | None | `void` | Increments failed login counter; locks account at 3 |
| `User::resetFailedAttempts()` | None | `void` | Resets failed login counter to 0 |
| `User::setLocked()` | `bool locked` | `void` | Sets account locked status |
| `User::getLocked()` | None | `bool` | Returns account locked status |
| `BankingSystem::login()` | None | `User*` | Handles user login with retry loop |
| `BankingSystem::registerNewUser()` | None | `void` | Public registration with duplicate check |
| `BankingSystem::registerUser()` | None | `void` | Admin-only registration with role selection |
| `BankingSystem::unlockAccount()` | None | `void` | Admin function to unlock locked accounts |
| `BankingSystem::manageUsers()` | None | `void` | Displays all users with status |
| `BankingSystem::saveUsers()` | None | `bool` | Saves user data to users.txt file |
| `BankingSystem::loadUsers()` | None | `bool` | Loads user data from users.txt file |
| `BankingSystem::createDefaultUsers()` | None | `void` | Creates default admin, user, guest accounts |

### Banking Functions

| Function Name | Parameters | Return Type | Description |
|--------------|------------|-------------|-------------|
| `BankAccount::deposit()` | `double amount` | `bool` | Adds money to account; validates amount > 0 |
| `BankAccount::withdraw()` | `double amount` | `bool` | Removes money; checks balance and amount validity |
| `BankAccount::getBalance()` | None | `double` | Returns current account balance |
| `BankAccount::displayAccountInfo()` | None | `void` | Shows account number, holder, balance |
| `BankAccount::displayTransactionHistory()` | None | `void` | Lists all transactions with amounts and balances |
| `BankAccount::addTransaction()` | `string type, double amount` | `void` | Adds transaction record to history |
| `BankingSystem::createAccount()` | `string name, double initial` | `void` | Creates new bank account with auto-increment ID |
| `BankingSystem::findAccount()` | `int accountNumber` | `BankAccount*` | Locates account by number; returns pointer |
| `BankingSystem::deleteAccount()` | `int accountNumber` | `void` | Removes account from system |
| `BankingSystem::listAllAccounts()` | None | `void` | Displays all accounts in tabular format |

### File Operations

| Function Name | Parameters | Return Type | Description |
|--------------|------------|-------------|-------------|
| `BankingSystem::saveToFile()` | None | `bool` | Saves all bank accounts to bank_data.txt |
| `BankingSystem::loadFromFile()` | None | `bool` | Loads all bank accounts from bank_data.txt |
| `BankingSystem::exportToJSON()` | `string filename` | `void` | Exports system data to JSON format |

### Session Management

| Function Name | Parameters | Return Type | Description |
|--------------|------------|-------------|-------------|
| `BankingSystem::run()` | None | `void` | Main program loop with menu display |
| `BankingSystem::runAdminSession()` | None | `void` | Admin menu with full system access |
| `BankingSystem::runUserSession()` | None | `void` | User menu with banking operations |
| `BankingSystem::runGuestSession()` | None | `void` | Guest menu with view-only access |
| `BankingSystem::displayMainMenu()` | None | `void` | Shows login/register/exit options |
| `BankingSystem::displayAdminMenu()` | None | `void` | Shows 13 admin menu options |
| `BankingSystem::displayUserMenu()` | None | `void` | Shows 8 user menu options |
| `BankingSystem::displayGuestMenu()` | None | `void` | Shows 4 guest menu options (view-only) |
| `BankingSystem::viewSystemLogs()` | None | `void` | Admin-only: displays system statistics |

---

## 4. SECURITY IMPLEMENTATION

### A. Password Hashing Algorithm

**Algorithm Used:** DJB2 Hash Function

**Implementation:**
```cpp
string User::hashPassword(const string& password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}
```

**How It Works:**
1. Initialize hash value to 5381 (prime number)
2. For each character in the password:
   - Left shift hash by 5 bits (multiply by 32)
   - Add original hash value (effectively multiply by 33)
   - Add character ASCII value
3. Convert final hash to 16-character hexadecimal string
4. Return hashed string

**Example:**
- Input: `"admin123"`
- Output: `"00000000185030e4"`

**Security Features:**
- **One-way function:** Cannot reverse hash to get original password
- **Deterministic:** Same password always produces same hash
- **Fixed length:** All hashes are 16 hex characters regardless of password length
- **No plain text storage:** Original passwords never saved to disk

### B. Three-Attempt Lock Tracking

**Implementation in File (users.txt):**

Each user has 4 data fields stored:
```
username
password_hash
role
locked_status (0 or 1)
```

**Tracking Mechanism:**

1. **Failed Attempt Counter:**
   - Stored in memory as `User::failedAttempts` (integer)
   - Incremented on each wrong password: `user->incrementFailedAttempts()`
   - Reset to 0 on successful login: `user->resetFailedAttempts()`

2. **Lock Trigger:**
   ```cpp
   void User::incrementFailedAttempts() {
       failedAttempts++;
       if (failedAttempts >= 3) {
           isLocked = true;
       }
   }
   ```

3. **Persistence:**
   - After each login attempt, `saveUsers()` is called
   - Locked status saved to file: `outFile << (user.getLocked() ? 1 : 0) << endl;`
   - Status persists across program restarts

4. **Login Flow with Lock Check:**
   ```cpp
   if (user->getLocked()) {
       // Display lock message
       // Continue to retry (allow trying other accounts)
       return;
   }
   
   if (user->authenticate(password)) {
       user->resetFailedAttempts();
       saveUsers(); // Persist reset
       return user; // Success
   } else {
       user->incrementFailedAttempts();
       saveUsers(); // Persist failed attempt and potential lock
       // Display attempts remaining
   }
   ```

5. **Unlock Mechanism:**
   - Only Admin users can unlock accounts
   - Admin menu option 12: "Unlock User Account"
   - Sets `isLocked = false` and resets failed attempts
   - Immediately saves to file

**Example Scenario:**
```
User: john
Attempt 1: Wrong password → failedAttempts = 1, saved to file
Attempt 2: Wrong password → failedAttempts = 2, saved to file
Attempt 3: Wrong password → failedAttempts = 3, isLocked = true, saved to file
[Account is now locked]
Attempt 4: Even with correct password → Login denied (account locked)
[Admin unlocks account]
Next login: Correct password → Success, failedAttempts reset to 0
```

### C. Data Persistence Security

**File-Based Storage:**
- `users.txt` - Stores username, password hash (not plain text), role, lock status
- `bank_data.txt` - Stores account numbers, holder names, balances
- Both files located in program directory

**Security Measures:**
- ✅ Password hashes stored, never plain text
- ✅ Lock status persists across sessions
- ✅ Automatic save after critical operations (login, register, unlock)
- ✅ Duplicate username prevention on registration
- ✅ Role-based file access (only admins can modify users)

---

## 5. ROLE-BASED ACCESS CONTROL

### Admin Role Features
- Full banking operations (create, deposit, withdraw, delete accounts)
- View system logs and statistics
- User management (view all users and their status)
- Register new users with any role
- Unlock locked user accounts
- Export data to JSON

### User Role Features
- Create bank accounts
- Deposit and withdraw money
- Check balances and view transaction history
- List all accounts
- Export data to JSON
- **Cannot:** Manage users, view system logs, unlock accounts

### Guest Role Features (View-Only)
- Check account balances
- View transaction history
- List all accounts
- **Cannot:** Create accounts, deposit, withdraw, or modify any data

---

## 6. SYSTEM REQUIREMENTS

### Software Requirements
- C++ Compiler (MSVC, g++, or Clang)
- Visual Studio 2017 or later (recommended)
- Windows 10/11 or compatible OS
- Standard C++ Library (STL)

### Hardware Requirements
- Minimum 512MB RAM
- 50MB free disk space
- Standard console/terminal

---

## 7. COMPILATION & EXECUTION

### Using Visual Studio:
1. Open `BankingSystem.sln`
2. Build → Rebuild Solution (Ctrl+Shift+B)
3. Run → Start Without Debugging (Ctrl+F5)

### Using g++ (Command Line):
```bash
g++ -o banking.exe main.cpp BankAccount.cpp BankingSystem.cpp User.cpp
./banking.exe
```

---

## 8. DEFAULT CREDENTIALS

```
Admin Account:
  Username: admin
  Password: admin123
  
User Account:
  Username: user1
  Password: pass123
  
Guest Account:
  Username: guest
  Password: guest123
```

**Note:** Delete `users.txt` to regenerate default users with fresh hashes.

---

## 9. FILE STRUCTURE

```
BankingSystem/
├── main.cpp                 # Program entry point
├── BankAccount.h            # Bank account class declaration
├── BankAccount.cpp          # Bank account implementation
├── BankingSystem.h          # Main system class declaration
├── BankingSystem.cpp        # System implementation
├── User.h                   # User class declaration
├── User.cpp                 # User implementation with hashing
├── BankingSystem.sln        # Visual Studio solution file
├── BankingSystem.vcxproj    # Visual Studio project file
├── bank_data.txt            # Persistent bank account data
├── users.txt                # Persistent user credentials (hashed)
├── bank_export.json         # JSON export (generated on demand)
└── README.md                # Project overview
```

---

## 10. CONCLUSION

This Automated Banking System successfully demonstrates secure software development practices including:
- Robust authentication with password hashing
- Brute-force attack prevention with account locking
- Role-based access control for authorization
- Persistent data storage with file I/O
- Comprehensive transaction management

The system meets all security requirements while providing an intuitive user interface for banking operations.

---

**End of Documentation**
