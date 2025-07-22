#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

string hashPassword(const string &password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return to_string(hash);
}

class Book {
private:
    string name;
    string author;
    bool isBorrowed;

public:
    Book(string name, string author) {
        this->name = name;
        this->author = author;
        isBorrowed = false;
    }

    string getBookName() const { return name; }
    string getAuthorName() const { return author; }
    bool BorrowedStatus() const { return isBorrowed; }

    void setBookStatus(bool status) { isBorrowed = status; }
};

class Library {
private:
    vector<Book> list;

public:
    void addBook(const string &name, const string &author) {
        Book b(name, author);
        list.push_back(b);
        cout << "Book added successfully.\n";
    }

    void returnBook(int ind) {
        if (ind <= 0 || ind > list.size()) {
            cout << "Invalid book index.\n";
            return;
        }
        list[ind - 1].setBookStatus(false);
        cout << "Book returned successfully.\n";
    }

    void displayBooks() const {
        if (list.empty()) {
            cout << "No books in the library.\n";
            return;
        }
        for (int i = 0; i < list.size(); i++) {
            cout << (i + 1) << ". " << list[i].getBookName()
                 << " by " << list[i].getAuthorName();
            if (list[i].BorrowedStatus()) cout << " (Borrowed)";
            cout << endl;
        }
    }

    void displayBorrowedBooks() const {
        bool found = false;
        for (int i = 0; i < list.size(); i++) {
            if (list[i].BorrowedStatus()) {
                found = true;
                cout << (i + 1) << ". " << list[i].getBookName()
                     << " by " << list[i].getAuthorName() << endl;
            }
        }
        if (!found) cout << "No books are currently borrowed.\n";
    }

    void borrowBook(int ind) {
        if (ind <= 0 || ind > list.size()) {
            cout << "Invalid book index.\n";
            return;
        }
        if (list[ind - 1].BorrowedStatus()) {
            cout << "Book is already borrowed.\n";
            return;
        }
        list[ind - 1].setBookStatus(true);
        cout << "Book borrowed successfully.\n";
    }
};

// Check if user exists and password is correct
bool authenticate(const string &username, const string &password) {
    ifstream file("users.txt");
    if (!file) {
        cerr << "Error: User file not found.\n";
        return false;
    }

    string line;
    string hashed = hashPassword(password);
    while (getline(file, line)) {
        stringstream ss(line);
        string user, passHash;
        ss >> user >> passHash;
        if (user == username && passHash == hashed) return true;
    }
    return false;
}

// Create new admin or librarian user
void registerUser() {
    string username, password;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    cin >> password;

    ofstream file("users.txt", ios::app);
    file << username << " " << hashPassword(password) << "\n";
    cout << "User registered successfully.\n";
}

int main() {
    Library lb;
    int ch;
    string username, password;
    bool isLoggedIn = false;

    while (true) {
        cout << "\n*** Library Menu ***\n";
        cout << "1. Login as Admin/Librarian\n";    // after runing the program you can first you have to login 
        cout << "2. Register Admin/Librarian\n";    // after login you have to set your username and password for admin or librarian
        cout << "3. Add Book (Login Required)\n";   // add a new book 
        cout << "4. Return Book\n";                 // return a book by choosing a option
        cout << "5. Borrow Book\n";                 // select the option no and borrow a book of your choice
        cout << "6. Display All Books\n";           // now you display all the book
        cout << "7. Display Borrowed Books\n";      // display the borrowed book
        cout << "8. Exit\n";                        // after choosing now exit for the library
        cout << "Enter your choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch) {
        case 1: {
            cout << "Username: ";     
            cin >> username;         //you can use set username by taking option 2 
            cout << "Password: ";    
            cin >> password;       // now choose the password , and remember your password for the next time login
            if (authenticate(username, password)) {
                cout << "Login successful.\n";
                isLoggedIn = true;
            } else {
                cout << "Invalid username or password.\n";
            }
            break;
        }
        case 2:
            registerUser();
            break;
        case 3:
            if (!isLoggedIn) {
                cout << "Access Denied. Please login first.\n";
                break;
            }
            {
                string name, author;
                cout << "Enter book name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter author name: ";
                getline(cin, author);
                lb.addBook(name, author);
            }
            break;
        case 4: {
            lb.displayBorrowedBooks();
            cout << "Enter book number to return: ";
            int ind;
            cin >> ind;
            lb.returnBook(ind);
            break;
        }
        case 5: {
            lb.displayBooks();
            cout << "Enter book number to borrow: ";
            int ind;
            cin >> ind;
            lb.borrowBook(ind);
            break;
        }
        case 6:
            lb.displayBooks();
            break;
        case 7:
            lb.displayBorrowedBooks();
            break;
        case 8:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
