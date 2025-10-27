#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
using namespace std;
using json = nlohmann::json;

int main() {
    ifstream in("students.json");
    if (!in) {
        cerr << "Error: cannot open students.json\n";
        return 1;
    }

    json students;
    try {
        in >> students;
    } catch (const exception &e) {
        cerr << "Error parsing students.json: " << e.what() << '\n';
        return 1;
    }

    if (!students.is_array()) {
        cerr << "Error: students.json must contain a JSON array of student objects\n";
        return 1;
    }

    double sum = 0.0;
    size_t count = 0;
    json deans = json::array();

    for (const auto &s : students) {
        if (!s.is_object()) continue;
        if (!s.contains("gpa")) continue;
        double gpa = 0.0;
        try {
            gpa = s.at("gpa").get<double>();
        } catch (const std::exception &) {
            continue;
        }
        sum += gpa;
        ++count;
        if (gpa >= 3.5) {
            deans.push_back(s);
        }
    }

    double avg = (count > 0) ? (sum / static_cast<double>(count)) : 0.0;
    cout << "Average GPA: " << avg << '\n';

    ofstream out("deans_list.json");
    if (!out) {
        cerr << "Error: cannot write deans_list.json\n";
        return 1;
    }
    out << deans.dump(4) << '\n';

    return 0;
}