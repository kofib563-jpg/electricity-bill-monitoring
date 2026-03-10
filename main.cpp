#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";
const int MAX_APPLIANCES = 100;

struct Appliance {
    string name;
    double watts;
    double hours;
};

void clearLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string trim(const string& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i <= j && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i++;
    while (j >= i && (s[j] == ' ' || s[j] == '\t' || s[j] == '\r' || s[j] == '\n')) j--;
    if (i > j) return "";
    return s.substr(i, j - i + 1);
}

string lowerText(string s) {
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = char(s[i] - 'A' + 'a');
    }
    return s;
}

int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) { clearLine(); return x; }
        cout << "Invalid number. Try again.\n";
        clearLine();
    }
}

double readDouble(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) { clearLine(); return x; }
        cout << "Invalid number. Try again.\n";
        clearLine();
    }
}

string readNonEmptyLine(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (!s.empty()) return s;
        cout << "Input must not be empty.\n";
    }
}

void menu() {
    cout << "\n1. Register appliance\n";
    cout << "2. View appliances\n";
    cout << "3. Search appliance\n";
    cout << "4. Billing\n";
    cout << "5. Save to file\n";
    cout << "6. Exit\n";
}

double kwhPerDay(const Appliance& a) {
    return (a.watts / 1000.0) * a.hours;
}

void saveAppliances(const Appliance a[], int n) {
    ofstream out(APPLIANCES_FILE.c_str());
    if (!out) {
        cout << "Error writing " << APPLIANCES_FILE << "\n";
        return;
    }
    for (int i = 0; i < n; i++) {
        out << a[i].name << "|" << a[i].watts << "|" << a[i].hours << "\n";
    }
}

void loadAppliances(Appliance a[], int& n) {
    n = 0;
    ifstream in(APPLIANCES_FILE.c_str());
    if (!in) return;

    string line;
    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        int p1 = (int)line.find('|');
        int p2 = (p1 == -1) ? -1 : (int)line.find('|', p1 + 1);
        if (p1 == -1 || p2 == -1) continue;

        string name = trim(line.substr(0, p1));
        string wStr = trim(line.substr(p1 + 1, p2 - p1 - 1));
        string hStr = trim(line.substr(p2 + 1));
        if (name.empty()) continue;

        double w = 0, h = 0;
        try { w = stod(wStr); h = stod(hStr); }
        catch (...) { continue; }

        if (w <= 0 || h < 0 || h > 24) continue;
        if (n >= MAX_APPLIANCES) break;

        a[n].name = name;
        a[n].watts = w;
        a[n].hours = h;
        n++;
    }
}

void addAppliance(Appliance a[], int& n) {
    if (n >= MAX_APPLIANCES) {
        cout << "Limit reached.\n";
        return;
    }

    Appliance x;
    x.name = readNonEmptyLine("Name: ");
    do { x.watts = readDouble("Watts (>0): "); } while (x.watts <= 0);
    do { x.hours = readDouble("Hours/day (0-24): "); } while (x.hours < 0 || x.hours > 24);

    a[n] = x;
    n++;

    saveAppliances(a, n);
    cout << "Saved.\n";
}

void listAppliances(const Appliance a[], int n) {
    if (n == 0) {
        cout << "No appliances.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << left << setw(4) << "#"
         << setw(25) << "Name"
         << setw(10) << "Watts"
         << setw(10) << "Hours"
         << setw(10) << "kWh/day"
         << "\n";

    for (int i = 0; i < n; i++) {
        cout << left << setw(4) << (i + 1)
             << setw(25) << a[i].name
             << setw(10) << a[i].watts
             << setw(10) << a[i].hours
             << setw(10) << kwhPerDay(a[i])
             << "\n";
    }
}

void searchAppliances(const Appliance a[], int n) {
    if (n == 0) {
        cout << "No appliances.\n";
        return;
    }

    string q = lowerText(readNonEmptyLine("Search name: "));
    bool found = false;

    cout << fixed << setprecision(2);
    for (int i = 0; i < n; i++) {
        string nm = lowerText(a[i].name);
        if (nm.find(q) != string::npos) {
            cout << "- " << a[i].name
                 << " | " << a[i].watts << " W"
                 << " | " << a[i].hours << " hrs"
                 << " | " << kwhPerDay(a[i]) << " kWh/day\n";
            found = true;
        }
    }
    if (!found) cout << "No match.\n";
}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;

    loadAppliances(appliances, count);

    cout << "Electrical Load Monitoring & Billing System\n";
    cout << "Loaded appliances: " << count << "\n";

    while (true) {
        menu();
        int choice = readInt("Choose (1-6): ");

        if (choice == 1) addAppliance(appliances, count);
        else if (choice == 2) listAppliances(appliances, count);
        else if (choice == 3) searchAppliances(appliances, count);
        else if (choice == 5) {
            saveAppliances(appliances, count);
            cout << "Saved to " << APPLIANCES_FILE << ".\n";
        } else if (choice == 6) {
            saveAppliances(appliances, count);
            cout << "Goodbye!\n";
            break;
        } else if (choice == 4) {
            cout << "Feature not added yet (will appear in next part).\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}