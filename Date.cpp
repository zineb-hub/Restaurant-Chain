#include "Date.h"
#include <iostream>
using namespace std;

Date::Date()
{
    day = 1;
    month = 1;
    year = 0;
}
Date::Date(int d, int m, int y)
{
    if (d > 0 && d <= 31)
        day = d;
    if (m > 0 && m <= 12)
        month = m;
    if (y > 0)
        year = y;
}
Date::Date(int m, int y)
{
    if (m > 0 && m < 13)
        month = m;
    if (year > 0)
        year = y;
}
bool Date::operator>(const Date& d)
{
    if (year > d.year)
    {
        return true;
    }
    else if (year == d.year)
    {
        if (month > d.month)
            return true;
        else if (month == d.month)
            return day > d.day;
    }
    return false;
}

bool Date::operator==(Date& d)
{
    return (year == d.year) && (month == d.month) && (day == d.day);
}

int Date::operator-(Date& d)
{ // gives the difference between two dates in months returns the position in the array
    if ((*this) > d)
    {
        if (year == d.year)
            return month - d.month;

        return (year - d.year) * 12 + month - d.month;
    }
    else
        return -1;
}

bool Date::IsLeap()
{ // returns true if the year is leap
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::Num_of_Days()
{ // returns the number of days in that month
    if (month == 2)
    {
        return 28 + IsLeap();
    }
    return (month < 8) ? 30 + month % 2 : 31 - month % 2;
}

int Date::set_day()
{
    int a;
    cout << "Insert day: ";
    cin >> a;
    while (a <= 0 || a > 31)
    {
        cout << endl
            << "Invalid Day, repeat.";
        cin >> a;
    }
    return a;
}
int Date::set_month()
{
    int a;
    cout << "Insert month: ";
    cin >> a;
    while (a <= 0 || a > 12)
    {
        cout << endl
            << "Invalid Month, repeat.";
        cin >> a;
    }
    return a;
}
int Date::set_year()
{
    int a;
    cout << "Insert year: ";
    cin >> a;
    while (a <= 0)
    {
        cout << endl
            << "Invalid Year, repeat.";
        cin >> a;
    }
    return a;
}
Date Date::input_date()
{
    Date date;
    date.day = set_day();
    date.month = set_month();
    date.year = set_year();
    cout << "Date is: " << date.day << '/' << date.month << '/' << date.year << endl;
    return date;
}
