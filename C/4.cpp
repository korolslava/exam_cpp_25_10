#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
using namespace std;

int main()
{
    ifstream in("text.txt");
    if (!in) {
        cerr << "Failed to open text.txt\n";
        return 1;
    }

    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    for (char &c : text) c = (isalnum(static_cast<unsigned char>(c)) ? tolower(static_cast<unsigned char>(c)) : ' ');

    map<string,int> freq;
    map<char, set<string>> indexByLetter;
    istringstream iss(text);
    string w;
    while (iss >> w) {
        ++freq[w];
        indexByLetter[w[0]].insert(w);
    }

    vector<pair<string,int>> vec(freq.begin(), freq.end());
    sort(vec.begin(), vec.end(), [](auto &a, auto &b){ if (a.second!=b.second) return a.second>b.second; return a.first<b.first; });

    cout << "Top5 words by frequency:\n";
    for (size_t i=0; i<vec.size() && i<5; ++i) cout << i+1 << ") " << vec[i].first << " - " << vec[i].second << '\n';

    cout << "\nIndex by first letter:\n";
    for (auto &p : indexByLetter) {
        cout << p.first << ": ";
        bool first=true;
        for (auto &s : p.second) {
            if (!first) cout << ", ";
            cout << s; first=false;
        }
        cout << '\n';
    }

    return 0;
}