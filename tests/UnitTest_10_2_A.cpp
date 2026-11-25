// UnitTest_10_2_A.cpp
// Reva Maksym
// Unit test for Lab_10_2_A.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#define main lab_10_2_a_main
#include "../Lab_10_2_A/Lab_10_2_A.cpp"
#undef main

TEST_SUITE("UnitTest_10_2_A")
{
    TEST_CASE("CompareKeys orders by specialty name, average grade, and surname")
    {
        // Fiz. ta inf. < Informatyka < Komp. nauky < Mat. ta ekon. < Trud. navch.
        CHECK(CompareKeys(FI, 5.0, "Zubenko", INF, 2.0, "Avramenko") < 0);
        CHECK(CompareKeys(INF, 5.0, "Zubenko", KN, 2.0, "Avramenko") < 0);
        CHECK(CompareKeys(KN, 4.0, "Zubenko", KN, 4.33, "Avramenko") < 0);
        CHECK(CompareKeys(KN, 4.33, "Avramenko", KN, 4.33, "Ivanenko") < 0);
        CHECK(CompareKeys(KN, 4.33, "Ivanenko", KN, 13.0 / 3.0, "Ivanenko") == 0);
    }

    TEST_CASE("IndexSort preserves original array and orders correctly")
    {
        Student group[5] = {
            {"Yakovenko", 2, KN, 5, 5, 5},  // KN, avg 5.00
            {"Petrenko", 1, INF, 4, 4, 4},  // INF, avg 4.00
            {"Avramenko", 3, KN, 4, 4, 5},  // KN, avg 4.33
            {"Ivanenko", 1, KN, 4, 5, 4},   // KN, avg 4.33
            {"Boiko", 2, TN, 3, 4, 5}       // TN, avg 4.00
        };

        int* I = IndexSort(group, 5);
        CHECK(I[0] == 1); // INF: Petrenko
        CHECK(I[1] == 2); // KN 4.33: Avramenko
        CHECK(I[2] == 3); // KN 4.33: Ivanenko
        CHECK(I[3] == 0); // KN 5.00: Yakovenko
        CHECK(I[4] == 4); // TN: Boiko

        CHECK(group[0].prizv == "Yakovenko");
        CHECK(group[1].prizv == "Petrenko");
        delete[] I;
    }

    TEST_CASE("Sort and BinSearch locate target student and reject absent keys")
    {
        Student group[5] = {
            {"Yakovenko", 2, KN, 5, 5, 5},
            {"Petrenko", 1, INF, 4, 4, 4},
            {"Avramenko", 3, KN, 4, 4, 5},
            {"Ivanenko", 1, KN, 4, 5, 4},
            {"Boiko", 2, TN, 3, 4, 5}
        };

        Sort(group, 5);

        CHECK(group[0].prizv == "Petrenko");
        CHECK(group[1].prizv == "Avramenko");
        CHECK(group[2].prizv == "Ivanenko");
        CHECK(group[3].prizv == "Yakovenko");
        CHECK(group[4].prizv == "Boiko");

        CHECK(BinSearch(group, 5, "Ivanenko", KN, 4.33) == 2);
        CHECK(BinSearch(group, 5, "Yakovenko", KN, 5.00) == 3);
        CHECK(BinSearch(group, 5, "Boiko", TN, 4.00) == 4);
        CHECK(BinSearch(group, 5, "Nevidomyi", KN, 4.33) == -1);
        CHECK(BinSearch(group, 5, "Ivanenko", INF, 4.33) == -1);
    }

    TEST_CASE("AutoCreate generates valid students compatible with Sort and BinSearch")
    {
        srand(77);
        const int N = 6;
        Student group[N];
        AutoCreate(group, N);
        for (int i = 0; i < N; i++)
        {
            CHECK(!group[i].prizv.empty());
            CHECK(group[i].kurs >= 1);
            CHECK(group[i].kurs <= 6);
            CHECK(group[i].fizyka >= 2);
            CHECK(group[i].fizyka <= 5);
        }
        Sort(group, N);
        double targetAvg = AvgGrade(group[0]);
        CHECK(BinSearch(group, N, group[0].prizv, group[0].spec, targetAvg) != -1);
    }
}
