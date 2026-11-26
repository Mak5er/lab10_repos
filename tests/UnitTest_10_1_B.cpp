// UnitTest_10_1_B.cpp
// Reva Maksym
// Unit test for Lab_10_1_B.cpp (shared union memory)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#define main lab_10_1_b_main
#include "../Lab_10_1_B/Lab_10_1_B.cpp"
#undef main

TEST_SUITE("UnitTest_10_1_B")
{
    TEST_CASE("Union fields share memory and IsWithoutThrees works across all specialties")
    {
        Student kn{"Ivanenko", 1, KN, 5, 4, {5}};
        Student inf{"Petrenko", 2, INF, 4, 4, {0}};
        Student me{"Kovalenko", 3, ME, 5, 5, {0}};
        Student fi{"Melnyk", 2, FI, 4, 5, {0}};
        Student tn{"Tkach", 1, TN, 4, 4, {0}};

        inf.chyselni_metody = 4;
        me.pedahohika = 3;
        fi.pedahohika = 5;
        tn.pedahohika = 2;

        CHECK(inf.programuvannia == 4);
        CHECK(me.programuvannia == 3);
        CHECK(fi.programuvannia == 5);
        CHECK(tn.programuvannia == 2);

        CHECK(IsWithoutThrees(kn) == true);
        CHECK(IsWithoutThrees(inf) == true);
        CHECK(IsWithoutThrees(me) == false);
        CHECK(IsWithoutThrees(fi) == true);
        CHECK(IsWithoutThrees(tn) == false);
    }

    TEST_CASE("CountPhysicsFive counts students with physics grade 5")
    {
        Student group[3] = {
            {"Ivanenko", 1, KN, 5, 5, {5}},
            {"Petrenko", 2, INF, 5, 4, {4}},
            {"Bondar", 3, TN, 4, 5, {5}}
        };
        CHECK(CountPhysicsFive(group, 3) == 2);
    }

    TEST_CASE("AutoCreate generates valid union students within bounds")
    {
        srand(42);
        const int N = 8;
        Student group[N];
        AutoCreate(group, N);
        for (int i = 0; i < N; i++)
        {
            CHECK(!group[i].prizv.empty());
            CHECK(group[i].kurs >= 1);
            CHECK(group[i].kurs <= 6);
            CHECK(group[i].spec >= KN);
            CHECK(group[i].spec <= TN);
            CHECK(group[i].fizyka >= 2);
            CHECK(group[i].fizyka <= 5);
            CHECK(group[i].matematyka >= 2);
            CHECK(group[i].matematyka <= 5);
            CHECK(group[i].programuvannia >= 2);
            CHECK(group[i].programuvannia <= 5);
        }
    }
}
