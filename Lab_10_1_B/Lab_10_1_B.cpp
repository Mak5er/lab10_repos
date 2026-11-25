// Lab_10_1_B.cpp
// Рева Максим
// Лабораторна робота № 10.1 (Рівень В)
// Послідовний пошук в масиві структур з об'єднаннями
// Варіант 23

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

enum Specialnist
{
    KN,
    INF,
    ME,
    FI,
    TN
};

struct Student
{
    string prizv;
    int kurs;
    Specialnist spec;
    int fizyka;
    int matematyka;
    union
    {
        int programuvannia;
        int chyselni_metody;
        int pedahohika;
    };
};

// Підрахунок кількості видимих символів у UTF-8 рядку для вирівнювання таблиці
int Utf8Len(const string& s)
{
    int count = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80)
            count++;
    return count;
}

// Форматування рядка з вирівнюванням ліворуч із урахуванням UTF-8
string PadLeft(const string& s, int width)
{
    int len = Utf8Len(s);
    if (len >= width)
        return s;
    return s + string(width - len, ' ');
}

// Перетворення переліку спеціальності у текстову назву (без глобальних змінних)
string SpecToStr(Specialnist spec)
{
    switch (spec)
    {
    case KN:
        return "Комп'ютерні науки";
    case INF:
        return "Інформатика";
    case ME:
        return "Математика та економіка";
    case FI:
        return "Фізика та інформатика";
    case TN:
        return "Трудове навчання";
    default:
        return "Невідома";
    }
}

// Отримання оцінки з третього (профільного) предмету із об'єднання залежно від спеціальності
int GetThirdGrade(const Student& s)
{
    switch (s.spec)
    {
    case KN:
        return s.programuvannia;
    case INF:
        return s.chyselni_metody;
    default:
        return s.pedahohika;
    }
}

// Безпечне зчитування цілого числа у заданому діапазоні [minVal; maxVal]
int ReadIntInRange(const string& prompt, int minVal, int maxVal)
{
    int value = 0;
    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Помилка! Введіть ціле число від " << minVal
             << " до " << maxVal << "." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Безпечне зчитування непорожнього текстового рядка
string ReadNonEmptyLine(const string& prompt)
{
    string line;
    while (true)
    {
        cout << prompt;
        getline(cin, line);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (!line.empty())
            return line;
        cout << "Помилка! Рядок не може бути порожнім." << endl;
    }
}

// Формування масиву структур з об'єднаннями з клавіатури
void Create(Student* p, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;
        p[i].prizv = ReadNonEmptyLine("  прізвище: ");
        p[i].kurs = ReadIntInRange("  курс (1..6): ", 1, 6);

        int specCode = ReadIntInRange(
            "  спеціальність (0 - Комп'ютерні науки, 1 - Інформатика,\n"
            "                 2 - Математика та економіка, 3 - Фізика та інформатика,\n"
            "                 4 - Трудове навчання): ",
            0, 4);
        p[i].spec = static_cast<Specialnist>(specCode);

        p[i].fizyka = ReadIntInRange("  оцінка з фізики (2..5): ", 2, 5);
        p[i].matematyka = ReadIntInRange("  оцінка з математики (2..5): ", 2, 5);

        switch (p[i].spec)
        {
        case KN:
            p[i].programuvannia = ReadIntInRange("  оцінка з програмування (2..5): ", 2, 5);
            break;
        case INF:
            p[i].chyselni_metody = ReadIntInRange("  оцінка з чисельних методів (2..5): ", 2, 5);
            break;
        default:
            p[i].pedahohika = ReadIntInRange("  оцінка з педагогіки (2..5): ", 2, 5);
            break;
        }
        cout << endl;
    }
}

// Виведення сформованого масиву студентів у вигляді 9-стовпцевої таблиці
void Print(const Student* p, const int N)
{
    cout << "Список студентів (Рівень В):" << endl;
    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << "| №  | Прізвище       | Курс | Спеціальність           | Фізика | Математика | Програмування | Чис. методи | Педагогіка |" << endl;
    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;

    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(2) << right << i + 1 << " "
             << "| " << PadLeft(p[i].prizv, 14) << " "
             << "| " << setw(4) << right << p[i].kurs << " "
             << "| " << PadLeft(SpecToStr(p[i].spec), 23) << " "
             << "| " << setw(6) << right << p[i].fizyka << " "
             << "| " << setw(10) << right << p[i].matematyka << " ";

        switch (p[i].spec)
        {
        case KN:
            cout << "| " << setw(13) << right << p[i].programuvannia << " "
                 << "| " << setw(11) << right << "" << " "
                 << "| " << setw(10) << right << "" << " |" << endl;
            break;
        case INF:
            cout << "| " << setw(13) << right << "" << " "
                 << "| " << setw(11) << right << p[i].chyselni_metody << " "
                 << "| " << setw(10) << right << "" << " |" << endl;
            break;
        default:
            cout << "| " << setw(13) << right << "" << " "
                 << "| " << setw(11) << right << "" << " "
                 << "| " << setw(10) << right << p[i].pedahohika << " |" << endl;
            break;
        }
    }

    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << endl;
}

// Перевірка, чи навчається студент без трійок (на «відмінно» і «добре»)
bool IsWithoutThrees(const Student& s)
{
    return s.fizyka >= 4 && s.matematyka >= 4 && GetThirdGrade(s) >= 4;
}

// Виведення прізвищ студентів, які вчаться без трійок
void PrintStudentsWithoutThrees(const Student* p, const int N)
{
    cout << "Прізвища студентів, які вчаться без трійок (на \"відмінно\" і \"добре\"):" << endl;
    int k = 0;
    for (int i = 0; i < N; i++)
    {
        if (IsWithoutThrees(p[i]))
        {
            k++;
            cout << "  " << k << ". " << p[i].prizv << endl;
        }
    }
    if (k == 0)
        cout << "  (таких студентів немає)" << endl;
}

// Підрахунок кількості студентів, які отримали з фізики оцінку «5»
int CountPhysicsFive(const Student* p, const int N)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (p[i].fizyka == 5)
            count++;
    }
    return count;
}

int main()
{
    int N = ReadIntInRange("Введіть кількість студентів N (1..100): ", 1, 100);
    cout << endl;

    Student* p = new Student[N];

    Create(p, N);
    Print(p, N);

    PrintStudentsWithoutThrees(p, N);
    int physFiveCount = CountPhysicsFive(p, N);
    cout << "Кількість студентів, які отримали з фізики оцінку \"5\": "
         << physFiveCount << endl;

    delete[] p;
    return 0;
}
