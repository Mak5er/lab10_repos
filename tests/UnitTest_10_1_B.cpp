// UnitTest_10_1_B.cpp
// Рева Максим
// Модульний тест для Lab_10_1_B.cpp (структура з об'єднанням union)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#define main lab_10_1_b_main
#include "../Lab_10_1_B/Lab_10_1_B.cpp"
#undef main

TEST_SUITE("UnitTest_10_1_B")
{
    TEST_CASE("GetThirdGrade and IsWithoutThrees work across all specialties and union fields")
    {
        Student kn{"Іваненко", 1, KN, 5, 4, {5}};
        Student inf{"Петренко", 2, INF, 4, 4, {4}};
        Student me{"Коваленко", 3, ME, 5, 5, {3}};
        Student fi{"Мельник", 2, FI, 4, 5, {5}};
        Student tn{"Ткач", 1, TN, 4, 4, {2}};

        CHECK(GetThirdGrade(kn) == 5);
        CHECK(GetThirdGrade(inf) == 4);
        CHECK(GetThirdGrade(me) == 3);
        CHECK(GetThirdGrade(fi) == 5);
        CHECK(GetThirdGrade(tn) == 2);

        CHECK(IsWithoutThrees(kn) == true);
        CHECK(IsWithoutThrees(inf) == true);
        CHECK(IsWithoutThrees(me) == false);
        CHECK(IsWithoutThrees(fi) == true);
        CHECK(IsWithoutThrees(tn) == false);
    }

    TEST_CASE("CountPhysicsFive counts students with physics grade 5")
    {
        Student group[3] = {
            {"Іваненко", 1, KN, 5, 5, {5}},
            {"Петренко", 2, INF, 5, 4, {4}},
            {"Бондар", 3, TN, 4, 5, {5}}
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
            int g3 = GetThirdGrade(group[i]);
            CHECK(g3 >= 2);
            CHECK(g3 <= 5);
        }
    }
}
