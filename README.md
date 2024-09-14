# # Bank Management System

This project is **Bank Management System** developed in C++ with database integration. It simulates a real-world banking system and provides a simple, command-line-based interface. The system offers different roles with distinct privileges: **User**, **Banker**, and **Admin**, each with unique access to banking functionalities.

### Role-Based Access:
1. **User:** Regular customers who can manage their bank accounts.
2. **Banker:** Bank staff responsible for managing user accounts.
3. **Admin:** The highest level of access, with control over bankers and full account visibility.
4. **Exit:** To quit the application.

### Features by Role:

#### 1. **User:**
   - **Login:** Users can log in using their login ID, password, and account number.
   - **View Profile:** Display personal and account details.
   - **Deposit:** Add funds to the account.
   - **Withdraw:** Withdraw funds from the account.
   - **Modify Account:** Update personal details or account information.
   - **Signup:** New users can register by providing their basic details.
   - **Exit:** Exit the user menu.

#### 2. **Banker:**
   - **Login:** Bankers log in using their credentials.
   - **Add User:** Register new users in the system.
   - **View All User Details:** Access information about all users.
   - **View Specific User Details:** Look up details of a specific user.
   - **Exit:** Exit the banker menu.

#### 3. **Admin:**
   - **Login:** Admin logs in with higher-level credentials.
   - **View All Bankers:** See a list of all bankers.
   - **Manage Bankers:** Add or remove bankers.
   - **View All Accounts:** Access details of all user accounts.
   - **Add Banker:** Add new bankers to the system.
   - **Delete Records:** Remove user or banker records from the database.
   - **Exit:** Exit the admin menu.

#### 4. **Exit:** Close the application.

### Key Features:
- **C++ and Database Integration:** The project is built using C++ and connected to a relational database (e.g., MySQL) for managing user, banker, and admin data.
- **Role-Based Access Control:** The system ensures that different types of users can only perform actions specific to their roles.
- **SQL Queries:** All banking actions like deposits, withdrawals, and record management are executed through SQL queries, ensuring data integrity and security.

### How to Run:
1. Clone the repository.
2. Set up and configure the required database (e.g., MySQL) and ensure the connection to the C++ project.
3. Open the project in a C++ IDE or compile it using the command line.
4. Run the executable to interact with the system, logging in as a **User**, **Banker**, or **Admin**.

This project is a comprehensive example of integrating C++ with databases, object-oriented design, and secure role-based access in a banking environment.
