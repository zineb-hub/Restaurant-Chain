#ifndef DATE_H
#define DATE_H
#pragma once
#include <string>
using namespace std;
class Date
{

public:
    Date();
    Date(int d, int m, int y);
    Date(int m, int y);
    bool operator>(const Date& d);
    bool operator==(Date& d);
    int operator-(Date& d);
    bool IsLeap();
    int Num_of_Days();
    Date input_date();
    int day;
    int month;
    int year;
    int set_day();
    int set_month();
    int set_year();
};
#endif
