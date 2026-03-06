#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
using namespace std;

class Contact {
public:
    int id;
    string firstName;
    string lastName;
    string email;
    string phone;
    string address;
    string city;
    string state;
    string zipCode;

    Contact() : id(0) {}
    
    Contact(int id, string fn, string ln, string em, string ph, 
            string addr, string cty, string st, string zip)
        : id(id), firstName(fn), lastName(ln), email(em), phone(ph),
          address(addr), city(cty), state(st), zipCode(zip) {}

    string getFullName() const {
        return firstName + " " + lastName;
    }

    string toString() const {
        stringstream ss;
        ss << id << "," << firstName << "," << lastName << "," 
           << email << "," << phone << "," << address << "," 
           << city << "," << state << "," << zipCode;
        return ss.str();
    }

    static Contact fromString(const string& line) {
        stringstream ss(line);
        Contact contact;
        string token;
        
        getline(ss, token, ','); contact.id = stoi(token);
        getline(ss, token, ','); contact.firstName = token;
        getline(ss, token, ','); contact.lastName = token;
        getline(ss, token, ','); contact.email = token;
        getline(ss, token, ','); contact.phone = token;
        getline(ss, token, ','); contact.address = token;
        getline(ss, token, ','); contact.city = token;
        getline(ss, token, ','); contact.state = token;
        getline(ss, token, ','); contact.zipCode = token;
        
        return contact;
    }
};

class ContactManager {
private:
    vector<Contact> contacts;
    int nextId;
    const string FILENAME = "contacts.txt";

public:
    ContactManager() : nextId(1) {
        loadContacts();
    }

    void loadContacts() {
        ifstream file(FILENAME);
        if (!file.is_open()) {
            cout << "No existing contacts file found. Starting fresh.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Contact contact = Contact::fromString(line);
                contacts.push_back(contact);
                nextId = max(nextId, contact.id + 1);
            }
        }
        file.close();
        cout << "Loaded " << contacts.size() << " contacts from file.\n";
    }

    void saveContacts() {
        ofstream file(FILENAME);
        if (!file.is_open()) {
            cout << "Error: Cannot save contacts!\n";
            return;
        }

        for (const auto& contact : contacts) {
            file << contact.toString() << endl;
        }
        file.close();
        cout << "Contacts saved successfully!\n";
    }

    void displayAllContacts() {
        if (contacts.empty()) {
            cout << "No contacts found.\n";
            return;
        }

        cout << "\n" << left << setw(4) << "ID" 
             << setw(15) << "Name" 
             << setw(25) << "Email" 
             << setw(12) << "Phone" 
             << setw(15) << "City" << endl;
        cout << string(75, '-') << endl;

        for (const auto& contact : contacts) {
            cout << left << setw(4) << contact.id
                 << setw(15) << contact.getFullName()
                 << setw(25) << contact.email
                 << setw(12) << contact.phone
                 << setw(15) << contact.city << endl;
        }
    }

    void addContact() {
        Contact newContact;
        newContact.id = nextId++;

        cout << "\n--- Add New Contact ---\n";
        cout << "First Name: ";
        cin.ignore();
        getline(cin, newContact.firstName);
        
        cout << "Last Name: ";
        getline(cin, newContact.lastName);
        
        cout << "Email: ";
        getline(cin, newContact.email);
        
        cout << "Phone: ";
        getline(cin, newContact.phone);
        
        cout << "Address: ";
        getline(cin, newContact.address);
        
        cout << "City: ";
        getline(cin, newContact.city);
        
        cout << "State: ";
        getline(cin, newContact.state);
        
        cout << "ZIP Code: ";
        getline(cin, newContact.zipCode);

        contacts.push_back(newContact);
        cout << "Contact added successfully!\n";
    }

    void searchContacts() {
        string searchTerm;
        cout << "\nEnter search term (name, email, or phone): ";
        cin.ignore();
        getline(cin, searchTerm);

        vector<Contact> results;
        for (const auto& contact : contacts) {
            if (contact.getFullName().find(searchTerm) != string::npos ||
                contact.email.find(searchTerm) != string::npos ||
                contact.phone.find(searchTerm) != string::npos) {
                results.push_back(contact);
            }
        }

        if (results.empty()) {
            cout << "No contacts found matching '" << searchTerm << "'\n";
            return;
        }

        cout << "\nSearch Results (" << results.size() << " found):\n";
        cout << left << setw(4) << "ID" 
             << setw(15) << "Name" 
             << setw(25) << "Email" 
             << setw(12) << "Phone" << endl;
        cout << string(60, '-') << endl;

        for (const auto& contact : results) {
            cout << left << setw(4) << contact.id
                 << setw(15) << contact.getFullName()
                 << setw(25) << contact.email
                 << setw(12) << contact.phone << endl;
        }
    }

    void editContact() {
        int id;
        cout << "\nEnter Contact ID to edit: ";
        cin >> id;
        cin.ignore();

        auto it = findContactById(id);
        if (it == contacts.end()) {
            cout << "Contact not found!\n";
            return;
        }

        Contact& contact = *it;
        cout << "\nCurrent info:\n";
        cout << "Name: " << contact.getFullName() << "\n";
        cout << "Email: " << contact.email << "\n";
        cout << "Phone: " << contact.phone << "\n";

        cout << "\nEnter new values (press Enter to keep current):\n";
        string input;

        cout << "First Name (" << contact.firstName << "): ";
        getline(cin, input);
        if (!input.empty()) contact.firstName = input;

        cout << "Last Name (" << contact.lastName << "): ";
        getline(cin, input);
        if (!input.empty()) contact.lastName = input;

        cout << "Email (" << contact.email << "): ";
        getline(cin, input);
        if (!input.empty()) contact.email = input;

        cout << "Phone (" << contact.phone << "): ";
        getline(cin, input);
        if (!input.empty()) contact.phone = input;

        cout << "Contact updated successfully!\n";
    }

    void deleteContact() {
        int id;
        cout << "\nEnter Contact ID to delete: ";
        cin >> id;

        auto it = findContactById(id);
        if (it == contacts.end()) {
            cout << "Contact not found!\n";
            return;
        }

        cout << "Are you sure you want to delete '" << it->getFullName() 
             << "'? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            contacts.erase(it);
            cout << "Contact deleted successfully!\n";
        } else {
            cout << "Deletion cancelled.\n";
        }
    }

private:
    vector<Contact>::iterator findContactById(int id) {
        return find_if(contacts.begin(), contacts.end(),
                      [id](const Contact& c) { return c.id == id; });
    }
};

void displayMenu() {
    cout << "\n=== Contact Management System ===\n";
    cout << "1. View All Contacts\n";
    cout << "2. Add New Contact\n";
    cout << "3. Search Contacts\n";
    cout << "4. Edit Contact\n";
    cout << "5. Delete Contact\n";
    cout << "6. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    ContactManager manager;
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        cin.ignore(); // Clear newline character

        switch (choice) {
            case 1:
                manager.displayAllContacts();
                break;
            case 2:
                manager.addContact();
                break;
            case 3:
                manager.searchContacts();
                break;
            case 4:
                manager.editContact();
                break;
            case 5:
                manager.deleteContact();
                break;
            case 6:
                manager.saveContacts();
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option! Please try again.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}