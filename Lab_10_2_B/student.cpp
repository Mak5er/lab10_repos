// student.cpp (Lab_10_2_B)
// Reva Maksym, RI-13, Variant 23
// Module 1 implementation: union handling, input validation, and 9-column output

#include "student.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <utility>

using namespace std;

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

double AvgGrade(const Student& s)
{
    // Усі int-поля union займають ту саму область пам'яті, тож читаємо напряму
    return (s.fizyka + s.matematyka + s.programuvannia) / 3.0;
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

double ReadDouble(const string& prompt, double minVal, double maxVal)
{
    string s;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, s))
            return minVal;

        for (size_t i = 0; i < s.size(); i++)
            if (s[i] == ',')
                s[i] = '.';

        const char* start = s.c_str();
        while (*start == ' ' || *start == '\t' || *start == '\r')
            start++;

        if (*start != '\0')
        {
            char* end = nullptr;
            errno = 0;
            double val = strtod(start, &end);
            while (end && (*end == ' ' || *end == '\t' || *end == '\r'))
                end++;
            if (errno == 0 && end && *end == '\0' && isfinite(val) && val >= minVal && val <= maxVal)
                return val;
        }
        cout << "Error! Enter number [" << minVal << ".." << maxVal << "]." << endl;
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
