// Task 5: Contact Management System (struct + vector + file persistence)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
using namespace std;

struct Contact {
    string name, phone, email, address;
};

const string FILE_NAME = "contacts.txt";

vector<string> split(const string& s, char d) {
    vector<string> out; string cur;
    for (char c : s) { if (c == d) { out.push_back(cur); cur.clear(); } else cur += c; }
    out.push_back(cur);
    return out;
}
string clean(string s) { replace(s.begin(), s.end(), '|', '/'); return s; }
string lower(string s) { transform(s.begin(), s.end(), s.begin(), ::tolower); return s; }

string readLine(const string& prompt) {
    string s; cout << prompt;
    if (!getline(cin, s)) exit(0);
    return s;
}
int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        string s = readLine(prompt);
        try { size_t p; int v = stoi(s, &p); if (p == s.size() && v >= lo && v <= hi) return v; } catch (...) {}
        cout << "  [Error] Enter a number between " << lo << " and " << hi << ".\n";
    }
}

void loadContacts(vector<Contact>& list) {
    ifstream in(FILE_NAME);
    string line;
    while (in && getline(in, line)) {
        if (line.empty()) continue;
        auto f = split(line, '|');
        if (f.size() >= 4) list.push_back({f[0], f[1], f[2], f[3]});
    }
}
void saveContacts(const vector<Contact>& list) {
    ofstream out(FILE_NAME);
    for (auto& c : list) out << c.name << '|' << c.phone << '|' << c.email << '|' << c.address << '\n';
}

void printContact(int idx, const Contact& c) {
    cout << "  [" << idx << "] " << c.name << " | " << c.phone << " | " << c.email << " | " << c.address << "\n";
}

void listAll(const vector<Contact>& list) {
    if (list.empty()) { cout << "  No contacts saved.\n"; return; }
    vector<int> order(list.size());
    for (size_t i = 0; i < order.size(); i++) order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b) { return lower(list[a].name) < lower(list[b].name); });
    for (int i : order) printContact(i + 1, list[i]);
}

vector<int> searchContacts(const vector<Contact>& list, const string& q) {
    vector<int> hits; string lq = lower(q);
    for (size_t i = 0; i < list.size(); i++)
        if (lower(list[i].name).find(lq) != string::npos || list[i].phone.find(q) != string::npos)
            hits.push_back(i);
    return hits;
}

void addContact(vector<Contact>& list) {
    Contact c;
    c.name = clean(readLine("Name   : "));
    if (c.name.empty()) { cout << "  [Error] Name is required.\n"; return; }
    c.phone = clean(readLine("Phone  : "));
    c.email = clean(readLine("Email  : "));
    c.address = clean(readLine("Address: "));
    list.push_back(c);
    saveContacts(list);
    cout << "  Contact added.\n";
}

void searchAndShow(const vector<Contact>& list) {
    auto hits = searchContacts(list, readLine("Search by name or phone: "));
    if (hits.empty()) cout << "  No match found.\n";
    for (int i : hits) printContact(i + 1, list[i]);
}

int pickContact(const vector<Contact>& list, const string& action) {
    auto hits = searchContacts(list, readLine("Search contact to " + action + " (name/phone): "));
    if (hits.empty()) { cout << "  No match found.\n"; return -1; }
    for (int i : hits) printContact(i + 1, list[i]);
    int n = readInt("Enter contact number to " + action + " (0 to cancel): ", 0, list.size());
    if (n == 0 || find(hits.begin(), hits.end(), n - 1) == hits.end()) {
        if (n != 0) cout << "  That number was not in the search results.\n";
        return -1;
    }
    return n - 1;
}

void editContact(vector<Contact>& list) {
    int i = pickContact(list, "edit");
    if (i < 0) return;
    cout << "Leave a field blank to keep the current value.\n";
    string v;
    v = clean(readLine("Name [" + list[i].name + "]: "));       if (!v.empty()) list[i].name = v;
    v = clean(readLine("Phone [" + list[i].phone + "]: "));     if (!v.empty()) list[i].phone = v;
    v = clean(readLine("Email [" + list[i].email + "]: "));     if (!v.empty()) list[i].email = v;
    v = clean(readLine("Address [" + list[i].address + "]: ")); if (!v.empty()) list[i].address = v;
    saveContacts(list);
    cout << "  Contact updated.\n";
}

void deleteContact(vector<Contact>& list) {
    int i = pickContact(list, "delete");
    if (i < 0) return;
    string ok = lower(readLine("Delete '" + list[i].name + "'? (y/n): "));
    if (ok == "y") { list.erase(list.begin() + i); saveContacts(list); cout << "  Contact deleted.\n"; }
    else cout << "  Cancelled.\n";
}

int main() {
    vector<Contact> contacts;
    loadContacts(contacts);
    cout << "Loaded " << contacts.size() << " contact(s) from " << FILE_NAME << "\n";
    while (true) {
        cout << "\n===== CONTACT MANAGEMENT SYSTEM =====\n"
             << "1. Add contact\n2. View all contacts\n3. Search contact\n4. Edit contact\n"
             << "5. Delete contact\n6. Save & Exit\n";
        switch (readInt("Choice: ", 1, 6)) {
            case 1: addContact(contacts); break;
            case 2: listAll(contacts); break;
            case 3: searchAndShow(contacts); break;
            case 4: editContact(contacts); break;
            case 5: deleteContact(contacts); break;
            case 6: saveContacts(contacts); cout << "Contacts saved. Goodbye!\n"; return 0;
        }
    }
}
