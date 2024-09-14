#include <iostream>
#include <math.h>
#include <mysql.h>
#include <cstdlib>
#include <iomanip> // For formatting output
#include <ctime>   // For getting the current date
#include <bits/stdc++.h>

using namespace std;

void homePage();


class Database {
protected:
    MYSQL *conn;

    // Function to initialize MySQL connection
    void initMySQL() {
        conn = mysql_init(NULL);
        if (conn == NULL) {
            cerr << "MySQL initialization failed" << endl;
            exit(1);
        }

        // Connect to the MySQL database
        if (mysql_real_connect(conn, "localhost", "root", "harsh", "bank_system", 0, NULL, 0) == NULL) {
            cerr << "MySQL connection failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            exit(1);
        }
    }

    // Function to execute an SQL query


public:
    Database() {
        initMySQL();
    }

    ~Database() {
        mysql_close(conn);
    }

        bool executeQuery(const string &query) {
        if (mysql_query(conn, query.c_str())) {
            cerr << "Query execution failed: " << mysql_error(conn) << endl;
            return false;
        }
        return true;
    }
};

// User class to handle user-related operations
class User : public Database {
private:
    // Function to check login credentials
    bool checkLoginCredentials(const string &login_id, const string &password) {
        string query = "SELECT account_no FROM login_details_user WHERE login_id = '" + login_id + "' AND password = '" + password + "'";
        if (!executeQuery(query)) return false;

        MYSQL_RES *res = mysql_store_result(conn);
        if (res == NULL) {
            cerr << "Failed to retrieve login details: " << mysql_error(conn) << endl;
            return false;
        }

        bool isValid = (mysql_num_rows(res) > 0);
        mysql_free_result(res);
        return isValid;
    }

