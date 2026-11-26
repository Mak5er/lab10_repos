// UnitTest_10_2_B.cpp
// Reva Maksym
// Multi-module unit test for Lab_10_2_B (union + sort_search modules)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../Lab_10_2_B/student.h"
#include "../Lab_10_2_B/sort_search.h"

TEST_SUITE("UnitTest_10_2_B")
{
    TEST_CASE("AvgGrade reads shared union memory regardless of specialty")
    {
        Student s1{"Ivanenko", 1, KN, 5, 4, {5}};
        Student s2{"Petrenko", 2, INF, 4, 4, {0}};
        Student s3{"Boiko", 3, ME, 3, 4, {0}};

        s2.chyselni_metody = 5;
        s3.pedahohika = 5;

        CHECK(AvgGrade(s1) == doctest::Approx(14.0 / 3.0));
        CHECK(AvgGrade(s2) == doctest::Approx(13.0 / 3.0));
        CHECK(AvgGrade(s3) == doctest::Approx(4.0));
    }

    TEST_CASE("IndexSort, Sort, and BinSearch work with union-based Student")
    {
        Student group[4] = {
            {"Shevchenko", 2, FI, 4, 5, {5}}, // FI, avg 4.67
            {"Hnatiuk", 1, INF, 4, 4, {4}},   // INF, avg 4.00
            {"Ivanchuk", 3, KN, 5, 4, {5}},   // KN, avg 4.67
            {"Avramchuk", 2, KN, 5, 5, {4}}   // KN, avg 4.67
        };

        int* idx = IndexSort(group, 4);
        CHECK(idx[0] == 0); // FI: Shevchenko
        CHECK(idx[1] == 1); // INF: Hnatiuk
        CHECK(idx[2] == 3); // KN 4.67: Avramchuk
        CHECK(idx[3] == 2); // KN 4.67: Ivanchuk
        delete[] idx;

        Sort(group, 4);
        CHECK(BinSearch(group, 4, "Ivanchuk", KN, 4.67) == 3);
        CHECK(BinSearch(group, 4, "Ivanchuk", KN, 4.7) == 3);
        CHECK(BinSearch(group, 4, "Shevchenko", FI, 4.67) == 0);
        CHECK(BinSearch(group, 4, "Shevchenko", KN, 4.67) == -1);
    }

    TEST_CASE("AutoCreate generates valid union students compatible with Sort and BinSearch")
    {
        srand(77);
        const int N = 6;
        Student group[N];
        AutoCreate(group, N);
        for (int i = 0; i < N; i++)
        {
            CHECK(!group[i].prizv.empty());
            CHECK(group[i].programuvannia >= 2);
            CHECK(group[i].programuvannia <= 5);
        }
        Sort(group, N);
        double targetAvg = AvgGrade(group[0]);
        CHECK(BinSearch(group, N, group[0].prizv, group[0].spec, targetAvg) != -1);
    }
}
