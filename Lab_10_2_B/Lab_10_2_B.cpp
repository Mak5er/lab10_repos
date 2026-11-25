// Lab_10_2_B.cpp
// Reva Maksym, RI-13, Variant 23
// Lab 10.2 (Level B): Physical sort, index sort, and binary search with union

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

enum Specialnist { KN, INF, ME, FI, TN };

struct Student
{
    string prizv;
    int kurs;
    Specialnist spec;
    int fizyka;
    int matematyka;
    union
    {
        int programuvannia;
        int chyselni_metody;
        int pedahohika;
    };
};

string SpecToStr(Specialnist spec)
{
    switch (spec)
    {
    case KN:  return "Komp. nauky";
    case INF: return "Informatyka";
    case ME:  return "Mat. ta ekon.";
    case FI:  return "Fiz. ta inf.";
    case TN:  return "Trud. navch.";
    default:  return "Unknown";
    }
}

int GetThirdGrade(const Student& s)
{
    return s.programuvannia; // усі поля union (int) ділять ту саму область пам'яті
}

double AvgGrade(const Student& s)
{
    return (s.fizyka + s.matematyka + GetThirdGrade(s)) / 3.0;
}

int CompareAvg(double a, double b)
{
    const double eps = 0.01;
    if (fabs(a - b) < eps)
        return 0;
    return (a < b) ? -1 : 1;
}

// Ключі сортування: 1) назва спеціальності ↑, 2) середній бал ↑, 3) прізвище ↑
int CompareKeys(Specialnist specA, double avgA, const string& prizvA,
                Specialnist specB, double avgB, const string& prizvB)
{
    string sA = SpecToStr(specA);
    string sB = SpecToStr(specB);
    if (sA != sB)
        return (sA < sB) ? -1 : 1;

    int avgCmp = CompareAvg(avgA, avgB);
    if (avgCmp != 0)
        return avgCmp;

    if (prizvA != prizvB)
        return (prizvA < prizvB) ? -1 : 1;
    return 0;
}