    // Function to view the user's profile
    void viewProfile(const string &account_no) {
        string query = "SELECT * FROM main_account WHERE account_no = '" + account_no + "'";
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n--- User Profile ---\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "User ID: " << row[0] << "\nName: " << row[1] << "\nDOB: " << row[2]
                 << "\nAccount Number: " << row[3] << "\nAccount Type: " << row[4]
                 << "\nDate of Account Opening: " << row[5] << "\nBalance: " << row[6]
                 << "\nInterest Rate: " << row[7] << "\nEmail: " << row[10]
                 << "\nAddress: " << row[11] << "\nPhone Number: " << row[12]
                 << "\nAadhar Number: " << row[13] << "\nPAN Number: " << row[14] << endl;
        }
        mysql_free_result(res);
    }

    // Function to handle withdrawal
    void withdraw(const string &account_no) {
        double amount;
        cout << "Enter amount to withdraw: ";
        cin >> amount;

        if(amount<0)
        {
            cout<<"Invalid amount please enter correct amount"<<endl;
            return withdraw(account_no);
        }

        string query = "SELECT balance FROM main_account WHERE account_no = '" + account_no + "'";
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        double balance = stod(row[0]);

        if (amount > balance) {
            cout << "Insufficient balance!" << endl;
        } else {
            balance -= amount;
            query = "UPDATE main_account SET balance = " + to_string(balance) + " WHERE account_no = '" + account_no + "'";
            if (executeQuery(query)) {
                cout << "Withdrawal successful. New balance: " << fixed << setprecision(2) << balance << endl;
            }
        }
        mysql_free_result(res);
    }

    // Function to handle deposit
    void deposit(const string &account_no) {
        double amount;
        cout << "Enter amount to deposit: ";
        cin >> amount;

        if(amount<0)
        {
            cout<<"Enter Valid amount that should be greater than 0"<<endl;
            return deposit(account_no);
        }

        string query = "SELECT balance FROM main_account WHERE account_no = '" + account_no + "'";
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        double balance = stod(row[0]);

        balance += amount;
        query = "UPDATE main_account SET balance = " + to_string(balance) + " WHERE account_no = '" + account_no + "'";
        if (executeQuery(query)) {
            cout << "Deposit successful. New balance: " << fixed << setprecision(2) << balance << endl;
        }

        mysql_free_result(res);
    }

    // Function to modify account details
    void modifyAccount(const string &account_no) {
        int choice;
        string newValue;
        cout << "What would you like to modify?\n1. Phone Number\n2. Name\n3. Address\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter new phone number: ";
                cin >> newValue;
                if (newValue.length() != 10) {
                    cout << "Invalid input data. Please make sure phone number has 10 digits." << endl;
                    return;
                }
                executeQuery("UPDATE main_account SET phone_number = '" + newValue + "' WHERE account_no = '" + account_no + "'");
                break;
            case 2:
                cout << "Enter new name: ";
                cin >> newValue;
                executeQuery("UPDATE main_account SET user_name = '" + newValue + "' WHERE account_no = '" + account_no + "'");
                break;
            case 3:
                cout << "Enter new address: ";
                cin.ignore(); // Clear the input buffer
                getline(cin, newValue);
                executeQuery("UPDATE main_account SET address = '" + newValue + "' WHERE account_no = '" + account_no + "'");
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
        cout << "Account details updated successfully!" << endl;
    }

    // Function to handle user signup
    void signup() {
        string account_type;
        double balance = 0.0;
        double interest_rate = 0.0;
        srand(time(0));
        long long account_no = rand();  // Starting value for account numbers

        // Input details
        cout << "Select account type (1 for saving, 2 for current): ";
        int type_choice;
        cin >> type_choice;

        switch (type_choice){

        case 1:
            account_type = "saving";
            interest_rate = 3.0;
            break;
        case 2:
             account_type = "current";
            balance = 1000.0;  // Minimum balance for current accounts
            interest_rate = 0.0;
            break;

        default:
            cout<<"enter the Correct Choice"<<endl;
            signup();
            return;
        }

   /*     if (type_choice == 1) {
            account_type = "saving";
            interest_rate = 3.0;
        } else if (type_choice == 2) {
            account_type = "current";
            balance = 1000.0;  // Minimum balance for current accounts
            interest_rate = 0.0;

        } else {
            cout << "Invalid account type choice. Please enter 1 for saving or 2 for current." << endl;
            return userMenu();
        }
    */

        string user_name, dob, email, address, phone_number, aadhar_no, pan_no;
        cout << "Enter your details:\n";
        cout << "Name: ";
        cin.ignore();
        getline(cin, user_name);

        cout << "Date of Birth (YYYY-MM-DD): ";
        cin >> dob;
        cout << "Email: ";
        cin >> email;
        cout << "Address: ";
        cin.ignore(); // Clear the input buffer
        getline(cin, address);
        cout << "Phone Number (10 digits): ";
        cin >> phone_number;
        cout << "Aadhar Number (12 digits): ";
        cin >> aadhar_no;
        cout << "PAN Number (10 characters): ";
        cin >> pan_no;




        //auto currentDate = getCurrentDate();
        // Perform basic validation checks

 if (email.find('@') != string::npos && email.front() != '@' && email.back() != '@') {

} else {
       cout<<"Email is invalid"<<endl;

       return signup();
    }


        if(dob>="2024-09-13"){
            cout<<"Invalid date"<<endl;
            return signup() ;
        }
        if (phone_number.length() != 10 || aadhar_no.length() != 12 || pan_no.length() != 10) {
            cout << "Invalid input data. Please make sure phone number has 10 digits, Aadhar number has 12 digits, and PAN number has 10 characters." << endl;
            return signup();
        }

        // Insert new user data into the main_account table
        string query = "INSERT INTO main_account (user_name, dob, account_no, account_type, date_of_acc_opening, balance, interest_rate, last_deposit, last_withdrawal, email, address, phone_number, aadhar_no, pan_no) VALUES ('" + user_name + "', '" + dob + "', " + to_string(account_no) + ", '" + account_type + "', CURDATE(), " +
            to_string(balance) + ", " + to_string(interest_rate) + ", 0, 0, '" + email + "', '" + address + "', '" + phone_number + "', '" + aadhar_no + "', '" + pan_no + "')";

        if (executeQuery(query)) {
            cout << "Signup successful! Your account number is: " << account_no << endl;

            // Insert login details into the login_details_user table
            string login_id, password;

            cout << "create your login ID: ";
            cin >> login_id;
            cout << "create your password: ";
            cin >> password;

            // Insert login details into the login_details_user table
            query = "INSERT INTO login_details_user (login_id, password, account_no) VALUES ('" +
                login_id + "', '" + password + "', " + to_string(account_no) + ")";

            if (executeQuery(query)) {
                cout << "Login details added successfully!" << endl;
                userMenu();
            }
        }
    }

