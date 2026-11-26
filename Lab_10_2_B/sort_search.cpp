// sort_search.cpp (Lab_10_2_B)
// Reva Maksym, RI-13, Variant 23
// Module 2 implementation: sorting and binary search algorithms

#include "sort_search.h"

#include <cmath>
#include <iostream>
#include <utility>

using namespace std;

int CompareAvg(double a, double b)
{
    // Різниця сусідніх реальних середніх >= 1/3, тож eps = 0.05 допускає 4.66, 4.67 і 4.7
    const double eps = 0.05;
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
