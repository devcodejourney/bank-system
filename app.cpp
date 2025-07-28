#include <bits/stdc++.h>
using namespace std;

const string fileName = "accounts.txt";

class BankAccount{
  private:
    string userName;
    double balance;
    int accountNumber, pin;

  public:
    BankAccount(string userName, double balance, int accountNumber, int pin){
      this->userName = userName;
      this->balance = balance;
      this->accountNumber = accountNumber;
      this->pin = pin;
    }

    string getUserName();
    double getBalance();
    int getAccountNumber();
    int getPin();

    void viewBalance();
    void depositMoney(double amount);
    void withdrawMoney(double amount);
    void transferMony(double amount);

};

vector<BankAccount> accounts;

string BankAccount::getUserName(){
  return userName;
}

double BankAccount::getBalance(){
  return balance;
}

int BankAccount::getAccountNumber(){
  return accountNumber;
}

int BankAccount::getPin(){
  return pin;
}

void BankAccount::viewBalance(){
  cout << "\nYour Balance: " << balance << endl;
}

void BankAccount::depositMoney(double amount){
  if(amount <= 0){
    cout << "\nInvalid amount!" << endl;
    return;
  }

  balance += amount;
}

void BankAccount::withdrawMoney(double amount){
  if(amount <= 0 || amount > balance){
    cout << "\nInvalid or Insuficient amount!" << endl;
    return;
  }

  balance -= amount;
}

void BankAccount::transferMony(double amount){
  if(amount <= 0 || amount > balance){
    cout << "\nInvalid or Insuficient amount!" << endl;
    return;
  }

  balance -= amount;
}

// wait and clear screen
void waitAndClearScreen(){
  cout << "\nPress Enter to continue.";
  cin.ignore();
  cin.get();
  system("cls");
}

// split account details
vector<string> splitLine(string line, char delimiter){
  vector<string> result;
  stringstream ss(line);
  string item;

  while(getline(ss, item, delimiter)){
    result.push_back(item);
  }

  return result;
}

// generate account number;
int generateAccountNumber(){
  ifstream file(fileName);
  string line;
  int count = 1000;

  while (getline (file, line)) {
    count++;
  }

  file.close();
  return count+1;
}

// save accounts
void saveAccounts(){
  ofstream file(fileName);
  
  for(auto &account: accounts){
    file << account.getUserName() << "," << account.getBalance() << "," << account.getAccountNumber() << "," << account.getPin() << "\n";
  }

  file.close();
}

// create a new account
void createAccount(){
  int accountNumber = generateAccountNumber();

  cout << "\n----- Create New Account -----" << endl;

  string userName;
  double amount;
  int pin;

  cin.ignore();
  cout << "Enter Account Holder Name: ";
  getline(cin, userName);
  
  cout << "Enter Initial Deposit Amount: ";
  cin >> amount;

  cout << "Enter A PIN: ";
  cin >> pin;

  BankAccount account(userName, amount, accountNumber, pin);
  accounts.push_back(account);

  // save accounts
  saveAccounts();

  cout << "\nAccount Created Successfully." << endl;
}

// logged in interface
void showLoggedInInterface(){
  cout << "1. View Balance." << endl;
  cout << "2. Deposit Money." << endl;
  cout << "3. Withdraw Money." << endl;
  cout << "4. Transfer Money." << endl;
  cout << "5. Logout" << endl;
}

// looged in profile
void loggedInProfile(BankAccount *account){
  while(true){
    cout << "\n===== Welcome, " << account->getUserName() << " =====" << endl;
    showLoggedInInterface();

    int option;
    cout << "\nChoose option: ";
    cin >> option;

    double amount, accountNumber;
    BankAccount *transferAccount = nullptr;

    switch (option)
    {
    case 1:
      account->viewBalance();
      break;
    case 2:
      cout << "\nEnter Deposit Amount: ";
      cin >> amount;

      account->depositMoney(amount);

      cout << "\nDeposit Successful!" << endl;
      cout << "New Balance: " << account->getBalance() << endl;

      saveAccounts();
      break;
    case 3:
      cout << "\nEnter Withdraw Amount: ";
      cin >> amount;

      account->withdrawMoney(amount);
      
      cout << "\nWithdrawal Successful!" << endl;
      cout << "New Balance: " << account->getBalance() << endl;

      saveAccounts();
      break;
    case 4:
      cout << "\nEnter Receiver's Account Number: ";
      cin >> accountNumber;
      cout << "Enter Transfer Amount: ";
      cin >> amount;

      for(auto &acc: accounts){
        if(acc.getAccountNumber() == accountNumber){
          transferAccount = &acc;
          break;
        }
      }

      if(transferAccount == nullptr){
        cout << "\nInvalid Account Number" << endl;
      }else{
        account->transferMony(amount);
        transferAccount->depositMoney(amount);

        cout << "\nAmount Transfer Successful." << endl;
        cout << "You sent " << amount << " to " << transferAccount->getUserName() << "." << endl;
        cout << "New Balance: " << account->getBalance() << endl;

        saveAccounts();
      }      
      break;
    case 5:
      cout << "\nLog out successful." << endl;
      return;
    default:
      cout << "\nInvalid option choosen!" << endl;
      break;
    }
    waitAndClearScreen();
  }
}

// login user account
void loginAccount(){
  cout << "\n----- Login Your Account -----" << endl;
  
  int accountNumber, pin;
  
  cout << "Enter Your Account Number: ";
  cin >> accountNumber;
  cout << "Enter Your PIN: ";
  cin >> pin;

  BankAccount *account = nullptr;
  
  for(auto &acc: accounts){
    if(acc.getAccountNumber() == accountNumber && acc.getPin() == pin){
      account = &acc;
      break;
    }
  }

  if(account == nullptr){
    cout << "\nInvalid Credentials!" << endl;
    waitAndClearScreen();
    return;
  }

  // redirect logged in profile
  loggedInProfile(account);
}

// laod all accounts
void loadAllAccounts(){
  ifstream file(fileName);
  string line;

  while(getline(file, line)){
    vector<string> result = splitLine(line, ',');
    BankAccount account(result[0], stod(result[1]), stoi(result[2]), stoi(result[3]));
    accounts.push_back(account);
  }

  file.close();
}

// logged out interface
void showLoggedOutInterface(){
  cout << "\n===== Welcome to 'ABC Bank Ltd' =====" << endl;
  cout << "1. Create New Account." << endl;
  cout << "2. Login Your Account." << endl;
  cout << "3. Exit." << endl;
}

// main function
int main(){
  loadAllAccounts();

  while(true){
    showLoggedOutInterface();

    int option;
    cout << "\nChoose option: ";
    cin >> option;

    switch (option){
    case 1:
      createAccount();
      break;
    case 2:
      loginAccount();
      break;
    case 3:
      cout << "\nThank you for using ABC Bank Ltd." << endl;
      exit(0);
    default:
      cout << "\nInvalid option choosen!" << endl;
      break;
    }
    waitAndClearScreen();
  }

  return 0;
}