int ReadInt(const string& prompt, int minVal, int maxVal)
{
    int val = 0;
    while (true)
    {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "Error! Enter integer [" << minVal << ".." << maxVal << "]." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double ReadDouble(const string& prompt, double minVal, double maxVal)
{
    double val = 0.0;
    while (true)
    {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cout << "Error! Enter number [" << minVal << ".." << maxVal << "]." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string ReadLine(const string& prompt)
{
    string s;
    while (true)
    {
        cout << prompt;
        getline(cin, s);
        if (!s.empty() && s.back() == '\r')
            s.pop_back();
        if (!s.empty())
            return s;
        cout << "Error! String cannot be empty." << endl;
    }
}

void Create(Student* p, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Student #" << i + 1 << ":" << endl;
        p[i].prizv = ReadLine("  prizvyshche: ");
        p[i].kurs = ReadInt("  kurs (1..6): ", 1, 6);
        int sc = ReadInt("  spec (0-KN, 1-INF, 2-ME, 3-FI, 4-TN): ", 0, 4);
        p[i].spec = static_cast<Specialnist>(sc);
        p[i].fizyka = ReadInt("  fizyka (2..5): ", 2, 5);
        p[i].matematyka = ReadInt("  matematyka (2..5): ", 2, 5);

        switch (p[i].spec)
        {
        case KN:  p[i].programuvannia = ReadInt("  programuvannia (2..5): ", 2, 5); break;
        case INF: p[i].chyselni_metody = ReadInt("  chyselni metody (2..5): ", 2, 5); break;
        default:  p[i].pedahohika = ReadInt("  pedahohika (2..5): ", 2, 5); break;
        }
        cout << endl;
    }
}

void AutoCreate(Student* p, const int N)
{
    const char* surnames[] = {
        "Ivanenko", "Petrenko", "Kovalenko", "Melnyk", "Shevchenko",
        "Bondar", "Tkachenko", "Kravchenko", "Oliinyk", "Savchenko",
        "Hnatiuk", "Boiko", "Yakovenko", "Lysenko", "Pavlenko",
        "Romanenko", "Polishchuk", "Marchenko", "Dmytruk", "Sydorenko"
    };
    const int pool = static_cast<int>(sizeof(surnames) / sizeof(surnames[0]));

    int perm[20];
    for (int i = 0; i < pool; i++)
        perm[i] = i;
    for (int i = pool - 1; i > 0; i--)
        swap(perm[i], perm[rand() % (i + 1)]);

    for (int i = 0; i < N; i++)
    {
        p[i].prizv = surnames[perm[i % pool]];
        p[i].kurs = 1 + rand() % 6;
        p[i].spec = static_cast<Specialnist>(rand() % 5);
        p[i].fizyka = 2 + rand() % 4;
        p[i].matematyka = 2 + rand() % 4;
        int g3 = 2 + rand() % 4;
        switch (p[i].spec)
        {
        case KN:  p[i].programuvannia = g3; break;
        case INF: p[i].chyselni_metody = g3; break;
        default:  p[i].pedahohika = g3; break;
        }
    }
}

void PrintTableRow(int rowNum, const Student& s)
{
    cout << "| " << setw(2) << right << rowNum << " "
         << "| " << setw(12) << left << s.prizv << " "
         << "| " << setw(4) << right << s.kurs << " "
         << "| " << setw(13) << left << SpecToStr(s.spec) << " "
         << "| " << setw(6) << right << s.fizyka << " "
         << "| " << setw(10) << right << s.matematyka << " ";

    switch (s.spec)
    {
    case KN:
        cout << "| " << setw(13) << right << s.programuvannia << " "
             << "| " << setw(11) << "" << " "
             << "| " << setw(10) << "" << " |" << endl;
        break;
    case INF:
        cout << "| " << setw(13) << "" << " "
             << "| " << setw(11) << right << s.chyselni_metody << " "
             << "| " << setw(10) << "" << " |" << endl;
        break;
    default:
        cout << "| " << setw(13) << "" << " "
             << "| " << setw(11) << "" << " "
             << "| " << setw(10) << right << s.pedahohika << " |" << endl;
        break;
    }
}

void Print(const Student* p, const int N)
{
    cout << "Spysok studentiv (Riven B):" << endl;
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << "| #  | Prizvyshche  | Kurs | Spetsialnist  | Fizyka | Matematyka | Programuvannia| Chys.metody | Pedahohika |" << endl;
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl;
    for (int i = 0; i < N; i++)
        PrintTableRow(i + 1, p[i]);
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl << endl;
}

void Sort(Student* p, const int N)
{
    for (int i0 = 0; i0 < N - 1; i0++)
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if (CompareKeys(p[i1].spec, AvgGrade(p[i1]), p[i1].prizv,
                            p[i1 + 1].spec, AvgGrade(p[i1 + 1]), p[i1 + 1].prizv) > 0)
                swap(p[i1], p[i1 + 1]);
}

int* IndexSort(const Student* p, const int N)
{
    int* I = new int[N];
    for (int i = 0; i < N; i++)
        I[i] = i;

    for (int i = 1; i < N; i++)
    {
        int val = I[i];
        int j = i - 1;
        while (j >= 0 &&
               CompareKeys(p[I[j]].spec, AvgGrade(p[I[j]]), p[I[j]].prizv,
                           p[val].spec, AvgGrade(p[val]), p[val].prizv) > 0)
        {
            I[j + 1] = I[j];
            j--;
        }
        I[j + 1] = val;
    }
    return I;
}

void PrintIndexSorted(const Student* p, const int* I, const int N)
{
    cout << "Spysok studentiv (indeksno vporyadkovanyi, Riven B):" << endl;
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << "| #  | Prizvyshche  | Kurs | Spetsialnist  | Fizyka | Matematyka | Programuvannia| Chys.metody | Pedahohika |" << endl;
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl;
    for (int i = 0; i < N; i++)
        PrintTableRow(i + 1, p[I[i]]);
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl << endl;
}

int BinSearch(const Student* p, const int N, const string& prizv, Specialnist spec, double avg)
{
    int L = 0, R = N - 1;
    while (L <= R)
    {
        int m = (L + R) / 2;
        int cmp = CompareKeys(p[m].spec, AvgGrade(p[m]), p[m].prizv, spec, avg, prizv);
        if (cmp == 0)
            return m;
        if (cmp < 0)
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}

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

    cout << "=== 4. Binarnyi poshuk (avtoperevirka) ===" << endl;
    cout << fixed << setprecision(2);
    cout << "  Poshuk isnuiuchoho: " << SpecToStr(target.spec)
         << ", ser. bal " << targetAvg << ", " << target.prizv << endl;
    cout << (found != -1 ? "  -> Znaiideno v pozytsii " + to_string(found + 1) + "." : "  -> Ne znaiideno.") << endl;
    cout << "  Poshuk vidsutnoho:  " << SpecToStr(KN) << ", ser. bal 5.00, Nevidomyi" << endl;
    cout << (notFound == -1 ? "  -> Shukanoho studenta ne znaiideno." : "  -> Znaiideno.") << endl;
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
