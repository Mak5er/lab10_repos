// student.h (Lab_10_2_B)
// Reva Maksym, RI-13, Variant 23
// Module 1: Data types (with union), validation, generation, and 9-column output

#pragma once

#include <string>

enum Specialnist { KN, INF, ME, FI, TN };

struct Student
{
    std::string prizv;
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

std::string SpecToStr(Specialnist spec);
double AvgGrade(const Student& s);

int ReadInt(const std::string& prompt, int minVal, int maxVal);
double ReadDouble(const std::string& prompt, double minVal, double maxVal);
std::string ReadLine(const std::string& prompt);

void Create(Student* p, const int N);
void AutoCreate(Student* p, const int N);
void PrintTableRow(int rowNum, const Student& s);
void Print(const Student* p, const int N);
