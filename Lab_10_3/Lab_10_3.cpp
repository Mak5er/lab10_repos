// Lab_10_3.cpp
// Рева Максим
// Лабораторна робота № 10.3
// Опрацювання масиву структур (Готель: 15 номерів — 5 одномісних, 10 двомісних)
// Варіант 23

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

enum RoomType
{
    SINGLE_ROOM = 1,
    DOUBLE_ROOM = 2
};

struct Resident
{
    char prizv[64];
    char initials[32];
    int roomNumber; // 1..5 — одномісний, 6..15 — двомісний
    RoomType roomType;
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

// Безпечне копіювання рядка у фіксований буфер структури (для бінарного запису у файл)
void CopyFixedStr(char* dst, size_t dstSize, const string& src)
{
    if (dstSize == 0)
        return;
    size_t copyLen = src.size();
    if (copyLen >= dstSize)
    {
        copyLen = dstSize - 1;
        while (copyLen > 0 && (static_cast<unsigned char>(src[copyLen]) & 0xC0) == 0x80)
            copyLen--;
    }
    memcpy(dst, src.data(), copyLen);
    dst[copyLen] = '\0';
}

// Визначення типу номера за його номером (1..5 — одномісний, 6..15 — двомісний)
RoomType GetRoomTypeByNumber(int roomNumber)
{
    return (roomNumber <= 5) ? SINGLE_ROOM : DOUBLE_ROOM;
}

// Визначення місткості номера (1 або 2; 0 — некоректний номер)
int GetRoomCapacity(int roomNumber)
{
    if (roomNumber >= 1 && roomNumber <= 5)
        return 1;
    if (roomNumber >= 6 && roomNumber <= 15)
        return 2;
    return 0;
}

// Текстове представлення типу номера (без глобальних змінних)
string RoomTypeToStr(RoomType type)
{
    switch (type)
    {
    case SINGLE_ROOM:
        return "одномісний";
    case DOUBLE_ROOM:
        return "двомісний";
    default:
        return "невідомий";
    }
}

// Підрахунок кількості мешканців у вказаному номері (опційно виключаючи індекс excludeIndex)
int CountOccupantsInRoom(const Resident* p, int N, int roomNumber, int excludeIndex = -1)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (i != excludeIndex && p[i].roomNumber == roomNumber)
            count++;
    }
    return count;
}

// Перевірка можливості поселення у вказаний номер
bool CanPlaceInRoom(const Resident* p, int N, int roomNumber, int excludeIndex = -1)
{
    int capacity = GetRoomCapacity(roomNumber);
    if (capacity == 0)
        return false;
    return CountOccupantsInRoom(p, N, roomNumber, excludeIndex) < capacity;
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

// Діалогове формування одного запису мешканця з перевіркою місткості номера
Resident ReadResidentData(const Resident* p, int N, int excludeIndex = -1)
{
    Resident r;
    memset(&r, 0, sizeof(Resident));

    string prizv = ReadNonEmptyLine("  прізвище: ");
    string initials = ReadNonEmptyLine("  ініціали (напр. І.І.): ");
    CopyFixedStr(r.prizv, sizeof(r.prizv), prizv);
    CopyFixedStr(r.initials, sizeof(r.initials), initials);

    while (true)
    {
        int room = ReadIntInRange("  номер помешкання (1..5 - одномісні, 6..15 - двомісні): ", 1, 15);
        if (CanPlaceInRoom(p, N, room, excludeIndex))
        {
            r.roomNumber = room;
            r.roomType = GetRoomTypeByNumber(room);
            break;
        }
        cout << "Помилка! Номер " << room << " (" << RoomTypeToStr(GetRoomTypeByNumber(room))
             << ") вже повністю зайнятий. Оберіть інший номер." << endl;
    }
    return r;
}

// Початкове заповнення динамічного масиву мешканців готелю
void CreateResidents(Resident*& p, int& N)
{
    int newN = ReadIntInRange("Введіть кількість мешканців (0..25): ", 0, 25);
    delete[] p;
    p = (newN > 0) ? new Resident[newN] : nullptr;
    N = 0;

    for (int i = 0; i < newN; i++)
    {
        cout << "Мешканець № " << i + 1 << ":" << endl;
        p[i] = ReadResidentData(p, i);
        N++;
        cout << endl;
    }
}

// Виведення списку мешканців готелю у вигляді таблиці
void PrintResidents(const Resident* p, int N)
{
    if (N == 0 || p == nullptr)
    {
        cout << "Список мешканців готелю порожній." << endl << endl;
        return;
    }

    cout << "Мешканці готелю (всього: " << N << "):" << endl;
    cout << "+----+----------------+----------+-------+------------+" << endl;
    cout << "| №  | Прізвище       | Ініціали | Номер | Тип номера |" << endl;
    cout << "+----+----------------+----------+-------+------------+" << endl;

    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(2) << right << i + 1 << " "
             << "| " << PadLeft(p[i].prizv, 14) << " "
             << "| " << PadLeft(p[i].initials, 8) << " "
             << "| " << setw(5) << right << p[i].roomNumber << " "
             << "| " << PadLeft(RoomTypeToStr(p[i].roomType), 10) << " |" << endl;
    }

    cout << "+----+----------------+----------+-------+------------+" << endl;
    cout << endl;
}

