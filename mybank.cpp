#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

// made with love from vikash ❤️

using namespace std;

struct UserData
{
    string username;
    int pin;
    int amount;
};

// Declare the function to submit a suggestion/query
void submitQuery(const UserData &user);

void updateUserData(const UserData &user)
{
    ifstream inputFile("user_data.txt");
    ofstream outputFile("temp.txt");

    UserData tempUser;
    while (inputFile >> tempUser.username >> tempUser.pin >> tempUser.amount)
    {
        if (tempUser.username == user.username && tempUser.pin == user.pin)
        {
            outputFile << user.username << " " << user.pin << " " << user.amount << endl;
        }
        else
        {
            outputFile << tempUser.username << " " << tempUser.pin << " " << tempUser.amount << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    remove("user_data.txt");
    rename("temp.txt", "user_data.txt");
}

void displayWelcomeMessage()
{
    cout << "Welcome to Our Bank!" << endl;
}

bool userHasAccount()
{
    char choice;
    cout << "Do you have an account? (y/n): ";
    cin >> choice;
    return (choice == 'y' || choice == 'Y');
}

UserData loginUser()
{
    UserData user;
    cout << "Enter your username: ";
    cin >> user.username;
    cout << "Enter your 6-digit PIN: ";
    cin >> user.pin;
    return user;
}

void createAccount()
{
    UserData user;
    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, user.username);
    cout << "Set your 6-digit PIN: ";
    cin >> user.pin;
    user.amount = 0;

    ofstream file("user_data.txt", ios::app);
    file << user.username << " " << user.pin << " " << user.amount << endl;
    file.close();

    cout << "Account created successfully!" << endl;
}

void performTransaction(UserData &user);

int main()
{
    displayWelcomeMessage();

    bool hasAccount = userHasAccount();
    UserData currentUser;

    if (hasAccount)
    {
        currentUser = loginUser();

        ifstream file("user_data.txt");
        bool userFound = false;
        UserData tempUser;

        while (file >> tempUser.username >> tempUser.pin >> tempUser.amount)
        {
            if (tempUser.username == currentUser.username && tempUser.pin == currentUser.pin)
            {
                userFound = true;
                currentUser = tempUser;
                break;
            }
        }
        file.close();

        if (!userFound)
        {
            cout << "Invalid credentials. Exiting the program." << endl;
            return 1;
        }

        cout << "Login successful. Welcome back, " << currentUser.username << "!" << endl;
    }
    else
    {
        char createAccountChoice;
        cout << "Do you want to create an account? (y/n): ";
        cin >> createAccountChoice;

        if (createAccountChoice == 'y' || createAccountChoice == 'Y')
        {
            createAccount();
        }
        else
        {
            cout << "Exiting the program. Thank you for considering our bank!" << endl;
            return 0;
        }
    }

    while (true)
    {
        performTransaction(currentUser);
        if (currentUser.amount < 0)
        {
            cout << "Warning: Your account is overdrawn!" << endl;
        }
        cout << "----------------------" << endl;
    }

    return 0;
}

// Function to submit a suggestion/query
void submitQuery(const UserData &user)
{
    cin.ignore(); // Clear the newline character from the buffer
    string query;
    cout << "Enter your suggestion or query: ";
    getline(cin, query);

    ofstream queryFile("userquery.txt", ios::app);
    if (queryFile.is_open())
    {
        queryFile << user.username << ": " << query << endl;
        cout << "Thank you for your suggestion/query. We will review it." << endl;
        queryFile.close();
    }
    else
    {
        cout << "Error opening query file." << endl;
    }
}

// Function to perform banking transactions
void performTransaction(UserData &user)
{
    int opt;
    cout << "----------------------" << endl;
    cout << "My Own Bank: " << endl;
    cout << "1. To check balance" << endl;
    cout << "2. To deposit money" << endl;
    cout << "3. To withdraw money" << endl;
    cout << "4. To submit a suggestion/query" << endl;
    cout << "5. To request a loan (under development)" << endl;
    cout << "6. To exit" << endl;
    cout << "-----------------------" << endl;
    cout << "Enter your option: ";
    cin >> opt;

    switch (opt)
    {
    case 1:
        cout << "Your balance: " << user.amount << " r" << endl;
        break;
    case 2:
        int depamt;
        cout << "Enter amount you want to deposit: ";
        cin >> depamt;
        user.amount += depamt;
        cout << depamt << " r have been successfully deposited." << endl;
        break;
    case 3:
        int withdrawamt;
        cout << "Enter amount you want to withdraw: ";
        cin >> withdrawamt;
        if (withdrawamt > user.amount)
        {
            cout << "Insufficient funds!" << endl;
        }
        else
        {
            user.amount -= withdrawamt;
            cout << withdrawamt << " r have been successfully withdrawn." << endl;
        }
        break;
    case 4:
        submitQuery(user);
        break;
    case 5:
        cout << "Loan option is currently under development. Check back soon!" << endl;
        break;
    case 6:
        cout << "Exiting the program. Thank you for using our bank!" << endl;
        updateUserData(user);
        exit(0);
    default:
        cout << "Invalid option. Please try again." << endl;
        break;
    }

    updateUserData(user);
}
