# Automated Banking System

A C++ console application for managing bank accounts with features for deposits, withdrawals, and transaction tracking.

## Features

- **Create Account**: Create new bank accounts with unique account numbers
- **Deposit Money**: Add funds to any account
- **Withdraw Money**: Remove funds from accounts (with balance validation)
- **Check Balance**: View current account balance and information
- **Transaction History**: View complete transaction history for any account
- **List All Accounts**: Display all accounts in the system
- **Delete Account**: Remove accounts from the system

## Project Structure

- `BankAccount.h` / `BankAccount.cpp`: Account class with balance and transaction management
- `BankingSystem.h` / `BankingSystem.cpp`: Main banking system with account management
- `main.cpp`: Program entry point

## Compilation

### Using g++:
```bash
g++ -o banking main.cpp BankAccount.cpp BankingSystem.cpp
```

### Using Visual Studio:
1. Create a new Console Application project
2. Add all .h and .cpp files to the project
3. Build and run (Ctrl+F5)

## Usage

Run the compiled program and follow the menu prompts:
1. Create accounts with initial deposits
2. Perform deposits and withdrawals
3. View balances and transaction history
4. Manage multiple accounts

## Example

```
1. Create account for "John Doe" with $1000 initial deposit
2. Deposit $500 to account
3. Withdraw $200 from account
4. View transaction history
5. Check current balance
```

## Notes

- Account numbers start from 1001 and auto-increment
- All monetary amounts use double precision (2 decimal places)
- Transaction history is maintained for each account
- Input validation prevents negative deposits/withdrawals
- Balance validation prevents overdrafts
