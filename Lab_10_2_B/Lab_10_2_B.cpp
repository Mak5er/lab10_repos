// Lab_10_2_B.cpp
// Рева Максим
// Лабораторна робота № 10.2 (Рівень В)
// Фізичне, індексне впорядкування та бінарний пошук в масиві структур з об'єднаннями
// Варіант 23

#include <cmath>
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

// Підрахунок кількості видимих символів у UTF-8 рядку
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

// Декодування чергової кодової точки UTF-8
unsigned int NextUtf8Codepoint(const string& s, size_t& pos)
{
    unsigned char b0 = static_cast<unsigned char>(s[pos]);
    if (b0 < 0x80)
    {
        pos += 1;
        return b0;
    }
    if ((b0 & 0xE0) == 0xC0 && pos + 1 < s.size())
    {
        unsigned int cp = ((b0 & 0x1F) << 6) |
                          (static_cast<unsigned char>(s[pos + 1]) & 0x3F);
        pos += 2;
        return cp;
    }
    if ((b0 & 0xF0) == 0xE0 && pos + 2 < s.size())
    {
        unsigned int cp = ((b0 & 0x0F) << 12) |
                          ((static_cast<unsigned char>(s[pos + 1]) & 0x3F) << 6) |
                          (static_cast<unsigned char>(s[pos + 2]) & 0x3F);
        pos += 3;
        return cp;
    }
    pos += 1;
    return b0;
}

// Порядковий ключ символу з урахуванням українського алфавіту
int UkrLetterOrder(unsigned int cp)
{
    switch (cp)
    {
    case 0x0410: return 100; // А
    case 0x0430: return 101; // а
    case 0x0411: return 102; // Б
    case 0x0431: return 103; // б
    case 0x0412: return 104; // В
    case 0x0432: return 105; // в
    case 0x0413: return 106; // Г
    case 0x0433: return 107; // г
    case 0x0490: return 108; // Ґ
    case 0x0491: return 109; // ґ
    case 0x0414: return 110; // Д
    case 0x0434: return 111; // д
    case 0x0415: return 112; // Е
    case 0x0435: return 113; // е
    case 0x0404: return 114; // Є
    case 0x0454: return 115; // є
    case 0x0416: return 116; // Ж
    case 0x0436: return 117; // ж
    case 0x0417: return 118; // З
    case 0x0437: return 119; // з
    case 0x0418: return 120; // И
    case 0x0438: return 121; // и
    case 0x0406: return 122; // І
    case 0x0456: return 123; // і
    case 0x0407: return 124; // Ї
    case 0x0457: return 125; // ї
    case 0x0419: return 126; // Й
    case 0x0439: return 127; // й
    case 0x041A: return 128; // К
    case 0x043A: return 129; // к
    case 0x041B: return 130; // Л
    case 0x043B: return 131; // л
    case 0x041C: return 132; // М
    case 0x043C: return 133; // м
    case 0x041D: return 134; // Н
    case 0x043D: return 135; // н
    case 0x041E: return 136; // О
    case 0x043E: return 137; // о
    case 0x041F: return 138; // П
    case 0x043F: return 139; // п
    case 0x0420: return 140; // Р
    case 0x0440: return 141; // р
    case 0x0421: return 142; // С
    case 0x0441: return 143; // с
    case 0x0422: return 144; // Т
    case 0x0442: return 145; // т
    case 0x0423: return 146; // У
    case 0x0443: return 147; // у
    case 0x0424: return 148; // Ф
    case 0x0444: return 149; // ф
    case 0x0425: return 150; // Х
    case 0x0445: return 151; // х
    case 0x0426: return 152; // Ц
    case 0x0446: return 153; // ц
    case 0x0427: return 154; // Ч
    case 0x0447: return 155; // ч
    case 0x0428: return 156; // Ш
    case 0x0448: return 157; // ш
    case 0x0429: return 158; // Щ
    case 0x0449: return 159; // щ
    case 0x042C: return 160; // Ь
    case 0x044C: return 161; // ь
    case 0x042E: return 162; // Ю
    case 0x044E: return 163; // ю
    case 0x042F: return 164; // Я
    case 0x044F: return 165; // я
    default:
        return static_cast<int>(cp >= 128 ? cp + 200 : cp);
    }
}

