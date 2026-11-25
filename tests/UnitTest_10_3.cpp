// UnitTest_10_3.cpp
// Рева Максим
// Модульний тест для Lab_10_3.cpp (Готель)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cstdio>

#define main lab_10_3_main
#include "../Lab_10_3/Lab_10_3.cpp"
#undef main

static Resident MakeResident(const string& prizv, const string& initials, int roomNumber)
{
    Resident r;
    memset(&r, 0, sizeof(Resident));
    CopyFixedStr(r.prizv, sizeof(r.prizv), prizv);
    CopyFixedStr(r.initials, sizeof(r.initials), initials);
    r.roomNumber = roomNumber;
    r.roomType = GetRoomTypeByNumber(roomNumber);
    return r;
}

TEST_SUITE("UnitTest_10_3")
{
    TEST_CASE("Room capacities, AddResident, EditResident, and RemoveResident")
    {
        Resident* p = nullptr;
        int N = 0;

        CHECK(GetRoomCapacity(1) == 1);
        CHECK(GetRoomCapacity(5) == 1);
        CHECK(GetRoomCapacity(6) == 2);
        CHECK(GetRoomCapacity(15) == 2);
        CHECK(GetRoomCapacity(16) == 0);

        CHECK(AddResident(p, N, MakeResident("Іваненко", "О.П.", 1)) == true);
        CHECK(AddResident(p, N, MakeResident("Петренко", "І.В.", 1)) == false); // Одномісний №1 зайнятий
        CHECK(AddResident(p, N, MakeResident("Петренко", "І.В.", 6)) == true);
        CHECK(AddResident(p, N, MakeResident("Сидоренко", "М.С.", 6)) == true);
        CHECK(AddResident(p, N, MakeResident("Бойко", "Т.А.", 6)) == false); // Двомісний №6 повний
        CHECK(N == 3);

        CHECK(EditResident(p, N, 1, MakeResident("Петренко", "І.В.", 7)) == true);
        CHECK(p[1].roomNumber == 7);

        CHECK(RemoveResident(p, N, 0) == true);
        CHECK(N == 2);
        CHECK(string(p[0].prizv) == "Петренко");

        delete[] p;
    }

    TEST_CASE("Surname lookup handles 0, 1, and multiple matches with initials")
    {
        Resident group[3] = {
            MakeResident("Коваленко", "А.І.", 2),
            MakeResident("Коваленко", "В.С.", 8),
            MakeResident("Мельник", "Д.О.", 10)
        };

        CHECK(CountBySurname(group, 3, "Шевченко") == 0);
        CHECK(CountBySurname(group, 3, "Мельник") == 1);
        CHECK(FindFirstBySurname(group, 3, "Мельник") == 2);

        CHECK(CountBySurname(group, 3, "Коваленко") == 2);
        CHECK(FindBySurnameAndInitials(group, 3, "Коваленко", "В.С.") == 1);
        CHECK(group[1].roomNumber == 8);
        CHECK(FindBySurnameAndInitials(group, 3, "Коваленко", "Х.Х.") == -1);
    }

    TEST_CASE("SaveToFile and LoadFromFile binary round-trip")
    {
        const char* testFile = "/tmp/lab10_3_test.bin";
        Resident* src = new Resident[2];
        src[0] = MakeResident("Бондар", "К.Л.", 3);
        src[1] = MakeResident("Ткачук", "Р.М.", 12);

        CHECK(SaveToFile(src, 2, testFile) == true);
        delete[] src;

        Resident* loaded = nullptr;
        int loadedN = 0;
        CHECK(LoadFromFile(loaded, loadedN, testFile) == true);
        CHECK(loadedN == 2);
        CHECK(string(loaded[0].prizv) == "Бондар");
        CHECK(loaded[0].roomNumber == 3);
        CHECK(loaded[0].roomType == SINGLE_ROOM);
        CHECK(string(loaded[1].initials) == "Р.М.");
        CHECK(loaded[1].roomNumber == 12);
        CHECK(loaded[1].roomType == DOUBLE_ROOM);

        delete[] loaded;
        std::remove(testFile);
    }
}
