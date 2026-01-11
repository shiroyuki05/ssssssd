#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

enum UserRole {
    ADMIN,
    USER,
    GUEST
};

class User {
private:
    string username;
    string passwordHash;  // Stored as hash, not plain text
    UserRole role;
    bool isLocked;
    int failedAttempts;

public:
    // Constructor
    User(string uname, string pwdHash, UserRole r, bool locked = false);
    
    // Getters
    string getUsername() const;
    string getPasswordHash() const;
    UserRole getRole() const;
    bool getLocked() const;
    int getFailedAttempts() const;
    
    // Setters
    void setLocked(bool locked);
    void incrementFailedAttempts();
    void resetFailedAttempts();
    
    // Authentication
    bool authenticate(string pwd) const;
    
    // Role name converter
    string getRoleName() const;
    
    // Static hash function
    static string hashPassword(const string& password);
};

#endif
