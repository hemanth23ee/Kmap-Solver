#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

string toBinaryString(int x, int n) {
    string s;
    for (int i = n - 1; i >= 0; i--)
        s += ((x >> i) & 1) + '0';
    return s;
}

string reduceQuineMcCluskey(const set<int>& minterms, const set<int>& dontcares, int n) {
    set<int> combined = minterms;
    combined.insert(dontcares.begin(), dontcares.end());

    map<int, vector<string>> groups;
    for (int m : combined) {
        string b = toBinaryString(m, n);
        int ones = count(b.begin(), b.end(), '1');
        groups[ones].push_back(b);
    }

    set<string> primeImplicants;
    set<string> used;
    bool done = false;
    while (!done) {
        done = true;
        map<int, vector<string>> newGroups;
        set<string> currentUsed;
        for (int i = 0; i < n; ++i) {
            if (!groups.count(i) || !groups.count(i + 1)) continue;
            for (auto& a : groups[i]) {
                for (auto& b : groups[i + 1]) {
                    int diff = 0, pos = -1;
                    for (int j = 0; j < n; ++j) {
                        if (a[j] != b[j]) {
                            diff++;
                            pos = j;
                        }
                    }
                    if (diff == 1) {
                        string merged = a;
                        merged[pos] = '-';
                        newGroups[i].push_back(merged);
                        currentUsed.insert(a);
                        currentUsed.insert(b);
                        done = false;
                    }
                }
            }
        }
        for (auto& g : groups) {
            for (auto& b : g.second) {
                if (!currentUsed.count(b))
                    primeImplicants.insert(b);
            }
        }
        groups = newGroups;
    }

    map<string, set<int>> pi_coverage;
    for (auto& pi : primeImplicants) {
        for (int m : minterms) {
            string b = toBinaryString(m, n);
            bool match = true;
            for (int i = 0; i < n; ++i) {
                if (pi[i] != '-' && pi[i] != b[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                pi_coverage[pi].insert(m);
            }
        }
    }

    set<int> covered;
    vector<string> finalImplicants;
    while (covered.size() < minterms.size()) {
        string bestPI;
        int maxCover = 0;
        for (auto& [pi, coveredMins] : pi_coverage) {
            int count = 0;
            for (int m : coveredMins) if (!covered.count(m)) count++;
            if (count > maxCover) {
                maxCover = count;
                bestPI = pi;
            }
        }
        if (bestPI.empty()) break;
        finalImplicants.push_back(bestPI);
        for (int m : pi_coverage[bestPI]) covered.insert(m);
        pi_coverage.erase(bestPI);
    }

    string result;
    for (auto& term : finalImplicants) {
        if (!result.empty()) result += " + ";
        for (int i = 0; i < n; ++i) {
            if (term[i] == '-') continue;
            result += (char)('A' + i);
            if (term[i] == '0') result += "'";
        }
    }
    return result.empty() ? "0" : result;
}

int main() {
    int n, m, d;
    cout << "Enter number of variables (2 to 10): ";
    cin >> n;
    if (n < 2 || n > 10) {
        cout << "Invalid number of variables.\n";
        return 0;
    }

    set<int> minterms, dontcares;
    cout << "Enter number of minterms: "; cin >> m;
    cout << "Enter minterms: ";
    for (int i = 0; i < m; i++) {
        int x; cin >> x;
        if (x >= 0 && x < (1 << n)) minterms.insert(x);
    }

    cout << "Enter number of don't-care terms: "; cin >> d;
    cout << "Enter don't-care terms: ";
    for (int i = 0; i < d; i++) {
        int x; cin >> x;
        if (x >= 0 && x < (1 << n) && !minterms.count(x))
            dontcares.insert(x);
    }

    string simplified = reduceQuineMcCluskey(minterms, dontcares, n);
    cout << "\nSimplified Expression (Sum of Products): " << simplified << "\n";
    return 0;
}