// Додавання нового мешканця у динамічний масив
bool AddResident(Resident*& p, int& N, const Resident& newRes)
{
    if (!CanPlaceInRoom(p, N, newRes.roomNumber))
        return false;

    Resident* temp = new Resident[N + 1];
    for (int i = 0; i < N; i++)
        temp[i] = p[i];
    temp[N] = newRes;

    delete[] p;
    p = temp;
    N++;
    return true;
}

// Вилучення мешканця за індексом з перерозподілом динамічного масиву
bool RemoveResident(Resident*& p, int& N, int index)
{
    if (index < 0 || index >= N)
        return false;

    if (N == 1)
    {
        delete[] p;
        p = nullptr;
        N = 0;
        return true;
    }

    Resident* temp = new Resident[N - 1];
    int j = 0;
    for (int i = 0; i < N; i++)
    {
        if (i != index)
            temp[j++] = p[i];
    }

    delete[] p;
    p = temp;
    N--;
    return true;
}

// Редагування запису мешканця за індексом
bool EditResident(Resident* p, int N, int index, const Resident& updatedRes)
{
    if (index < 0 || index >= N)
        return false;
    if (!CanPlaceInRoom(p, N, updatedRes.roomNumber, index))
        return false;

    p[index] = updatedRes;
    return true;
}

// Підрахунок кількості мешканців із вказаним прізвищем
int CountBySurname(const Resident* p, int N, const string& prizv)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (prizv == p[i].prizv)
            count++;
    }
    return count;
}

// Пошук першого індексу мешканця за прізвищем (-1, якщо не знайдено)
int FindFirstBySurname(const Resident* p, int N, const string& prizv)
{
    for (int i = 0; i < N; i++)
    {
        if (prizv == p[i].prizv)
            return i;
    }
    return -1;
}

// Пошук індексу мешканця за прізвищем та ініціалами (-1, якщо не знайдено)
int FindBySurnameAndInitials(const Resident* p, int N, const string& prizv, const string& initials)
{
    for (int i = 0; i < N; i++)
    {
        if (prizv == p[i].prizv && initials == p[i].initials)
            return i;
    }
    return -1;
}

// Діалог визначення номера помешкання за прізвищем (і за потреби — ініціалами)
void SearchResidentDialog(const Resident* p, int N)
{
    if (N == 0 || p == nullptr)
    {
        cout << "Список мешканців порожній." << endl;
        return;
    }

    string prizv = ReadNonEmptyLine("Введіть прізвище мешканця: ");
    int matches = CountBySurname(p, N, prizv);

    if (matches == 0)
    {
        cout << "Мешканця з прізвищем \"" << prizv << "\" у готелі немає." << endl;
    }
    else if (matches == 1)
    {
        int idx = FindFirstBySurname(p, N, prizv);
        cout << "Знайдено мешканця: " << p[idx].prizv
             << ", номер помешкання: " << p[idx].roomNumber
             << " (" << RoomTypeToStr(p[idx].roomType) << ")." << endl;
    }
    else
    {
        cout << "У готелі проживає декілька мешканців (" << matches
             << ") з прізвищем \"" << prizv << "\"." << endl;
        string initials = ReadNonEmptyLine("Введіть ініціали мешканця: ");
        int idx = FindBySurnameAndInitials(p, N, prizv, initials);
        if (idx != -1)
        {
            cout << "Знайдено мешканця: " << p[idx].prizv << " " << p[idx].initials
                 << ", номер помешкання: " << p[idx].roomNumber
                 << " (" << RoomTypeToStr(p[idx].roomType) << ")." << endl;
        }
        else
        {
            cout << "Мешканця \"" << prizv << " " << initials << "\" не знайдено." << endl;
        }
    }
}

