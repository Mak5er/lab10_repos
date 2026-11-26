// Lab_10_2_B.cpp
// Reva Maksym, RI-13, Variant 23
// Main module (Level B): menu orchestration and automated demo scenario

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#include "student.h"
#include "sort_search.h"

using namespace std;

void RunAllAuto(Student* p, const int N, bool& isCreated, bool& isSorted)
{
    AutoCreate(p, N);
    isCreated = true;
    isSorted = false;

    cout << "=== 1. Avtogenerovanyi masyv ===" << endl;
    Print(p, N);

    int* idx = IndexSort(p, N);
    cout << "=== 2. Indeksne vporyadkuvannia ===" << endl;
    PrintIndexSorted(p, idx, N);
    delete[] idx;

    Sort(p, N);
    isSorted = true;
    cout << "=== 3. Fizychne vporyadkuvannia ===" << endl;
    Print(p, N);

    const Student& target = p[N / 2];
    double targetAvg = round(AvgGrade(target) * 100.0) / 100.0;
    int found = BinSearch(p, N, target.prizv, target.spec, targetAvg);
    int notFound = BinSearch(p, N, "Nevidomyi", KN, 5.00);

    ios::fmtflags oldFlags = cout.flags();
    streamsize oldPrec = cout.precision();

    cout << "=== 4. Binarnyi poshuk (avtoperevirka) ===" << endl;
    cout << fixed << setprecision(2);
    cout << "  Poshuk isnuiuchoho: " << SpecToStr(target.spec)
         << ", ser. bal " << targetAvg << ", " << target.prizv << endl;
    cout << (found != -1 ? "  -> Znaiideno v pozytsii " + to_string(found + 1) + "." : "  -> Ne znaiideno.") << endl;
    cout << "  Poshuk vidsutnoho:  " << SpecToStr(KN) << ", ser. bal 5.00, Nevidomyi" << endl;
    cout << (notFound == -1 ? "  -> Shukanoho studenta ne znaiideno." : "  -> Znaiideno.") << endl;

    cout.flags(oldFlags);
    cout.precision(oldPrec);
}

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    int N = ReadInt("Vvedit kilkist studentiv N (1..100): ", 1, 100);
    Student* p = new Student[N];
    bool isCreated = false;
    bool isSorted = false;

    int menuItem = 0;
    do
    {
        cout << endl << "Vyberit diiu:" << endl;
        cout << "  [1] - vykonaty vsi dii avtomatychno (generatsiia + sortuvannia + poshuk)" << endl;
        cout << "  [2] - vvedennia danykh z klaviatury" << endl;
        cout << "  [3] - fizychne vporyadkuvannia danykh" << endl;
        cout << "  [4] - indeksne vporyadkuvannia ta vyvid danykh" << endl;
        cout << "  [5] - binarnyi poshuk studenta (vruchnu)" << endl;
        cout << "  [0] - vykhid" << endl;

        menuItem = ReadInt("Vvedit znachennia: ", 0, 5);
        cout << endl;

        switch (menuItem)
        {
        case 1:
            RunAllAuto(p, N, isCreated, isSorted);
            break;
        case 2:
            Create(p, N);
            isCreated = true;
            isSorted = false;
            Print(p, N);
            break;
        case 3:
            if (!isCreated) { cout << "Spochatku sformuite dani ([1] abo [2])." << endl; break; }
            Sort(p, N);
            isSorted = true;
            cout << "Masyv fizychno vporyadkovano." << endl << endl;
            Print(p, N);
            break;
        case 4:
        {
            if (!isCreated) { cout << "Spochatku sformuite dani ([1] abo [2])." << endl; break; }
            int* idx = IndexSort(p, N);
            PrintIndexSorted(p, idx, N);
            delete[] idx;
            break;
        }
        case 5:
        {
            if (!isCreated) { cout << "Spochatku sformuite dani ([1] abo [2])." << endl; break; }
            if (!isSorted) { cout << "Spochatku vykonaiite fizychne vporyadkuvannia ([3])." << endl; break; }
            Specialnist spec = static_cast<Specialnist>(ReadInt("  spec (0-KN, 1-INF, 2-ME, 3-FI, 4-TN): ", 0, 4));
            double avg = ReadDouble("  serednii bal (2.0..5.0): ", 2.0, 5.0);
            string prizv = ReadLine("  prizvyshche: ");
            int found = BinSearch(p, N, prizv, spec, avg);
            if (found != -1)
                cout << "Znaiideno studenta v pozytsii " << found + 1 << "." << endl;
            else
                cout << "Shukanoho studenta ne znaiideno." << endl;
            break;
        }
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] p;
    return 0;
}
