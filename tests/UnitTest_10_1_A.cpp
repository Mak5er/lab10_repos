// UnitTest_10_1_A.cpp
// Рева Максим
// Модульний тест для Lab_10_1_A.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#define main lab_10_1_a_main
#include "../Lab_10_1_A/Lab_10_1_A.cpp"
#undef main

TEST_SUITE("UnitTest_10_1_A")
{
    TEST_CASE("IsWithoutThrees checks grades 4 and 5 only")
    {
        Student s1{"Іваненко", 1, KN, 5, 5, 5};
        Student s2{"Петренко", 2, INF, 4, 5, 4};
        Student s3{"Коваленко", 3, ME, 5, 3, 5};
        Student s4{"Мельник", 4, FI, 4, 4, 2};

        CHECK(IsWithoutThrees(s1) == true);
        CHECK(IsWithoutThrees(s2) == true);
        CHECK(IsWithoutThrees(s3) == false);
        CHECK(IsWithoutThrees(s4) == false);
    }

    TEST_CASE("CountPhysicsFive counts exact grade 5 in physics")
    {
        Student group[4] = {
            {"Іваненко", 1, KN, 5, 4, 4},
            {"Петренко", 2, INF, 4, 5, 5},
            {"Сидоренко", 3, ME, 5, 3, 4},
            {"Бондар", 1, TN, 3, 4, 5}
        };
        CHECK(CountPhysicsFive(group, 4) == 2);

        Student none[2] = {
            {"Авраменко", 1, KN, 4, 5, 5},
            {"Гнатюк", 2, FI, 3, 4, 4}
        };
        CHECK(CountPhysicsFive(none, 2) == 0);
    }
}
