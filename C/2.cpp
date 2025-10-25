#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

class Book {
public:
	string title;
	string author;
	int year;
	Book(const string& t, const string& a, int y) : title(t), author(a), year(y) {}
};

class Library {
public:
	vector<Book> books;
	void addBook(const Book& book) {
		books.push_back(book);
	}
	void removeByTitle(const string& title) {
		auto count = std::count_if(books.begin(), books.end(),
			[&title](const Book& b) { return b.title == title; });
		if (count ==0) {
			throw runtime_error("Book with title '" + title + "' not found");
		}
		books.erase(std::remove_if(books.begin(), books.end(),
			[&title](const Book& b) { return b.title == title; }), books.end());
	}
	void findByAuthor(const string& author) {
		for (const auto& book : books) {
			if (book.author == author) {
				cout << "Found: " << book.title << " by " << book.author << " (" << book.year << ")\n";
			}
		}
	}
	
	void saveTxt(const string& filename) {
		ofstream file(filename);
		for (const auto& book : books) {
			file << book.title << "," << book.author << "," << book.year << "\n";
		}
	}
	void loadTxt(const string& filename) {
		ifstream file(filename);
		string line;
		books.clear();
		while (getline(file, line)) {
			stringstream ss(line);
			string title, author, yearStr;
			getline(ss, title, ',');
			getline(ss, author, ',');
			getline(ss, yearStr);
			books.emplace_back(title, author, stoi(yearStr));
		}
	}
	void saveJson(const string& filename) {
		json j;
		for (const auto& book : books) {
			j.push_back({ {"title", book.title}, {"author", book.author}, {"year", book.year} });
		}
		ofstream file(filename);
		file << j.dump(4);
	}
	void loadJson(const string& filename) {
		ifstream file(filename);
		json j;
		file >> j;
		books.clear();
		for (const auto& item : j) {
			books.emplace_back(item["title"], item["author"], item["year"]);
		}
	}
};

int main()
{
	Library lib;
	lib.addBook(Book("1984", "George Orwell",1949));
	lib.addBook(Book("To Kill a Mockingbird", "Harper Lee",1960));
	lib.saveTxt("library.txt");
	lib.saveJson("library.json");
	Library lib2;
	lib2.loadTxt("library.txt");
	lib2.findByAuthor("Harper Lee");
	Library lib3;
	lib3.loadJson("library.json");
	lib3.findByAuthor("George Orwell");

	try {
		lib.removeByTitle("A Nonexistent Book");
		cout << "Removed successfully\n";
	} catch (const runtime_error& e) {
		cerr << "Error removing book: " << e.what() << "\n";
	}

	try {
		lib.removeByTitle("1984");
		cout << "'1984' removed from library\n";
	} catch (const runtime_error& e) {
		cerr << "Error removing book: " << e.what() << "\n";
	}

	return 0;
}