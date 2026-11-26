// Lab_10_1_B.cpp
// Reva Maksym, RI-13, Variant 23
// Lab 10.1 (Level B): Sequential search in array of structures with union

#include <cerrno>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
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

int ReadInt(const string& prompt, int minVal, int maxVal)
{
    string s;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, s))
            return minVal;

        const char* start = s.c_str();
        while (*start == ' ' || *start == '\t' || *start == '\r')
            start++;

        if (*start != '\0')
        {
            char* end = nullptr;
            errno = 0;
            long val = strtol(start, &end, 10);
            while (end && (*end == ' ' || *end == '\t' || *end == '\r'))
                end++;
            if (errno == 0 && end && *end == '\0' && val >= minVal && val <= maxVal)
                return static_cast<int>(val);
        }
        cout << "Error! Enter integer [" << minVal << ".." << maxVal << "]." << endl;
    }
}

string ReadLine(const string& prompt)
{
    string s;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, s))
            return "";
        size_t first = s.find_first_not_of(" \t\r");
        if (first != string::npos)
        {
            size_t last = s.find_last_not_of(" \t\r");
            return s.substr(first, last - first + 1);
        }
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
        // Усі int-поля union займають ту саму область пам'яті, тому записуємо напряму
        p[i].programuvannia = 2 + rand() % 4;
    }
}

void Print(const Student* p, const int N)
{
    cout << "Spysok studentiv (Riven B):" << endl;
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << "| #  | Prizvyshche  | Kurs | Spetsialnist  | Fizyka | Matematyka | Programuvannia| Chys.metody | Pedahohika |" << endl;
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(2) << right << i + 1 << " "
             << "| " << setw(12) << left << p[i].prizv << " "
             << "| " << setw(4) << right << p[i].kurs << " "
             << "| " << setw(13) << left << SpecToStr(p[i].spec) << " "
             << "| " << setw(6) << right << p[i].fizyka << " "
             << "| " << setw(10) << right << p[i].matematyka << " ";

        switch (p[i].spec)
        {
        case KN:
            cout << "| " << setw(13) << right << p[i].programuvannia << " "
                 << "| " << setw(11) << "" << " "
                 << "| " << setw(10) << "" << " |" << endl;
            break;
        case INF:
            cout << "| " << setw(13) << "" << " "
                 << "| " << setw(11) << right << p[i].chyselni_metody << " "
                 << "| " << setw(10) << "" << " |" << endl;
            break;
        default:
            cout << "| " << setw(13) << "" << " "
                 << "| " << setw(11) << "" << " "
                 << "| " << setw(10) << right << p[i].pedahohika << " |" << endl;
            break;
        }
    }
    cout << "+----+--------------+------+---------------+--------+------------+---------------+-------------+------------+" << endl << endl;
}

bool IsWithoutThrees(const Student& s)
{
    return s.fizyka >= 4 && s.matematyka >= 4 && s.programuvannia >= 4;
}

void PrintStudentsWithoutThrees(const Student* p, const int N)
{
    cout << "Studenty bez triiok (na 4 i 5):" << endl;
    int k = 0;
    for (int i = 0; i < N; i++)
        if (IsWithoutThrees(p[i]))
            cout << "  " << ++k << ". " << p[i].prizv << endl;
    if (k == 0)
        cout << "  (nemaie)" << endl;
}

int CountPhysicsFive(const Student* p, const int N)
{
    int count = 0;
    for (int i = 0; i < N; i++)
        if (p[i].fizyka == 5)
            count++;
    return count;
}

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    int N = ReadInt("Vvedit kilkist studentiv N (1..100): ", 1, 100);
    int mode = ReadInt("Sposib zapovnennia (1 - avtogeneratsiia, 2 - vruchnu): ", 1, 2);
    cout << endl;

    Student* p = new Student[N];
    if (mode == 1)
        AutoCreate(p, N);
    else
        Create(p, N);

    Print(p, N);
    PrintStudentsWithoutThrees(p, N);
    cout << "Kilkist studentiv z otsinkoiu 5 z fizyky: "
         << CountPhysicsFive(p, N) << endl;

    delete[] p;
    return 0;
}