// Запис усього масиву структур у бінарний файл
bool SaveToFile(const Resident* p, int N, const char* filename)
{
    ofstream fout(filename, ios::binary);
    if (!fout.is_open())
        return false;

    fout.write(reinterpret_cast<const char*>(&N), sizeof(N));
    for (int i = 0; i < N; i++)
        fout.write(reinterpret_cast<const char*>(&p[i]), sizeof(Resident));

    bool ok = fout.good();
    fout.close();
    return ok;
}

// Зчитування усього масиву структур із бінарного файлу
bool LoadFromFile(Resident*& p, int& N, const char* filename)
{
    ifstream fin(filename, ios::binary);
    if (!fin.is_open())
        return false;

    int fileN = 0;
    fin.read(reinterpret_cast<char*>(&fileN), sizeof(fileN));
    if (fin.fail() || fileN < 0 || fileN > 25)
    {
        fin.close();
        return false;
    }

    Resident* temp = (fileN > 0) ? new Resident[fileN] : nullptr;
    for (int i = 0; i < fileN; i++)
        fin.read(reinterpret_cast<char*>(&temp[i]), sizeof(Resident));

    if (fin.fail())
    {
        delete[] temp;
        fin.close();
        return false;
    }

    fin.close();
    delete[] p;
    p = temp;
    N = fileN;
    return true;
}

int main()
{
    Resident* p = nullptr;
    int N = 0;

    cout << "=== Система обліку мешканців готелю (15 номерів) ===" << endl;
    CreateResidents(p, N);

    int menuItem = 0;
    do
    {
        cout << endl;
        cout << "Виберіть дію:" << endl;
        cout << "  [1] - переформувати список мешканців" << endl;
        cout << "  [2] - вивести таблицю мешканців" << endl;
        cout << "  [3] - додати мешканця" << endl;
        cout << "  [4] - вилучити мешканця" << endl;
        cout << "  [5] - редагувати дані мешканця" << endl;
        cout << "  [6] - пошук номера помешкання за прізвищем" << endl;
        cout << "  [7] - записати дані у файл" << endl;
        cout << "  [8] - зчитати дані з файлу" << endl;
        cout << "  [0] - вихід та завершення роботи програми" << endl;

        menuItem = ReadIntInRange("Введіть значення: ", 0, 8);
        cout << endl;

        switch (menuItem)
        {
        case 1:
            CreateResidents(p, N);
            break;
        case 2:
            PrintResidents(p, N);
            break;
        case 3:
        {
            if (N >= 25)
            {
                cout << "Усі 15 номерів готелю (25 місць) заповнені." << endl;
                break;
            }
            cout << "Додавання нового мешканця:" << endl;
            Resident r = ReadResidentData(p, N);
            if (AddResident(p, N, r))
                cout << "Мешканця успішно додано." << endl;
            else
                cout << "Не вдалося додати мешканця." << endl;
            break;
        }
        case 4:
        {
            if (N == 0)
            {
                cout << "Список мешканців порожній." << endl;
                break;
            }
            PrintResidents(p, N);
            int idx = ReadIntInRange("Введіть № мешканця для вилучення: ", 1, N) - 1;
            if (RemoveResident(p, N, idx))
                cout << "Мешканця вилучено зі списку." << endl;
            else
                cout << "Помилка вилучення." << endl;
            break;
        }
        case 5:
        {
            if (N == 0)
            {
                cout << "Список мешканців порожній." << endl;
                break;
            }
            PrintResidents(p, N);
            int idx = ReadIntInRange("Введіть № мешканця для редагування: ", 1, N) - 1;
            cout << "Введіть оновлені дані для мешканця № " << idx + 1 << ":" << endl;
            Resident updated = ReadResidentData(p, N, idx);
            if (EditResident(p, N, idx, updated))
                cout << "Дані мешканця оновлено." << endl;
            else
                cout << "Помилка редагування." << endl;
            break;
        }
        case 6:
            SearchResidentDialog(p, N);
            break;
        case 7:
        {
            string filename = ReadNonEmptyLine("Введіть ім'я файлу для запису: ");
            if (SaveToFile(p, N, filename.c_str()))
                cout << "Дані успішно записано у файл \"" << filename << "\"." << endl;
            else
                cout << "Помилка запису у файл \"" << filename << "\"." << endl;
            break;
        }
        case 8:
        {
            string filename = ReadNonEmptyLine("Введіть ім'я файлу для зчитування: ");
            if (LoadFromFile(p, N, filename.c_str()))
                cout << "Дані успішно зчитано з файлу \"" << filename << "\"." << endl;
            else
                cout << "Помилка зчитування з файлу \"" << filename << "\"." << endl;
            break;
        }
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] p;
    return 0;
}