public:

    void loginMenu(string account_no){
            int user_choice;
                    cout << "1. View Profile\n2. Deposit\n3. Withdraw\n4. Modify Account\n5. Exit\n";
                    cin >> user_choice;

                    switch (user_choice) {
                        case 1:
                            viewProfile(account_no);
                            loginMenu(account_no);
                            break;

                        case 2:
                            deposit(account_no);
                            loginMenu(account_no);
                            break;
                        case 3:
                            withdraw(account_no);
                            loginMenu(account_no);
                            break;
                        case 4:
                            modifyAccount(account_no);
                            loginMenu(account_no);
                            break;
                        case 5:
                            exit(0);
                            userMenu();
                    }
        }
    void userMenu() {
        string login_id, password;



        cout << "User Menu:\n";
        cout << "1. Login\n2. Signup\n3. Exit\n";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter login ID: ";
                cin >> login_id;
                cout << "Enter password: ";
                cin >> password;
                if (checkLoginCredentials(login_id, password)) {
                        long long acc;
                cout<<"Enter account No"<<endl;
                cin >> acc;

                string account_no=to_string(acc);
                cout<<endl;
                    int user_choice;
                    cout << "1. View Profile\n2. Deposit\n3. Withdraw\n4. Modify Account\n5. Exit\n";
                    cin >> user_choice;

                    switch (user_choice) {
                        case 1:
                            viewProfile(account_no);
                            loginMenu(account_no);
                            break;

                        case 2:
                            deposit(account_no);
                            loginMenu(account_no);
                            break;
                        case 3:
                            withdraw(account_no);
                            loginMenu(account_no);
                            break;
                        case 4:
                            modifyAccount(account_no);
                            loginMenu(account_no);
                            break;
                        case 5:
                            userMenu();
                        default:
                            cout<<"invalid input"<<endl;
                            loginMenu(account_no);
                    }
                } else {
                    cout << "Invalid login credentials!" << endl;
                    userMenu();
                }
                break;

            case 2:
                signup();
                break;

            case 3:
                homePage();
                break;
            default:
                cout<<"invalid input"<<endl;
                userMenu();
        }
    }
};

// Banker class to handle banker-related operations
class Banker : public Database{
private:
bool checkLoginCredentials(const string &login_id, const string &password) {
        string query = "SELECT employee_id FROM login_details_banker WHERE login_id = '" + login_id + "' AND password = '" + password + "'";
        if (!executeQuery(query)) return false;

        MYSQL_RES *res = mysql_store_result(conn);
        if (res == NULL) {
            cerr << "Failed to retrieve login details: " << mysql_error(conn) << endl;
            return false;
        }

        bool isValid = (mysql_num_rows(res) > 0);
        mysql_free_result(res);
        return isValid;
    }
    // Function to add a new user

    void loginpage()
    {
        int banker_choice;
                    cout << "1. Add User\n2. View All User Details\n3. View Specific User Details\n4. Exit\n";
                    cin >> banker_choice;

                    switch (banker_choice) {
                        case 1:
                            addUser();
                            loginpage();
                            break;
                        case 2:
                            viewAllDetails();
                            loginpage();
                            break;
                        case 3:
                            viewSpecificUserDetails();
                            loginpage();
                            break;
                        case 4:
                            homePage();
                            break;
                        default :
                            cout<<"Invalid choice"<<endl;
                            homePage();
                    }
    }
    void addUser() {
        string account_type;
        double balance = 0.0;
        double interest_rate = 0.0;
        srand(time(0));
        long long account_no = rand();   // Starting value for account numbers


        // Input details
        cout << "Select account type (1 for saving, 2 for current): ";
        int type_choice;
        cin >> type_choice;

        switch (type_choice){

        case 1:
            account_type = "saving";
            interest_rate = 3.0;
            break;
        case 2:
             account_type = "current";
            balance = 1000.0;  // Minimum balance for current accounts
            interest_rate = 0.0;
            break;

        default:
            cout<<"enter the Correct Choice"<<endl;
            addUser();
            return;
        }

        string user_name, dob, email, address, phone_number, aadhar_no, pan_no;
        cout << "Enter your details:\n";
        cout << "Name: ";
        cin.ignore();
        getline(cin, user_name);

        cout << "Date of Birth (YYYY-MM-DD): ";
        cin >> dob;
        cout << "Email: ";
        cin >> email;
        cout << "Address: ";
        cin.ignore(); // Clear the input buffer
        getline(cin, address);
        cout << "Phone Number (10 digits): ";
        cin >> phone_number;
        cout << "Aadhar Number (12 digits): ";
        cin >> aadhar_no;
        cout << "PAN Number (10 characters): ";
        cin >> pan_no;




        //auto currentDate = getCurrentDate();
        // Perform basic validation checks

 if (email.find('@') != string::npos && email.front() != '@' && email.back() != '@') {

} else {
       cout<<"Email is invalid"<<endl;

       return addUser();
    }


        if(dob>="2024-09-13"){
            cout<<"Invalid date"<<endl;
            return addUser() ;
        }
        if (phone_number.length() != 10 || aadhar_no.length() != 12 || pan_no.length() != 10) {
            cout << "Invalid input data. Please make sure phone number has 10 digits, Aadhar number has 12 digits, and PAN number has 10 characters." << endl;
            return addUser();
        }

        // Insert new user data into the main_account table
        string query = "INSERT INTO main_account (user_name, dob, account_no, account_type, date_of_acc_opening, balance, interest_rate, last_deposit, last_withdrawal, email, address, phone_number, aadhar_no, pan_no) VALUES ('" + user_name + "', '" + dob + "', " + to_string(account_no) + ", '" + account_type + "', CURDATE(), " +
            to_string(balance) + ", " + to_string(interest_rate) + ", 0, 0, '" + email + "', '" + address + "', '" + phone_number + "', '" + aadhar_no + "', '" + pan_no + "')";

        if (executeQuery(query)) {
            cout << "Signup successful! Your account number is: " << account_no << endl;

            // Insert login details into the login_details_user table
            string login_id, password;

            cout << "create your login ID: ";
            cin >> login_id;
            cout << "create your password: ";
            cin >> password;

            // Insert login details into the login_details_user table
            query = "INSERT INTO login_details_user (login_id, password, account_no) VALUES ('" +
                login_id + "', '" + password + "', " + to_string(account_no) + ")";

            if (executeQuery(query)) {
                cout << "Login details added successfully!" << endl;
                bankerMenu();
            }
        }
    }

