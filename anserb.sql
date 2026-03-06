-- Create the database schema
CREATE TABLE Categories (
    Category_ID INTEGER PRIMARY KEY,
    Category_Name TEXT NOT NULL
);

CREATE TABLE Contacts (
    Contact_ID INTEGER PRIMARY KEY,
    First_Name TEXT NOT NULL,
    Last_Name TEXT NOT NULL,
    Category_ID INTEGER,
    FOREIGN KEY (Category_ID) REFERENCES Categories(Category_ID)
);

CREATE TABLE Phones (
    Phone_ID INTEGER PRIMARY KEY,
    Contact_ID INTEGER,
    Phone_Number TEXT NOT NULL,
    Phone_Type TEXT,
    FOREIGN KEY (Contact_ID) REFERENCES Contacts(Contact_ID)
);

CREATE TABLE Emails (
    Email_ID INTEGER PRIMARY KEY,
    Contact_ID INTEGER,
    Email_Address TEXT NOT NULL,
    FOREIGN KEY (Contact_ID) REFERENCES Contacts(Contact_ID)
);

-- Insert sample data
INSERT INTO Categories (Category_ID, Category_Name) VALUES
(1, 'Work'),
(2, 'Personal'),
(3, 'Family');

INSERT INTO Contacts (Contact_ID, First_Name, Last_Name, Category_ID) VALUES
(1, 'John', 'Doe', 1),
(2, 'Jane', 'Smith', 1),
(3, 'Bob', 'Johnson', 2),
(4, 'Alice', 'Brown', 2),
(5, 'Charlie', 'Wilson', 3),
(6, 'Diana', 'Davis', 1);

INSERT INTO Phones (Phone_ID, Contact_ID, Phone_Number, Phone_Type) VALUES
(1, 1, '555-0101', 'Mobile'),
(2, 1, '555-0102', 'Work'),
(3, 2, '555-0201', 'Mobile'),
(4, 3, '555-0301', 'Mobile'),
(5, 4, '555-0401', 'Home'),
(6, 5, '555-0501', 'Mobile'),
(7, 6, '555-0601', 'Work');

INSERT INTO Emails (Email_ID, Contact_ID, Email_Address) VALUES
(1, 1, 'john.doe@work.com'),
(2, 1, 'john.personal@gmail.com'),
(3, 2, 'jane.smith@work.com'),
(4, 3, 'bob@gmail.com'),
(5, 4, 'alice@yahoo.com'),
(6, 5, 'charlie@family.com'),
(7, 6, 'diana@work.com');

-- ========== RUN THE QUERIES ==========

-- 1. Full name, phones, emails for Contact_ID = 1
SELECT 
    c.First_Name || ' ' || c.Last_Name AS Full_Name,
    p.Phone_Number,
    e.Email_Address
FROM Contacts c
LEFT JOIN Phones p ON c.Contact_ID = p.Contact_ID
LEFT JOIN Emails e ON c.Contact_ID = e.Contact_ID
WHERE c.Contact_ID = 1
ORDER BY p.Phone_Number, e.Email_Address;

-- 2. Contacts in 'Work' category
SELECT DISTINCT 
    c.First_Name, 
    c.Last_Name
FROM Contacts c
JOIN Categories cat ON c.Category_ID = cat.Category_ID
WHERE cat.Category_Name = 'Work'
ORDER BY c.Last_Name, c.First_Name;

-- 3. Contacts with 'Mobile' phone
SELECT DISTINCT 
    c.Contact_ID, 
    c.First_Name
FROM Contacts c
JOIN Phones p ON c.Contact_ID = p.Contact_ID
WHERE p.Phone_Type = 'Mobile'
ORDER BY c.Contact_ID;

-- 4. Count contacts per category
SELECT 
    cat.Category_Name,
    COUNT(c.Contact_ID) AS Contact_Count
FROM Categories cat
LEFT JOIN Contacts c ON cat.Category_ID = c.Category_ID
GROUP BY cat.Category_ID, cat.Category_Name
ORDER BY Contact_Count DESC;

-- 5. Contacts with >1 phone number
SELECT 
    c.First_Name,
    c.Last_Name
FROM Contacts c
JOIN Phones p ON c.Contact_ID = p.Contact_ID
GROUP BY c.Contact_ID, c.First_Name, c.Last_Name
HAVING COUNT(p.Phone_ID) > 1
ORDER BY c.Last_Name, c.First_Name;