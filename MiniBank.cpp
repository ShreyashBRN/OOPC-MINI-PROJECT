#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdio>
using namespace std;

// Custom exception for insufficient balance
class InsufficientFundsException {
public:
string message;
InsufficientFundsException(string msg) : message(msg) {}
};

// Class representing a bank account
class BankAccount {
string name;
int accountNumber;
float balance;

public:
// Constructors
BankAccount() {}
BankAccount(string n, int acc, float bal) {
name = n;
accountNumber = acc;
balance = bal;
}

// Create a new account and write to file
void createAccount() {
cout << "Enter name: ";
cin >> name;
cout << "Enter account number: ";
cin >> accountNumber;
cout << "Enter initial balance: ";
cin >> balance;

ofstream fout("accounts.txt", ios::app);
if (!fout) {
throw runtime_error("Unable to open file.");
}

fout << accountNumber << " " << name << " " << balance << endl;
fout.close();
cout << "Account created successfully!" << endl;
}

// Display the account details
void display() {
cout << "Account Number: " << accountNumber << endl;
cout << "Name: " << name << endl;
cout << "Balance: " << balance << endl;
}

// Used to access private members
int getAccNo() { return accountNumber; }
float getBalance() { return balance; }
string getName() { return name; }

void setBalance(float b) { balance = b; }
};

// Utility: Find account by reading file
bool findAccount(int accNo, BankAccount &acc) {
ifstream fin("accounts.txt");
if (!fin) throw runtime_error("File not found.");

int aNum;
string aName;
float aBal;

while (fin >> aNum >> aName >> aBal) {
if (aNum == accNo) {
acc = BankAccount(aName, aNum, aBal);
fin.close();
return true;
}
}

fin.close();
return false;
}

// Utility: Update account after deposit/withdraw
void updateAccount(int accNo, float amount, bool isDeposit) {
ifstream fin("accounts.txt");
ofstream fout("temp.txt");
if (!fin || !fout) throw runtime_error("File error.");

int aNum;
string aName;
float aBal;
bool found = false;

while (fin >> aNum >> aName >> aBal) {
if (aNum == accNo) {
found = true;

if (!isDeposit && amount > aBal)
throw InsufficientFundsException("Insufficient balance!");

aBal = isDeposit ? aBal + amount : aBal - amount;
}
fout << aNum << " " << aName << " " << aBal << endl;
}

fin.close();
fout.close();

if (!found) throw runtime_error("Account not found.");

remove("accounts.txt");
rename("temp.txt", "accounts.txt");

cout << (isDeposit ? "Deposit" : "Withdrawal") << " successful!" << endl;
}

// Menu for interacting with the system
void menu() {
int choice;
while (true) {
cout << "\n--- Mini Banking System ---\n";
cout << "1. Create Account\n";
cout << "2. Deposit Money\n";
cout << "3. Withdraw Money\n";
cout << "4. Display Account\n";
cout << "5. Exit\n";
cout << "Enter choice: ";
cin >> choice;

try {
if (choice == 1) {
BankAccount acc;
acc.createAccount();
}
else if (choice == 2) {
int accNo;
float amount;
cout << "Enter account number: ";
cin >> accNo;
cout << "Enter amount to deposit: ";
cin >> amount;

if (amount < 0)
throw invalid_argument("Negative deposit not allowed.");

updateAccount(accNo, amount, true);
}
else if (choice == 3) {
int accNo;
float amount;
cout << "Enter account number: ";
cin >> accNo;
cout << "Enter amount to withdraw: ";
cin >> amount;

if (amount < 0)
throw invalid_argument("Negative withdrawal not allowed.");

updateAccount(accNo, amount, false);
}
else if (choice == 4) {
int accNo;
cout << "Enter account number: ";
cin >> accNo;
BankAccount acc;
if (findAccount(accNo, acc)) {
acc.display();
} else {
cout << "Account not found." << endl;
}
}
else if (choice == 5) {
cout << "Thank you for using the system!" << endl;
break;
}
else {
cout << "Invalid choice." << endl;
}
} catch (InsufficientFundsException &e) {
cout << "Error: " << e.message << endl;
} catch (exception &e) {
cout << "Error: " << e.what() << endl;
}
}
}

int main() {
menu();
return 0;
}