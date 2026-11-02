#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

class Book {
private:
    std::string title;
    std::string author;
    int pages;
    std::string id;
    bool isBorrowed;

public:
    Book(std::string t, std::string a, int p, std::string i, bool b)
        : title(t), author(a), pages(p), id(i), isBorrowed(b) {}

    std::string getID() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    int getPages() const { return pages; }
    bool getStatus() const { return isBorrowed; }

    void setStatus(bool status) { isBorrowed = status; }

    void displayRow() const {
        std::cout << std::left << std::setw(6) << id
                  << std::setw(30) << (title.length() > 28 ? title.substr(0, 25) + "..." : title)
                  << std::setw(24) << (author.length() > 22 ? author.substr(0, 19) + "..." : author)
                  << std::setw(8) << pages
                  << (isBorrowed ? "Borrowed" : "Available") << "\n";
    }

    void displayFull() const {
        std::cout << "\n---------------- Book Details ----------------\n";
        std::cout << "ID          : " << id << "\n";
        std::cout << "Title       : " << title << "\n";
        std::cout << "Author      : " << author << "\n";
        std::cout << "Page Count  : " << pages << "\n";
        std::cout << "Status      : " << (isBorrowed ? "Borrowed" : "Available") << "\n";
        std::cout << "---------------------------------------------\n";
    }
};

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open " << filename << "\n";
        return books;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string title, author, pagesStr, id, statusStr;

        if (std::getline(ss, title, ',') &&
            std::getline(ss, author, ',') &&
            std::getline(ss, pagesStr, ',') &&
            std::getline(ss, id, ',') &&
            std::getline(ss, statusStr, ',')) {
            
            int pages = std::stoi(pagesStr);
            bool isBorrowed = (statusStr == "true" || statusStr == "1");
            books.emplace_back(title, author, pages, id, isBorrowed);
        }
    }
    return books;
}

Book* findBook(std::vector<Book>& books, const std::string& id) {
    for (auto& b : books) {
        if (b.getID() == id) return &b;
    }
    return nullptr;
}

int main() {
    std::vector<Book> library = loadBooks("bookData.txt");
    if (library.empty()) {
        std::cout << "No books loaded from bookData.txt\n";
        return 1;
    }

    int choice = 0;
    while (choice != 5) {
        std::cout << "\n===== Library Management System =====\n";
        std::cout << "1. View All Books\n";
        std::cout << "2. View Individual Book\n";
        std::cout << "3. Borrow a Book\n";
        std::cout << "4. Return a Book\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter option (1-5): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            std::cout << "\n" << std::left << std::setw(6) << "ID"
                      << std::setw(30) << "Title"
                      << std::setw(24) << "Author"
                      << std::setw(8) << "Pages"
                      << "Status\n";
            std::cout << std::string(75, '-') << "\n";
            for (const auto& b : library) b.displayRow();
        } else if (choice == 2) {
            std::string id;
            std::cout << "Enter Book ID (e.g., 001): ";
            std::cin >> id;
            Book* book = findBook(library, id);
            if (book) book->displayFull();
            else std::cout << "Book with ID " << id << " not found.\n";
        } else if (choice == 3) {
            std::string id;
            std::cout << "Enter Book ID to borrow: ";
            std::cin >> id;
            Book* book = findBook(library, id);
            if (!book) std::cout << "Book with ID " << id << " not found.\n";
            else if (book->getStatus()) std::cout << "Book is already borrowed.\n";
            else {
                book->setStatus(true);
                std::cout << "Successfully borrowed '" << book->getTitle() << "'.\n";
            }
        } else if (choice == 4) {
            std::string id;
            std::cout << "Enter Book ID to return: ";
            std::cin >> id;
            Book* book = findBook(library, id);
            if (!book) std::cout << "Book with ID " << id << " not found.\n";
            else if (!book->getStatus()) std::cout << "Book was not borrowed.\n";
            else {
                book->setStatus(false);
                std::cout << "Successfully returned '" << book->getTitle() << "'.\n";
            }
        }
    }
    std::cout << "Exiting system. Have a good day!\n";
    return 0;
}