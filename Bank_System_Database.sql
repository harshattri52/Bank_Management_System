use bank_system;
show tables;



CREATE TABLE main_account (
    -- User Identification
    user_id INT auto_increment PRIMARY KEY,
    user_name VARCHAR(100) NOT NULL,
    dob DATE NOT NULL,
    
    -- Account Information
    account_no BIGINT UNIQUE NOT NULL,
    account_type VARCHAR(20) NOT NULL,
    date_of_acc_opening DATE DEFAULT (CURRENT_DATE),
    balance DECIMAL(12, 2) DEFAULT 0.0,
    interest_rate DECIMAL(5, 2) DEFAULT 0.0,
    
    -- Transaction Details
    last_deposit INT DEFAULT NULL,
    last_withdrawal INT DEFAULT NULL,
    email varchar(50) not null,
    -- Contact Information
    address varchar(100),
    phone_number VARCHAR(15) UNIQUE,
    
    -- Government Identifiers
    aadhar_no VARCHAR(12) UNIQUE,
    pan_no VARCHAR(10) UNIQUE
);
 
 CREATE TABLE login_details_user (
    login_id VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,
    account_no BIGINT UNIQUE NOT NULL,
    CONSTRAINT fk_account_no FOREIGN KEY (account_no) REFERENCES main_account(account_no)
);

CREATE TABLE login_details_admin (
    admin_id INT PRIMARY KEY,
    admin_name VARCHAR(100) NOT NULL,
    dob DATE NOT NULL,
    address VARCHAR(255),
    phone_number VARCHAR(15),
    aadhar_no VARCHAR(12) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    login_id VARCHAR(50) NOT NULL UNIQUE,
    login_password VARCHAR(255) NOT NULL
);

CREATE TABLE login_details_banker (
    employee_id INT AUTO_INCREMENT PRIMARY KEY,
    employee_name VARCHAR(100) NOT NULL,
    dob DATE NOT NULL,
    address VARCHAR(255),
    phone_number VARCHAR(15),
    aadhar_no VARCHAR(12) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    login_id VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

select * from main_account;
select * from login_details_user;
select * from login_details_banker;
select * from login_details_admin;

CREATE TABLE user_add_requests (
    request_id INT AUTO_INCREMENT PRIMARY KEY,
    employee_id INT NOT NULL,
    user_name VARCHAR(100) NOT NULL,
    dob DATE NOT NULL,
    account_no BIGINT UNIQUE NOT NULL,
    account_type ENUM('saving', 'current') NOT NULL,
    email VARCHAR(100) NOT NULL,
    address VARCHAR(255),
    phone_number VARCHAR(15),
    aadhar_no VARCHAR(12) NOT NULL,
    pan_no VARCHAR(10) NOT NULL,
    status ENUM('Pending', 'Approved', 'Rejected') NOT NULL
);

