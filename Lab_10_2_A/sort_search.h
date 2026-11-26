// sort_search.h (Lab_10_2_A)
// Reva Maksym, RI-13, Variant 23
// Module 2: Comparators, physical sort, index sort, and binary search

#pragma once

#include <string>
#include "student.h"

int CompareAvg(double a, double b);
int CompareKeys(Specialnist specA, double avgA, const std::string& prizvA,
                Specialnist specB, double avgB, const std::string& prizvB);

void Sort(Student* p, const int N);
int* IndexSort(const Student* p, const int N);
void PrintIndexSorted(const Student* p, const int* I, const int N);
int BinSearch(const Student* p, const int N, const std::string& prizv, Specialnist spec, double avg);
