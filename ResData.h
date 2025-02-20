#ifndef RESDATA_H
#define RESDATA_H
#pragma once
#include <iostream>
using namespace std;

struct Earnings
{
    Earnings();
    Earnings &operator=(const Earnings &obj);
    Earnings(double Sal, double Rati);
    void SetSales(double Sal);
    void SetRating(double Rati);
    double sales;
    double average_rating; // average rating in a day
    int num_of_peopleR;    // number of people who rated in a day
};

struct ResData
{
    string NameR = "default";
    Earnings Algerian[32];
    Earnings Syrian[32];
    Earnings Chinese[32];
    Earnings Indian[32];
    Earnings European[32];
    double cost[32] = {0};
    double publicity[32] = {0};

    ResData()
    {
        Algerian[31].average_rating =
            Syrian[31].average_rating =
                Chinese[31].average_rating =
                    Indian[31].average_rating =
                        European[31].average_rating = 0;
        Algerian[31].sales =
            Syrian[31].sales =
                Chinese[31].sales =
                    Indian[31].sales =
                        European[31].sales = 0;
        cost[31] = 0;
        publicity[31] = 0;
    }
    Earnings *GetArr(int arrNum);
    ResData &operator=(const ResData &Inc);
    void setCost(int index, double Cos);
    void setPublicity(int index, double Pub);
    int getRescost(int);
    ResData copy_obj();
    double calc_sum_Rating(Earnings *arr);
    double calc_sum_Sales(Earnings *arr);
    double calc_sum(double *arr);
    double calcPrizeCriterion(Earnings *arr);
};

#endif