    // Function to view all user details
    void viewAllDetails() {
        string query = "SELECT * FROM main_account";
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n--- All User Details ---\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "User ID: " << row[0] << "\nName: " << row[1] << "\nDOB: " << row[2]
                 << "\nAccount Number: " << row[3] << "\nAccount Type: " << row[4]
                 << "\nDate of Account Opening: " << row[5] << "\nBalance: " << row[6]
                 << "\nInterest Rate: " << row[7] << "\nEmail: " << row[10]
                 << "\nAddress: " << row[11] << "\nPhone Number: " << row[12]
                 << "\nAadhar Number: " << row[13] << "\nPAN Number: " << row[14] << endl << endl;
        }
        mysql_free_result(res);
    }

    // Function to view specific user's details
    void viewSpecificUserDetails() {
        int user_id;
        cout << "Enter the User ID: ";
        cin >> user_id;

        string query = "SELECT * FROM main_account WHERE user_id = " + to_string(user_id);
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;

        if ((row = mysql_fetch_row(res))) {
            cout << "\n--- User Details ---\n";
            cout << "User ID: " << row[0] << "\nName: " << row[1] << "\nDOB: " << row[2]
                 << "\nAccount Number: " << row[3] << "\nAccount Type: " << row[4]
                 << "\nDate of Account Opening: " << row[5] << "\nBalance: " << row[6]
                 << "\nInterest Rate: " << row[7] << "\nEmail: " << row[10]
                 << "\nAddress: " << row[11] << "\nPhone Number: " << row[12]
                 << "\nAadhar Number: " << row[13] << "\nPAN Number: " << row[14] << endl;
        } else {
            cout << "User not found!" << endl;
        }
        mysql_free_result(res);
    };
public:
    void bankerMenu() {
        int choice;
        cout << "Banker Menu:\n";
        cout << "1. Login\n2. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1: {
                string login_id, password;
                cout << "Enter login ID: ";
                cin >> login_id;
                cout << "Enter password: ";
                cin >> password;
                if (checkLoginCredentials(login_id, password)) {
                    int banker_choice;
                    cout << "1. Add User\n2. View All User Details\n3. View Specific User Details\n4. Exit\n";
                    cin >> banker_choice;

                    switch (banker_choice) {
                        case 1:
                            addUser();
                            loginpage();
                            break;
                        case 2:
                            viewAllDetails();
                            loginpage();
                            break;
                        case 3:
                            viewSpecificUserDetails();
                            loginpage();
                            break;
                        case 4:
                            homePage();
                            break;

                        default :
                            cout<<"Invalid choice"<<endl;
                            loginpage();


                    }
                } else {
                    cout << "Invalid login credentials!" << endl;
                    bankerMenu();
                    }
    }
    case 2:
        homePage();
        ;
    break;}
}
};
// Admin class to handle admin-related operations
class Admin : public Database {
private:

