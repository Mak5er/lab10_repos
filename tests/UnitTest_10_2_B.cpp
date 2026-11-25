// UnitTest_10_2_B.cpp
// Рева Максим
// Модульний тест для Lab_10_2_B.cpp (структура з об'єднанням union)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#define main lab_10_2_b_main
#include "../Lab_10_2_B/Lab_10_2_B.cpp"
#undef main

TEST_SUITE("UnitTest_10_2_B")
{
    TEST_CASE("AvgGrade uses union field according to specialty")
    {
        Student s1{"Іваненко", 1, KN, 5, 4, {5}};
        Student s2{"Петренко", 2, INF, 4, 4, {5}};
        Student s3{"Бойко", 3, ME, 3, 4, {5}};

        CHECK(AvgGrade(s1) == doctest::Approx(14.0 / 3.0));
        CHECK(AvgGrade(s2) == doctest::Approx(13.0 / 3.0));
        CHECK(AvgGrade(s3) == doctest::Approx(4.0));
    }

    TEST_CASE("IndexSort, Sort, and BinSearch work with union-based Student")
    {
        Student group[4] = {
            {"Шевченко", 2, FI, 4, 5, {5}}, // FI, avg 4.67
            {"Гнатюк", 1, INF, 4, 4, {4}},  // INF, avg 4.00
            {"Іванчук", 3, KN, 5, 4, {5}},  // KN, avg 4.67
            {"Аврамчук", 2, KN, 5, 5, {4}}  // KN, avg 4.67
        };

        int* idx = IndexSort(group, 4);
        CHECK(idx[0] == 1); // INF: Гнатюк
        CHECK(idx[1] == 3); // KN 4.67: Аврамчук
        CHECK(idx[2] == 2); // KN 4.67: Іванчук
        CHECK(idx[3] == 0); // FI 4.67: Шевченко
        delete[] idx;

        Sort(group, 4);
        CHECK(BinSearch(group, 4, "Іванчук", KN, 4.67) == 2);
        CHECK(BinSearch(group, 4, "Шевченко", FI, 4.67) == 3);
        CHECK(BinSearch(group, 4, "Шевченко", KN, 4.67) == -1);
    }
}
