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

double totalKwhPerDay(const Appliance a[], int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) sum += kwhPerDay(a[i]);
    return sum;
}

double totalWatts(const Appliance a[], int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) sum += a[i].watts;
    return sum;
}

bool validAppliance(const Appliance& a) {
    if (trim(a.name).empty()) return false;
    if (a.watts <= 0) return false;
    if (a.hours < 0 || a.hours > 24) return false;
    return true;
}

int findByName(const Appliance a[], int n, const string& name) {
    string key = lowerText(trim(name));
    for (int i = 0; i < n; i++) {
        if (lowerText(trim(a[i].name)) == key) return i;
    }
    return -1;
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

        Appliance tmp;
        tmp.name = trim(line.substr(0, p1));

        string wStr = trim(line.substr(p1 + 1, p2 - p1 - 1));
        string hStr = trim(line.substr(p2 + 1));

        try { tmp.watts = stod(wStr); tmp.hours = stod(hStr); }
        catch (...) { continue; }

        if (!validAppliance(tmp)) continue;
        if (n >= MAX_APPLIANCES) break;

        a[n] = tmp;
        n++;
    }
}

void addAppliance(Appliance a[], int& n) {
    Appliance x;
    x.name = readNonEmptyLine("Name: ");
    do { x.watts = readDouble("Watts (>0): "); } while (x.watts <= 0);
    do { x.hours = readDouble("Hours/day (0-24): "); } while (x.hours < 0 || x.hours > 24);

    if (!validAppliance(x)) {
        cout << "Invalid appliance data.\n";
        return;
    }

    int idx = findByName(a, n, x.name);
    if (idx != -1) {
        cout << "Appliance already exists: " << a[idx].name << "\n";
        cout << "Overwrite its watts/hours with new values? (y/n): ";
        char ch; cin >> ch; clearLine();
        if (ch == 'y' || ch == 'Y') {
            a[idx].watts = x.watts;
            a[idx].hours = x.hours;
            saveAppliances(a, n);
            cout << "Updated and saved.\n";
        } else {
            cout << "Not changed.\n";
        }
        return;
    }

    if (n >= MAX_APPLIANCES) {
        cout << "Limit reached.\n";
        return;
    }

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

    cout << "-------------------------------------------------\n";
    cout << "Total appliances: " << n << "\n";
    cout << "Total connected load (watts): " << totalWatts(a, n) << " W\n";
    cout << "Total daily energy: " << totalKwhPerDay(a, n) << " kWh/day\n";
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
        if (lowerText(a[i].name).find(q) != string::npos) {
            cout << "- " << a[i].name
                 << " | " << a[i].watts << " W"
                 << " | " << a[i].hours << " hrs"
                 << " | " << kwhPerDay(a[i]) << " kWh/day\n";
            found = true;
        }
    }
    if (!found) cout << "No match.\n";
}

void appendSummary(double tariff, int count,
                   double dayKwh, double dayCost,
                   double monthKwh, double monthCost) {
    ofstream out(BILLING_FILE.c_str(), ios::app);
    if (!out) {
        cout << "Error appending " << BILLING_FILE << "\n";
        return;
    }

    out << "================ BILLING SUMMARY ================\n";
    out << fixed << setprecision(2);
    out << "Tariff: " << tariff << " per kWh\n";
    out << "Appliances count: " << count << "\n";
    out << "Total daily energy: " << dayKwh << " kWh\n";
    out << "Total daily cost:  " << dayCost << "\n";
    out << "Estimated 30-day energy: " << monthKwh << " kWh\n";
    out << "Estimated 30-day cost:  " << monthCost << "\n";
    out << "=================================================\n\n";
}

void billing(const Appliance a[], int n) {
    if (n == 0) {
        cout << "No appliances. Add some first.\n";
        return;
    }

    double tariff;
    do { tariff = readDouble("Tariff per kWh (>0): "); } while (tariff <= 0);

    double dayKwh = totalKwhPerDay(a, n);
    double dayCost = dayKwh * tariff;
    double monthKwh = dayKwh * 30.0;
    double monthCost = dayCost * 30.0;

    cout << fixed << setprecision(2);
    cout << "\nTariff: " << tariff << " per kWh\n";
    cout << "Daily energy: " << dayKwh << " kWh\n";
    cout << "Daily cost:   " << dayCost << "\n";
    cout << "30-day energy: " << monthKwh << " kWh\n";
    cout << "30-day cost:   " << monthCost << "\n";

    cout << "Save summary to billing_summary.txt? (y/n): ";
    char ch;
    cin >> ch;
    clearLine();

    if (ch == 'y' || ch == 'Y') {
        appendSummary(tariff, n, dayKwh, dayCost, monthKwh, monthCost);
        cout << "Billing summary saved.\n";
    } else {
        cout << "Not saved.\n";
    }
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
        else if (choice == 4) billing(appliances, count);
        else if (choice == 5) { saveAppliances(appliances, count); cout << "Saved.\n"; }
        else if (choice == 6) { saveAppliances(appliances, count); cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }
    return 0;
}