    bool checkLoginCredentials(const string &login_id, const string &password) {
        string query = "SELECT admin_id FROM login_details_admin WHERE login_id = '" + login_id + "' AND login_password = '" + password + "'";
        if (!executeQuery(query)) return false;

        MYSQL_RES *res = mysql_store_result(conn);
        if (res == NULL) {
            cerr << "Failed to retrieve login details: " << mysql_error(conn) << endl;
            return false;
        }

        bool isValid = (mysql_num_rows(res) > 0);
        mysql_free_result(res);
        return isValid;
    }
    // Function to view all bankers
    void viewAllBankers() {
        string query = "SELECT * FROM login_details_banker";
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n--- All Bankers ---\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "Employee ID: " << row[0] << "\nName: " << row[1] << "\nDOB: " << row[2]
                 << "\nAddress: " << row[3] << "\nPhone Number: " << row[4]
                 << "\nAadhar Number: " << row[5] << "\nEmail: " << row[6]
                 << "\nLogin ID: " << row[7] << "\nLogin Password: " << row[8] << endl << endl;
        }
        mysql_free_result(res);
    }

    // Function to view all accounts
    void viewAllAccounts() {
        string query = "SELECT * FROM main_account";
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "\n--- All Accounts ---\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "User ID: " << row[0] << "\nName: " << row[1] << "\nDOB: " << row[2]
                 << "\nAccount Number: " << row[3] << "\nAccount Type: " << row[4]
                 << "\nDate of Account Opening: " << row[5] << "\nBalance: " << row[6]
                 << "\nInterest Rate: " << row[7] << "\nEmail: " << row[10]
                 << "\nAddress: " << row[11] << "\nPhone Number: " << row[12]
                 << "\nAadhar Number: " << row[13] << "\nPAN Number: " << row[14] << endl << endl;
        }
        mysql_free_result(res);
    }

    // Function to manage bankers
    void manageBankers() {
        viewAllBankers();
        int banker_id;
        cout << "Enter the Employee ID of the banker to modify: ";
        cin >> banker_id;
        string query = "SELECT * FROM login_details_banker WHERE employee_id = " + to_string(banker_id);
        if (!executeQuery(query)) return;

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        if ((row = mysql_fetch_row(res))) {
            cout << "\n--- Banker Details ---\n";
            cout << "Employee ID: " << row[0] << "\nName: " << row[1] << "\nDOB: " << row[2]
                 << "\nAddress: " << row[3] << "\nPhone Number: " << row[4]
                 << "\nAadhar Number: " << row[5] << "\nEmail: " << row[6]
                 << "\nLogin ID: " << row[7] << "\nLogin Password: " << row[8] << endl;

            int choice;
            cout << "What would you like to modify?\n1. Address\n2. Phone Number\n3. Email\n4. Exit\nEnter your choice: ";
            cin >> choice;

            string newValue;
            switch (choice) {
                case 1:
                    cout << "Enter new address: ";
                    cin.ignore(); // Clear the input buffer
                    getline(cin, newValue);
                    query = "UPDATE login_details_banker SET address = '" + newValue + "' WHERE employee_id = " + to_string(banker_id);
                    break;
                case 2:
                    cout << "Enter new phone number: ";
                    cin >> newValue;
                    if (newValue.length() != 10) {
                        cout << "Invalid input. Phone number must be 10 digits." << endl;
                        return;
                    }
                    query = "UPDATE login_details_banker SET phone_number = '" + newValue + "' WHERE employee_id = " + to_string(banker_id);
                    break;
                case 3:
                    cout << "Enter new email: ";
                    cin >> newValue;
                    query = "UPDATE login_details_banker SET email = '" + newValue + "' WHERE employee_id = " + to_string(banker_id);
                    break;
                case 4:
                    loginMenu();
                    ;
                default:
                    cout << "Invalid choice!" << endl;
                    loginMenu() ;
            }
            if (executeQuery(query)) {
                cout << "Banker details updated successfully!" << endl;
                loginMenu();
            }
        } else {
            cout << "Banker not found!" << endl;
            loginMenu();

        }
        mysql_free_result(res);
    }

    // Function to add a new banker
    void addBanker() {
        string name, dob, address, phone_number, aadhar_no, email, login_id, password;
        cout << "Enter new banker details:\n";
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Date of Birth (YYYY-MM-DD): ";
        cin >> dob;
        cout << "Address: ";
        cin.ignore(); // Clear the input buffer
        getline(cin, address);
        cout << "Phone Number (10 digits): ";
        cin >> phone_number;
        cout << "Aadhar Number (12 digits): ";
        cin >> aadhar_no;
        cout << "Email: ";
        cin >> email;
        cout << "Login ID: ";
        cin >> login_id;
        cout << "Login Password: ";
        cin >> password;

        if (phone_number.length() != 10 || aadhar_no.length() != 12) {
            cout << "Invalid input data. Please ensure the phone number has 10 digits and Aadhar number has 12 digits." << endl;
            return;
        }

        // Insert new banker data into the login_details_banker table
        string query = "INSERT INTO login_details_banker (employee_name, dob, address, phone_number, aadhar_no, email, login_id, login_password) VALUES ('" +
            name + "', '" + dob + "', '" + address + "', '" + phone_number + "', '" + aadhar_no + "', '" + email + "', '" + login_id + "', '" + password + "')";

        if (executeQuery(query)) {
            cout << "Banker added successfully!" << endl;
        }
    }

    void deleteAccount(){

        long long acc;
        cout<<"Enter the account you want to delelte from the database"<<endl;
        cin>>acc;

        string accNo=to_string(acc);

      //  string query="set FOREIGN_KEY_CHECKS = 0; Delete from main_account where account_no = "+accNo+"; SET FOREIGN_KEY_CHECKS = ;";

        string query="delete from login_details_user where account_no = "+accNo+";";
        string query2="Delete from main_account where account_no = "+accNo+";";


        if(executeQuery(query))
        {
            if(executeQuery(query2))
            {
                cout<<"Account deleted from the database"<<endl;
            }

        }
        else
        {
            cout<<"No Such Account is found"<<endl;
        }
        return;

    }

