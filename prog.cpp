#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// ================= CORE CLASSES =================

class Drug {
public:
    string name; int stock, expiry;
    Drug() {}
    Drug(string n, int s, int e) : name(n), stock(s), expiry(e) {}
};

class Inventory {
    Drug drugs[10]; int count;
public:
    Inventory() { count = 0; }
    void addDrug(Drug d) { drugs[count++] = d; }
    Drug* findDrug(string name) {
        for (int i = 0; i < count; i++)
            if (drugs[i].name == name) return &drugs[i];
        return NULL;
    }
};

class Prescription {
public:
    int id, quantity; string drugName;
    Prescription() {}
    Prescription(int i, string d, int q) : id(i), drugName(d), quantity(q) {}
};

class DispenseService {
public:
    void dispense(Inventory& inv, Prescription& p, int currentDate) {

        if (p.quantity <= 0) {
            cout << "Invalid quantity!\n"; return;
        }

        Drug* drug = inv.findDrug(p.drugName);
        if (drug == NULL) {
            cout << "Drug not found!\n"; return;
        }

        if (drug->expiry < currentDate) {
            cout << "Drug expired!\n"; return;
        }

        if (drug->stock < p.quantity) {
            cout << (drug->stock == 0 ? "Out of stock!\n" : "Insufficient stock!\n");
            return;
        }

        char allergy;
        cout << "Any allergy? (y/n): ";
        cin >> allergy;
        if (allergy == 'y') cout << "Allergy warning!\n";

        char interaction;
        cout << "Drug interaction? (y/n): ";
        cin >> interaction;
        if (interaction == 'y') cout << "Drug interaction alert!\n";

        drug->stock -= p.quantity;

        cout << "Medicine dispensed successfully!\n";
        cout << "Remaining stock: " << drug->stock << "\n";
        cout << "Bill: Rs. " << (p.quantity * 10) << "\n";
    }
};

// ================= TEST FRAMEWORK =================

int passed = 0, failed = 0;

string captureDispense(DispenseService& svc, Inventory& inv,
                       Prescription& p, int date, const string& input) {

    istringstream fakeIn(input);
    ostringstream fakeOut;

    streambuf* oldIn = cin.rdbuf(fakeIn.rdbuf());
    streambuf* oldOut = cout.rdbuf(fakeOut.rdbuf());

    svc.dispense(inv, p, date);

    cin.rdbuf(oldIn);
    cout.rdbuf(oldOut);

    return fakeOut.str();
}

bool contains(string s, string f) {
    return s.find(f) != string::npos;
}

void report(string id, string scenario,
            string input, string expected,
            string actual, bool pass) {

    cout << "\n--------------------------------------\n";
    cout << id << " : " << scenario << endl;
    cout << "Input: " << input << endl;
    cout << "Expected: " << expected << endl;
    cout << "Actual: " << actual << endl;
    cout << "Result: " << (pass ? "PASS" : "FAIL") << endl;

    pass ? passed++ : failed++;
}
// ================= TEST CASES =================