// Порівняння двох UTF-8 рядків за українським алфавітом (<0, 0, >0)
int CompareUkrStrings(const string& a, const string& b)
{
    size_t i = 0;
    size_t j = 0;
    while (i < a.size() && j < b.size())
    {
        int oa = UkrLetterOrder(NextUtf8Codepoint(a, i));
        int ob = UkrLetterOrder(NextUtf8Codepoint(b, j));
        if (oa < ob)
            return -1;
        if (oa > ob)
            return 1;
    }
    if (i < a.size())
        return 1;
    if (j < b.size())
        return -1;
    return 0;
}

// Перетворення переліку спеціальності у текстову назву
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

// Отримання профільної третьої оцінки з об'єднання
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

// Обчислення середнього балу студента (з урахуванням профільної третьої оцінки)
double AvgGrade(const Student& s)
{
    return (s.fizyka + s.matematyka + GetThirdGrade(s)) / 3.0;
}

// Порівняння середніх балів із допустимою точністю округлення до сотих
int CompareAvg(double a, double b)
{
    const double eps = 0.01;
    if (fabs(a - b) < eps)
        return 0;
    return (a < b) ? -1 : 1;
}

// Узгоджене порівняння ключів (1 - назва спеціальності, 2 - середній бал, 3 - прізвище)
int CompareKeys(Specialnist specA, double avgA, const string& prizvA,
                Specialnist specB, double avgB, const string& prizvB)
{
    int specCmp = CompareUkrStrings(SpecToStr(specA), SpecToStr(specB));
    if (specCmp != 0)
        return specCmp;

    int avgCmp = CompareAvg(avgA, avgB);
    if (avgCmp != 0)
        return avgCmp;

    return CompareUkrStrings(prizvA, prizvB);
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

// Безпечне зчитування дійсного числа у заданому діапазоні [minVal; maxVal]
double ReadDoubleInRange(const string& prompt, double minVal, double maxVal)
{
    double value = 0.0;
    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Помилка! Введіть дійсне число від " << minVal
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

// Виведення одного рядка 9-стовпцевої таблиці
void PrintTableRow(int rowNum, const Student& s)
{
    cout << "| " << setw(2) << right << rowNum << " "
         << "| " << PadLeft(s.prizv, 14) << " "
         << "| " << setw(4) << right << s.kurs << " "
         << "| " << PadLeft(SpecToStr(s.spec), 23) << " "
         << "| " << setw(6) << right << s.fizyka << " "
         << "| " << setw(10) << right << s.matematyka << " ";

    switch (s.spec)
    {
    case KN:
        cout << "| " << setw(13) << right << s.programuvannia << " "
             << "| " << setw(11) << right << "" << " "
             << "| " << setw(10) << right << "" << " |" << endl;
        break;
    case INF:
        cout << "| " << setw(13) << right << "" << " "
             << "| " << setw(11) << right << s.chyselni_metody << " "
             << "| " << setw(10) << right << "" << " |" << endl;
        break;
    default:
        cout << "| " << setw(13) << right << "" << " "
             << "| " << setw(11) << right << "" << " "
             << "| " << setw(10) << right << s.pedahohika << " |" << endl;
        break;
    }
}

// Виведення масиву студентів у вигляді 9-стовпцевої таблиці
void Print(const Student* p, const int N)
{
    cout << "Список студентів (Рівень В):" << endl;
    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << "| №  | Прізвище       | Курс | Спеціальність           | Фізика | Математика | Програмування | Чис. методи | Педагогіка |" << endl;
    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;

    for (int i = 0; i < N; i++)
        PrintTableRow(i + 1, p[i]);

    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << endl;
}

// Фізичне впорядкування масиву методом бульбашки (спеціальність ↑, середній бал ↑, прізвище ↑)
void Sort(Student* p, const int N)
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++)
    {
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
        {
            if (CompareKeys(p[i1].spec, AvgGrade(p[i1]), p[i1].prizv,
                            p[i1 + 1].spec, AvgGrade(p[i1 + 1]), p[i1 + 1].prizv) > 0)
            {
                tmp = p[i1];
                p[i1] = p[i1 + 1];
                p[i1 + 1] = tmp;
            }
        }
    }
}

