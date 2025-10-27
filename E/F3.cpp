#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
using json = nlohmann::json;

int main() {
    json j;
    ifstream in("contacts.json");
    if (in) {
        try {
            in >> j;
        } catch (const exception &e) {
            cerr << "Error parsing contacts.json: " << e.what() << '\n';
            return 1;
        }
    } else {
        j = json::object();
    }

    if (!j.is_object()) {
        cerr << "Error: contacts.json must be a JSON object mapping names to phones\n";
        return 1;
    }

    map<string, string> contacts;
    for (auto it = j.begin(); it != j.end(); ++it) {
        try {
            contacts[it.key()] = it.value().get<string>();
        } catch (const exception &) {
            continue;
        }
    }

    cout << "Loaded " << contacts.size() << " contacts." << '\n';

    string name;
    while (true) {
        cout << "\nEnter name to search (empty = exit): ";
        if (!std::getline(cin, name)) break;
        if (name.empty()) break;

        auto it = contacts.find(name);
        if (it != contacts.end()) {
            cout << "Found: " << it->first << " -> " << it->second << '\n';
        } else {
            cout << "Not found: " << name << '\n';
        }

        cout << "Enter new phone to add/update, 'DELETE' to remove, or leave empty to skip: ";
        string phone;
        if (!std::getline(cin, phone)) break;
        if (!phone.empty()) {
            if (phone == "DELETE") {
                if (it != contacts.end()) {
                    contacts.erase(it);
                    cout << "Removed contact: " << name << '\n';
                } else {
                    cout << "Contact not present to delete." << '\n';
                }
            } else {
                contacts[name] = phone;
                cout << "Saved: " << name << " -> " << phone << '\n';
            }
        }
    }

    cout << "\nSave changes to contacts.json? (y/n): ";
    string ans;
    if (!getline(cin, ans)) ans = "n";
    if (!ans.empty() && (ans[0] == 'y' || ans[0] == 'Y')) {
        json outj = json::object();
        for (const auto &p : contacts) outj[p.first] = p.second;
        ofstream out("contacts.json");
        if (!out) {
            cerr << "Error: cannot write contacts.json\n";
            return 1;
        }
        out << outj.dump(4) << '\n';
        cout << "Saved " << contacts.size() << " contacts to contacts.json" << '\n';
    } else {
        cout << "Changes not saved." << '\n';
    }

    return 0;
}