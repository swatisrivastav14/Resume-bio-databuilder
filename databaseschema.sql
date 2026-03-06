-- Create database (uncomment if needed)
-- CREATE DATABASE IF NOT EXISTS ContactDB;
-- USE ContactDB;

-- 1. MAIN CONTACTS TABLE
CREATE TABLE IF NOT EXISTS Contacts (
    Contact_ID INT PRIMARY KEY AUTO_INCREMENT,
    First_Name VARCHAR(50) NOT NULL,
    Last_Name VARCHAR(50) NOT NULL,
    Category VARCHAR(50) NOT NULL,
    Created_Date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    Updated_Date TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- 2. PHONE NUMBERS TABLE
CREATE TABLE IF NOT EXISTS Phone_Numbers (
    Phone_ID INT PRIMARY KEY AUTO_INCREMENT,
    Contact_ID INT NOT NULL,
    Phone_Type ENUM('Mobile', 'Home', 'Work', 'Other') NOT NULL,
    Phone_Number VARCHAR(20) NOT NULL,
    Is_Primary BOOLEAN DEFAULT FALSE,
    Created_Date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (Contact_ID) REFERENCES Contacts(Contact_ID) ON DELETE CASCADE,
    UNIQUE KEY unique_contact_phone (Contact_ID, Phone_Number),
    INDEX idx_contact_phone (Contact_ID, Phone_Type)
);

-- 3. EMAIL ADDRESSES TABLE
CREATE TABLE IF NOT EXISTS Email_Addresses (
    Email_ID INT PRIMARY KEY AUTO_INCREMENT,
    Contact_ID INT NOT NULL,
    Email_Type ENUM('Personal', 'Work', 'Other') NOT NULL,
    Email_Address VARCHAR(100) NOT NULL,
    Is_Primary BOOLEAN DEFAULT FALSE,
    Created_Date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (Contact_ID) REFERENCES Contacts(Contact_ID) ON DELETE CASCADE,
    UNIQUE KEY unique_contact_email (Contact_ID, Email_Address),
    INDEX idx_contact_email (Contact_ID, Email_Type)
);

-- =============================================
-- SAMPLE DATA INSERT
-- =============================================

-- Insert sample contacts
INSERT INTO Contacts (First_Name, Last_Name, Category) VALUES
('John', 'Doe', 'Work'),
('Jane', 'Smith', 'Family'),
('Bob', 'Johnson', 'Friend'),
('Alice', 'Brown', 'Work');

-- Insert phone numbers
INSERT INTO Phone_Numbers (Contact_ID, Phone_Type, Phone_Number, Is_Primary) VALUES
(1, 'Mobile', '555-0101', TRUE),
(1, 'Work', '555-0102', FALSE),
(2, 'Mobile', '555-0201', TRUE),
(2, 'Home', '555-0202', FALSE),
(3, 'Mobile', '555-0301', TRUE),
(4, 'Work', '555-0401', TRUE),
(4, 'Mobile', '555-0402', FALSE);

-- Insert email addresses
INSERT INTO Email_Addresses (Contact_ID, Email_Type, Email_Address, Is_Primary) VALUES
(1, 'Work', 'john.doe@company.com', TRUE),
(1, 'Personal', 'john.doe@gmail.com', FALSE),
(2, 'Personal', 'jane.smith@gmail.com', TRUE),
(2, 'Work', 'jane.smith@home.com', FALSE),
(3, 'Personal', 'bob.johnson@yahoo.com', TRUE),
(4, 'Work', 'alice.brown@company.com', TRUE);

-- =============================================
-- USEFUL QUERIES
-- =============================================

-- 1. View all contacts with their phones and emails
SELECT 
    c.Contact_ID,
    CONCAT(c.First_Name, ' ', c.Last_Name) AS Full_Name,
    c.Category,
    p.Phone_Type,
    p.Phone_Number,
    e.Email_Type,
    e.Email_Address
FROM Contacts c
LEFT JOIN Phone_Numbers p ON c.Contact_ID = p.Contact_ID
LEFT JOIN Email_Addresses e ON c.Contact_ID = e.Contact_ID
ORDER BY c.Last_Name, c.First_Name;

-- 2. Contacts with primary phone only
SELECT 
    CONCAT(c.First_Name, ' ', c.Last_Name) AS Full_Name,
    c.Category,
    p.Phone_Number AS Primary_Phone
FROM Contacts c
JOIN Phone_Numbers p ON c.Contact_ID = p.Contact_ID AND p.Is_Primary = TRUE;

-- 3. Find contacts by category
SELECT * FROM Contacts WHERE Category = 'Work';

-- 4. Add new phone to existing contact (Contact_ID = 1)
-- INSERT INTO Phone_Numbers (Contact_ID, Phone_Type, Phone_Number, Is_Primary) 
-- VALUES (1, 'Home', '555-0103', FALSE);

-- 5. Update contact category
-- UPDATE Contacts SET Category = 'Close Friend' WHERE Contact_ID = 3;

-- 6. Delete a contact (also deletes phones/emails automatically)
-- DELETE FROM Contacts WHERE Contact_ID = 1;

-- =============================================
-- VIEW TABLES STATUS
-- =============================================

SELECT 'Contacts' as Table_Name, COUNT(*) as Record_Count FROM Contacts
UNION ALL
SELECT 'Phone_Numbers', COUNT(*) FROM Phone_Numbers
UNION ALL
SELECT 'Email_Addresses', COUNT(*) FROM Email_Addresses;

-- Show table structure
-- DESCRIBE Contacts;
-- DESCRIBE Phone_Numbers;
-- DESCRIBE Email_Addresses;