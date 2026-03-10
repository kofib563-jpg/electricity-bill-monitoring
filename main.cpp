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

int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) { clearLine(); return x; }
        cout << "Invalid number. Try again.\n";
        clearLine();
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

void loadAppliances(Appliance a[], int& n) { n = 0; }
void saveAppliances(const Appliance a[], int n) {}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;

    loadAppliances(appliances, count);

    cout << "Electrical Load Monitoring & Billing System\n";
    cout << "Loaded appliances: " << count << "\n";

    while (true) {
        menu();
        int choice = readInt("Choose (1-6): ");

        if (choice == 6) {
            saveAppliances(appliances, count);
            cout << "Goodbye!\n";
            break;
        } else if (choice >= 1 && choice <= 5) {
            cout << "Feature not added yet (will appear in next parts).\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}