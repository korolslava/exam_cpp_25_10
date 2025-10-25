#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

template<class T> class Box {
	vector<T> elements;
public:
	void addElement(const T& element) {
		elements.push_back(element);
	}
	template<typename Pred>
	void removeIf(Pred pred) {
		elements.erase(std::remove_if(elements.begin(), elements.end(), pred), elements.end());
	}
	template<typename F>
	void transform(F f) {
		for (auto& element : elements) {
			element = f(element);
		}
	}
	template<typename G>
	void forEach(G g) const {
		for (const auto& element : elements) {
			g(element);
		}
	}
	void saveTXT(const string& filename) const {
		ofstream file(filename);
		for (const auto& element : elements) {
			file << element << endl;
		}
	}
	void loadTXT(const string& filename) {
		ifstream file(filename);
		T element;
		elements.clear();
		while (file >> element) {
			elements.push_back(element);
		}
	}
	void saveJSON(const string& filename) const {
		json j = elements;
		ofstream file(filename);
		file << j.dump(4);
	}
	void loadJSON(const string& filename) {
		ifstream file(filename);
		json j;
		file >> j;
		elements = j.get<vector<T>>();
	}
};


int main() {
	Box<int> intBox;
	intBox.addElement(1);
	intBox.addElement(2);
	intBox.addElement(3);

	intBox.transform([](int x) { return x *10; });

	intBox.saveJSON("intBox.json");
	Box<int> intBox2;
	intBox2.loadJSON("intBox.json");

	cout << "intBox2 contents after loadJSON: ";
	intBox2.forEach([](int x){ cout << x << " "; });
	cout << '\n';

	intBox2.removeIf([](int x){ return x >10; });
	cout << "intBox2 after removeIf(>10): ";
	intBox2.forEach([](int x){ cout << x << " "; });
	cout << '\n';

	Box<string> strBox;
	strBox.addElement("Hello");
	strBox.addElement("World");

	strBox.saveTXT("strBox.txt");
	Box<string> strBox2;
	strBox2.loadTXT("strBox.txt");

	cout << "strBox2 contents after loadTXT: ";
	strBox2.forEach([](const string &s){ cout << s << " "; });
	cout << '\n';

	strBox2.transform([](string s){
		for (char &c : s) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
		return s;
	});

	cout << "strBox2 after transform to UPPER: ";
	strBox2.forEach([](const string &s){ cout << s << " "; });
	cout << '\n';

	return 0;
}