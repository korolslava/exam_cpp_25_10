#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
using namespace std;
using json = nlohmann::json;

int main() {
    ifstream in("products.json");
    if (!in) {
        cerr << "Error: cannot open products.json\n";
        return 1;
    }

    json products;
    try {
        in >> products;
    } catch (const exception &e) {
        cerr << "Error parsing products.json: " << e.what() << '\n';
        return 1;
    }

    if (!products.is_array()) {
        cerr << "Error: products.json must contain a JSON array of product objects\n";
        return 1;
    }

    json expensive = json::array();

    for (const auto &p : products) {
        if (!p.is_object()) continue;
        if (!p.contains("price")) continue;
        double price =0.0;
        try {
            price = p.at("price").get<double>();
        } catch (const std::exception &) {
            continue;
        }
        if (price >100.0) {
            expensive.push_back(p);
            cout << p.dump(4) << '\n';
        }
    }

    ofstream out("expensive.json");
    if (!out) {
        cerr << "Error: cannot write expensive.json\n";
        return 1;
    }
    out << expensive.dump(4) << '\n';

    return 0;
}