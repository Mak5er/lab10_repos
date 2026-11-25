// UnitTest_10_2_A.cpp
// Рева Максим
// Модульний тест для Lab_10_2_A.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#define main lab_10_2_a_main
#include "../Lab_10_2_A/Lab_10_2_A.cpp"
#undef main

TEST_SUITE("UnitTest_10_2_A")
{
    TEST_CASE("CompareUkrStrings respects Ukrainian alphabet order")
    {
        CHECK(CompareUkrStrings("Інформатика", "Комп'ютерні науки") < 0);
        CHECK(CompareUkrStrings("Комп'ютерні науки", "Математика та економіка") < 0);
        CHECK(CompareUkrStrings("Трудове навчання", "Фізика та інформатика") < 0);
        CHECK(CompareUkrStrings("Євтушенко", "Іваненко") < 0);
        CHECK(CompareUkrStrings("Іваненко", "Іваненко") == 0);
    }

    TEST_CASE("IndexSort preserves original array and orders by specialty, avg, surname")
    {
        Student group[5] = {
            {"Яковенко", 2, KN, 5, 5, 5},  // KN, avg 5.00
            {"Петренко", 1, INF, 4, 4, 4}, // INF, avg 4.00
            {"Авраменко", 3, KN, 4, 4, 5}, // KN, avg 4.33
            {"Іваненко", 1, KN, 4, 5, 4},  // KN, avg 4.33
            {"Бойко", 2, TN, 3, 4, 5}      // TN, avg 4.00
        };

        int* I = IndexSort(group, 5);
        // Порядок: INF (Петренко), KN avg 4.33 (Авраменко, Іваненко), KN avg 5.00 (Яковенко), TN (Бойко)
        CHECK(I[0] == 1);
        CHECK(I[1] == 2);
        CHECK(I[2] == 3);
        CHECK(I[3] == 0);
        CHECK(I[4] == 4);

        // Перевіряємо, що початковий масив не змінився
        CHECK(group[0].prizv == "Яковенко");
        CHECK(group[1].prizv == "Петренко");
        delete[] I;
    }

    TEST_CASE("Sort and BinSearch locate target student and reject absent keys")
    {
        Student group[5] = {
            {"Яковенко", 2, KN, 5, 5, 5},
            {"Петренко", 1, INF, 4, 4, 4},
            {"Авраменко", 3, KN, 4, 4, 5},
            {"Іваненко", 1, KN, 4, 5, 4},
            {"Бойко", 2, TN, 3, 4, 5}
        };

        Sort(group, 5);

        CHECK(group[0].prizv == "Петренко");
        CHECK(group[1].prizv == "Авраменко");
        CHECK(group[2].prizv == "Іваненко");
        CHECK(group[3].prizv == "Яковенко");
        CHECK(group[4].prizv == "Бойко");

        CHECK(BinSearch(group, 5, "Іваненко", KN, 4.33) == 2);
        CHECK(BinSearch(group, 5, "Яковенко", KN, 5.00) == 3);
        CHECK(BinSearch(group, 5, "Бойко", TN, 4.00) == 4);
        CHECK(BinSearch(group, 5, "Невідомий", KN, 4.33) == -1);
        CHECK(BinSearch(group, 5, "Іваненко", INF, 4.33) == -1);
    }
}
