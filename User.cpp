#include "User.h"
#include <sstream>
#include <iomanip>

// Simple hash function (for educational purposes - use SHA256 in production)
string User::hashPassword(const string& password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    // Convert to hex string
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hash;
    return ss.str();
}

// Constructor
User::User(string uname, string pwdHash, UserRole r, bool locked)
    : username(uname), passwordHash(pwdHash), role(r), isLocked(locked), failedAttempts(0) {}

// Getters
string User::getUsername() const {
    return username;
}

string User::getPasswordHash() const {
    return passwordHash;
}

UserRole User::getRole() const {
    return role;
}

bool User::getLocked() const {
    return isLocked;
}

int User::getFailedAttempts() const {
    return failedAttempts;
}

// Setters
void User::setLocked(bool locked) {
    isLocked = locked;
}

void User::incrementFailedAttempts() {
    failedAttempts++;
    if (failedAttempts >= 3) {
        isLocked = true;
    }
}

void User::resetFailedAttempts() {
    failedAttempts = 0;
}

// Authenticate user
bool User::authenticate(string pwd) const {
    if (isLocked) {
        return false;
    }
    return passwordHash == hashPassword(pwd);
}

// Get role name as string
string User::getRoleName() const {
    switch (role) {
        case ADMIN: return "Admin";
        case USER: return "User";
        case GUEST: return "Guest";
        default: return "Unknown";
    }
}