public:
    void loginMenu(){
        int choice;
        cout << "1. View All Bankers\n2. Manage Bankers\n3. View All Accounts\n4. Add Banker\n5. Delete Account \n 6.Exit\n";
                cin >> choice;

                switch (choice) {
                case 1:
                viewAllBankers();
                loginMenu();

                break;
                case 2:
                manageBankers();
                loginMenu();

                break;
                case 3:
                viewAllAccounts();
                loginMenu();

                break;
                case 4:
                addBanker();
                loginMenu();

                break;

                case 5:
                    deleteAccount();
                    loginMenu();
                    break;
                case 6:
                homePage();
                default:
                printf("INVALID CHOICE... PLEASE ENTER AGAIN...");
                adminMenu();
        }

    }
    void adminMenu() {

        int choice;
        cout << "Admin Menu:\n";
        cout << "1. Login\n2. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1: {
                string login_id, password;
                cout << "Enter login ID: ";
                cin >> login_id;
                cout << "Enter password: ";
                cin >> password;
                if (checkLoginCredentials(login_id, password)) {

                cout << "1. View All Bankers\n2. Manage Bankers\n3. View All Accounts\n4. Add Banker\n5. Delete Records\n 6. Exit\n";
                cin >> choice;

                switch (choice) {
                case 1:
                viewAllBankers();
                loginMenu();
                break;
                case 2:
                manageBankers();
                loginMenu();
                break;
                case 3:
                viewAllAccounts();
                loginMenu();
                break;
                case 4:
                addBanker();
                loginMenu();
                break;
                case 5:
                    deleteAccount();
                    loginMenu();
                    break;
                case 6:
                homePage();
                default:
                printf("INVALID CHOICE... PLEASE ENTER AGAIN...");
                adminMenu();
        }

                    }
             else {
                    cout << "Invalid login credentials!" << endl;
                    adminMenu();
                    }
    }
    case 2:
        homePage();
        ;
    break;}

}
};

void homePage()
{
    int role;
    cout << "Select your role:\n1. User\n2. Banker\n3. Admin\n4. Exit\nEnter your choice: ";
    cin >> role;

    if (role == 1) {
        User user;
        user.userMenu();
    } else if (role == 2) {
        Banker banker;
        banker.bankerMenu();
    } else if (role == 3) {
        Admin admin;
        admin.adminMenu();
    }
    else if(role ==4)
    {
        return;
    }
     else {
        cout << "Invalid role selected!" << endl;
        homePage();
    }

}


// Main function to drive the application
int main() {

    homePage();


    return 0;
}