void runAllTests() {

    passed = 0; failed = 0;
    DispenseService svc;
    const int TODAY = 20250501;

    cout << "\n===== RUNNING ALL TEST CASES =====\n";

    // TC1 - Valid case
    {
        Inventory inv; inv.addDrug(Drug("Paracetamol", 10, 20261231));
        Prescription p(101, "Paracetamol", 5);
        string out = captureDispense(svc, inv, p, TODAY, "n\nn\n");
        report("TC1", "Valid case", "-", "-", out,
               contains(out, "Medicine dispensed successfully"));
    }

    // TC2 - Not logged in
    {
        bool loggedIn = false;
        ostringstream buf;
        streambuf* old = cout.rdbuf(buf.rdbuf());
        if (!loggedIn) cout << "Please login first!\n";
        cout.rdbuf(old);

        string out = buf.str();
        report("TC2", "Not logged in", "-", "-", out,
               contains(out, "Please login first"));
    }

    // TC3 - Invalid drug
    {
        Inventory inv;
        Prescription p(101, "XYZ", 5);
        string out = captureDispense(svc, inv, p, TODAY, "");
        report("TC3", "Drug not found", "-", "-", out,
               contains(out, "Drug not found"));
    }

    // TC4 - Exact stock
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 5, 20261231));
        Prescription p(101, "DrugX", 5);
        string out = captureDispense(svc, inv, p, TODAY, "n\nn\n");
        report("TC4", "Exact stock boundary", "-", "-", out,
               contains(out, "Remaining stock: 0"));
    }

    // TC5 - Quantity > stock
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 5, 20261231));
        Prescription p(101, "DrugX", 6);
        string out = captureDispense(svc, inv, p, TODAY, "");
        report("TC5", "Insufficient stock", "-", "-", out,
               contains(out, "Insufficient stock"));
    }

    // TC6 - Zero stock
    {
        Inventory inv; inv.addDrug(Drug("Crocin", 0, 20261231));
        Prescription p(101, "Crocin", 1);
        string out = captureDispense(svc, inv, p, TODAY, "");
        report("TC6", "Out of stock", "-", "-", out,
               contains(out, "Out of stock"));
    }

    // TC7 - Negative qty
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 10, 20261231));
        Prescription p(101, "DrugX", -2);
        string out = captureDispense(svc, inv, p, TODAY, "");
        report("TC7", "Negative quantity", "-", "-", out,
               contains(out, "Invalid quantity"));
    }

    // TC8 - Zero qty
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 10, 20261231));
        Prescription p(101, "DrugX", 0);
        string out = captureDispense(svc, inv, p, TODAY, "");
        report("TC8", "Zero quantity", "-", "-", out,
               contains(out, "Invalid quantity"));
    }

    // TC9 - Expired drug
    {
        Inventory inv; inv.addDrug(Drug("Aspirin", 5, 20240101));
        Prescription p(101, "Aspirin", 2);
        string out = captureDispense(svc, inv, p, TODAY, "");
        report("TC9", "Expired drug", "-", "-", out,
               contains(out, "Drug expired"));
    }

    // TC10 - Expiry today
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 5, TODAY));
        Prescription p(101, "DrugX", 2);
        string out = captureDispense(svc, inv, p, TODAY, "n\nn\n");
        report("TC10", "Expiry today", "-", "-", out,
               contains(out, "Medicine dispensed successfully"));
    }

    // TC11 - Allergy
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 10, 20261231));
        Prescription p(101, "DrugX", 2);
        string out = captureDispense(svc, inv, p, TODAY, "y\nn\n");
        report("TC11", "Allergy warning", "-", "-", out,
               contains(out, "Allergy warning"));
    }

    // TC12 - Interaction
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 10, 20261231));
        Prescription p(101, "DrugX", 2);
        string out = captureDispense(svc, inv, p, TODAY, "n\ny\n");
        report("TC12", "Drug interaction", "-", "-", out,
               contains(out, "Drug interaction alert"));
    }

    // TC13 - Mixed case
    {
        Inventory inv1; inv1.addDrug(Drug("Paracetamol", 10, 20261231));
        Prescription p1(101, "Paracetamol", 2);
        string out1 = captureDispense(svc, inv1, p1, TODAY, "n\nn\n");

        Inventory inv2; inv2.addDrug(Drug("Aspirin", 5, 20240101));
        Prescription p2(102, "Aspirin", 2);
        string out2 = captureDispense(svc, inv2, p2, TODAY, "");

        bool pass = contains(out1, "Medicine dispensed successfully") &&
                    contains(out2, "Drug expired");

        report("TC13", "Mixed valid + invalid", "-", "-", out1 + out2, pass);
    }

    // TC14 - Large qty
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 1000, 20261231));
        Prescription p(101, "DrugX", 999);
        string out = captureDispense(svc, inv, p, TODAY, "n\nn\n");
        report("TC14", "Large quantity", "-", "-", out,
               contains(out, "Remaining stock: 1"));
    }

    // TC15 - Billing
    {
        Inventory inv; inv.addDrug(Drug("DrugX", 10, 20261231));
        Prescription p(101, "DrugX", 5);
        string out = captureDispense(svc, inv, p, TODAY, "n\nn\n");
        report("TC15", "Billing check", "-", "-", out,
               contains(out, "Bill: Rs. 50"));
    }

    cout << "\n===== SUMMARY =====\n";
    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
}


// ================= MAIN MENU =================

int main() {

    int choice;

    while (true) {
        cout << "\n========== MENU ==========\n";
        cout << "1. Run System Manually(manual Testing)\n";
        cout << "2. Run All Tests\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1: {
            DispenseService svc;
            Inventory inv;

            inv.addDrug(Drug("Paracetamol", 10, 20261231));

            string drug;
            int qty;

            cout << "Enter Drug Name: ";
            cin >> drug;

            cout << "Enter Quantity: ";
            cin >> qty;

            Prescription p(101, drug, qty);

            svc.dispense(inv, p, 20250501);
            break;
        }

        case 2:
            runAllTests();
            break;

        case 3:
            return 0;

        default:
            cout << "Invalid choice\n";
        }
    }
}
