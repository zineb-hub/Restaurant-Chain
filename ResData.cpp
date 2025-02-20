#include "ResData.h"
#include <iostream>
#include <cmath>
using namespace std;

Earnings::Earnings() : sales(-1.0), average_rating(0.0), num_of_peopleR(0) {} // Sales set to -1 so that we differentiate between 0 sales and no sales at all (so we know the end of the month)

Earnings &Earnings::operator=(const Earnings &obj)
{ // copy assignment
    sales = obj.sales;
    average_rating = obj.average_rating;
    num_of_peopleR = obj.num_of_peopleR;
    return *this;
}

Earnings::Earnings(double Sal, double Rati)
{
    sales = Sal;
    if (Rati <= 5 && Rati >= 0)
    {
        average_rating = Rati;
    }
}
void Earnings::SetSales(double Sal)
{
    if (Sal >= 0)
        sales = Sal;
}
void Earnings::SetRating(double Rati)
{ // calculate new average rating based on people who rated
    if (Rati <= 5 && Rati >= 0)
    {
        num_of_peopleR++;
        average_rating = (average_rating * (num_of_peopleR - 1) + Rati) / num_of_peopleR;
    }
}

Earnings *ResData::GetArr(int arrNum)
{ // returns the array of a specific cuisine(used for find winner)
    switch (arrNum)
    {
    case 1:
        return Algerian;
    case 2:
        return Syrian;
    case 3:
        return Chinese;
    case 4:
        return Indian;
    case 5:
        return European;
    default:
        return nullptr;
    }
}

ResData &ResData::operator=(const ResData &Inc)
{ // copy assignment
    NameR = Inc.NameR;
    for (int i = 0; i < 32; i++)
    {
        Algerian[i].average_rating = Inc.Algerian[i].average_rating;
        Algerian[i].sales = Inc.Algerian[i].sales;
        Algerian[i].num_of_peopleR = Inc.Algerian[i].num_of_peopleR;

        Syrian[i].average_rating = Inc.Syrian[i].average_rating;
        Syrian[i].sales = Inc.Syrian[i].sales;
        Syrian[i].num_of_peopleR = Inc.Syrian[i].num_of_peopleR;

        Chinese[i].average_rating = Inc.Chinese[i].average_rating;
        Chinese[i].sales = Inc.Chinese[i].sales;
        Chinese[i].num_of_peopleR = Inc.Chinese[i].num_of_peopleR;

        Indian[i].average_rating = Inc.Indian[i].average_rating;
        Indian[i].sales = Inc.Indian[i].sales;
        Indian[i].num_of_peopleR = Inc.Indian[i].num_of_peopleR;

        European[i].average_rating = Inc.European[i].average_rating;
        European[i].sales = Inc.European[i].sales;
        European[i].num_of_peopleR = Inc.European[i].num_of_peopleR;

        cost[i] = Inc.cost[i];
        publicity[i] = Inc.publicity[i];
    }
    return *this;
}

void ResData::setCost(int index, double Cos)
{ // takes the index of the array we want to set the cost in
    if (Cos >= 0)
        cost[index] = Cos;
}
int ResData::getRescost(int ind)
{
    return cost[ind];
}
void ResData::setPublicity(int index, double Pub)
{ // takes the index of the array we want to set the publicity in
    if (Pub >= 0)
        publicity[index] = Pub;
}

ResData ResData::copy_obj()
{ // copies only the sums (for creating the tree of next month)
    ResData Earnings;
    Earnings.NameR = NameR;
    Earnings.Algerian[31] = Algerian[31];
    Earnings.Syrian[31] = Syrian[31];
    Earnings.Chinese[31] = Chinese[31];
    Earnings.Indian[31] = Indian[31];
    Earnings.European[31] = European[31];
    return Earnings;
}

double ResData::calc_sum_Sales(Earnings *arr)
{ // calculates the sum of all the sales
    double sum = 0;
    for (int i = 0; i < 31; i++)
    {
        if (arr[i].sales != -1)
            sum += arr[i].sales;
    }
    return sum;
}
double ResData::calc_sum_Rating(Earnings *arr)
{ // calculates the sum of all the sales

    double sum = 0;
    int people = 0;
    for (int i = 0; i < 31; i++)
    {
        if (arr[i].average_rating != -1)
            sum += arr[i].average_rating;
        people += arr[i].num_of_peopleR;
    }
    if (people != 0)
        return std::round(sum / people * 100) / 100;
    return -1;
}
double ResData::calc_sum(double *arr)
{
    double sum = 0;
    for (int i = 0; i < 31; i++)
    {
        if (arr[i] != 0)
            sum += arr[i];
    }
    return sum;
}
double ResData::calcPrizeCriterion(Earnings *arr)
{ // calculates the value by which the restaurants will be sorted to get the prize winner
    if (arr[31].sales != -1)
    {
        return arr[31].sales + (arr[31].average_rating) / 100;
    }
    return -1;
}
