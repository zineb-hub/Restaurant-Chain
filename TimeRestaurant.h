#ifndef TIMERESTAURANT_H
#define TIMERESTAURANT_H

#pragma once
#include "ResDataBST.h"
#include "Date.h"
#include "wilaya.h"
#include <ctime>

struct node
{
    Date d;            // date object
    ResDataBST BStree; // BST
    node(int month, int year) : d(month, year) {}
    node() {}
};
class TimeRestaurant
{
private:
    int maxSize = 12; // size of the array
    node *aptr;       // the array
    Date lastDate;    // last insertion date

    void getDailySales(string, Date);
    void getDailyCost(string, Date);
    void getDailyPublicity(string, Date);
    void getCuisineRatingMonth(string, Date);
    void getCuisineRatingPeriod(string, Date, Date);
    void modifySales(string, Date, double, char);
    void modifyCost(string, Date, double);
    void modifyPublicity(string, Date, double);
    void setTodaySale(string, Date, double, string);
    void setTodayCost(string, Date, double);
    void setTodayPublicity(string, Date, double);
    bool reSize(Date &);
    void setTodayRating(string, Date, string);
    void setLastDate(const Date &);
    bool checkIfNextDate();
    void change(const Date &);
    void setTodayRating(string name, Date d, string option, double value); // to use this function in file handling by inserting one value that is the average
    Date getCurrentDate();
    Date getSelectedDate();
    int getPosition(Date);

    // place
    double districtSales(string wil, string cit, string dist, const wilaya::RestauNode *);
    double getRatioRestaurant(Date, Date, string); // takes the start date and end date if we're not in a period of time then it will be the same date
    void RatioCity(Date, Date, string, string, wilaya);
    void RatioWilaya(Date, Date, string, wilaya);
    void RatioCountry(Date, Date, wilaya);

public:
    TimeRestaurant();
    ~TimeRestaurant();
    Date getLastDate();
    void insertResTree(ResDataBST &); // to insert the first tree in the array of months

    // getters month/period of time
    void getSales();
    void getCost();
    void getPublicity();
    void getCuisineRating();

    // daily getters
    void getDailySales();
    void getDailyCost();
    void getDailyPublicity();

    // functional getters

    // setters
    void setTodaySale();
    void setTodayCost();
    void setTodayPublicity();
    void setTodayRating();

    // functional
    void modify();
    bool DateCheck(Date d1, Date d2);
    node *getArray();

    // place time
    void districtSales(wilaya);
    void citySales(wilaya);
    void wilayaSales(wilaya);
    void countrySales(wilaya);
    void getRatio(wilaya); // time and place, calculate the ratio (monthly sales/ moneyPub)

    // file handling
    // insertion

    void writeAllDataToFile();
    void writeCuisineToFile(ofstream &outFile, BinaryNode *node, const string &cuisine, int index);
    void writeCostToFile(ofstream &outFile, BinaryNode *node, int index);
    void writePublicityToFile(ofstream &outFile, BinaryNode *node, int index);

    // constructiong (reading from files)
    void fillFromCuisineFiles();
    void fillFromCostFile();
    void fillFromPublicityFile();

    // make the files empty
    void makeAllEmpty();
    void makeCuisineEmpty();
    void makeCostEmpty();
    void makePublicityEmpty();
};

#endif