// Індексне впорядкування масиву методом вставки (основний масив не змінюється)
int* IndexSort(const Student* p, const int N)
{
    int* I = new int[N];
    for (int i = 0; i < N; i++)
        I[i] = i;

    for (int i = 1; i < N; i++)
    {
        int value = I[i];
        int j = i - 1;
        while (j >= 0 &&
               CompareKeys(p[I[j]].spec, AvgGrade(p[I[j]]), p[I[j]].prizv,
                           p[value].spec, AvgGrade(p[value]), p[value].prizv) > 0)
        {
            I[j + 1] = I[j];
            j--;
        }
        I[j + 1] = value;
    }

    return I;
}

// Виведення масиву студентів згідно з індексним масивом
void PrintIndexSorted(const Student* p, const int* I, const int N)
{
    cout << "Список студентів (індексно впорядкований, Рівень В):" << endl;
    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << "| №  | Прізвище       | Курс | Спеціальність           | Фізика | Математика | Програмування | Чис. методи | Педагогіка |" << endl;
    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;

    for (int i = 0; i < N; i++)
        PrintTableRow(i + 1, p[I[i]]);

    cout << "+----+----------------+------+-------------------------+--------+------------+---------------+-------------+------------+" << endl;
    cout << endl;
}

// Бінарний пошук у фізично впорядкованому масиві за ключами (спеціальність, середній бал, прізвище)
int BinSearch(const Student* p, const int N, const string& prizv, Specialnist spec, double avg)
{
    int L = 0;
    int R = N - 1;
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

int main()
{
    int N = ReadIntInRange("Введіть кількість студентів N (1..100): ", 1, 100);
    Student* p = new Student[N];
    bool isCreated = false;
    bool isSorted = false;

    int menuItem = 0;
    do
    {
        cout << endl;
        cout << "Виберіть дію:" << endl;
        cout << "  [1] - введення даних з клавіатури" << endl;
        cout << "  [2] - вивід даних на екран" << endl;
        cout << "  [3] - фізичне впорядкування даних" << endl;
        cout << "  [4] - індексне впорядкування та вивід даних" << endl;
        cout << "  [5] - бінарний пошук студента" << endl;
        cout << "  [0] - вихід та завершення роботи програми" << endl;

        menuItem = ReadIntInRange("Введіть значення: ", 0, 5);
        cout << endl;

        switch (menuItem)
        {
        case 1:
            Create(p, N);
            isCreated = true;
            isSorted = false;
            break;
        case 2:
            if (!isCreated)
            {
                cout << "Спочатку введіть дані студентів (пункт [1])." << endl;
                break;
            }
            Print(p, N);
            break;
        case 3:
            if (!isCreated)
            {
                cout << "Спочатку введіть дані студентів (пункт [1])." << endl;
                break;
            }
            Sort(p, N);
            isSorted = true;
            cout << "Масив фізично впорядковано." << endl;
            break;
        case 4:
        {
            if (!isCreated)
            {
                cout << "Спочатку введіть дані студентів (пункт [1])." << endl;
                break;
            }
            int* idx = IndexSort(p, N);
            PrintIndexSorted(p, idx, N);
            delete[] idx;
            break;
        }
        case 5:
        {
            if (!isCreated)
            {
                cout << "Спочатку введіть дані студентів (пункт [1])." << endl;
                break;
            }
            if (!isSorted)
            {
                cout << "Для бінарного пошуку спочатку виконайте фізичне впорядкування (пункт [3])." << endl;
                break;
            }
            cout << "Введіть ключі пошуку:" << endl;
            int specCode = ReadIntInRange(
                "  спеціальність (0 - Комп'ютерні науки, 1 - Інформатика,\n"
                "                 2 - Математика та економіка, 3 - Фізика та інформатика,\n"
                "                 4 - Трудове навчання): ",
                0, 4);
            Specialnist spec = static_cast<Specialnist>(specCode);
            double avg = ReadDoubleInRange("  середній бал (2.0..5.0): ", 2.0, 5.0);
            string prizv = ReadNonEmptyLine("  прізвище: ");

            int found = BinSearch(p, N, prizv, spec, avg);
            if (found != -1)
                cout << "Знайдено студента в позиції " << found + 1 << "." << endl;
            else
                cout << "Шуканого студента не знайдено." << endl;
            break;
        }
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] p;
    return 0;
}
