#pragma once
#include<iostream>
#include"ResDataAVL.h"
#include "TimeRestaurantAVL.h"
#include<vector>

using namespace std;
class Prize
{
private:
    ResDataAVL* restautree;
    string checkAlgerian()const;
    // cal the FindWinner function for Algerian cuisine since  GetArr(1) returns Algerian



// check in all the tree for the bigger sales+rate* and returns the name of restaurant in that cuisine
    string checkSyrian()const;
    // cal the FindWinner function for Syrian cuisine since  GetArr(2) returns Syrian


    string checkChinese() const;
    // cal the FindWinner function for Chinese cuisine since  GetArr(3) returns Chinese


    string checkIndian()const;
    // cal the FindWinner function for Indian cuisine since  GetArr(4) returns Indian


    string checkEuropean()const;
    // cal the FindWinner function for European cuisine since  GetArr(1) returns European


public:
    Prize() :restautree(nullptr) {};
    Prize(ResDataAVL* tree) : restautree(tree) {};
    ~Prize() {};

    vector<string> getWinner()const;

};




