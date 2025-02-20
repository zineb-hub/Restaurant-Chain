#ifndef RESDATABST_H
#define RESDATABST_H

#pragma once
#include "ResData.h"
#include "Date.h"
struct BinaryNode
{
    ResData element;
    BinaryNode *left;
    BinaryNode *right;
    BinaryNode(const ResData &Element, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr) : element(Element), left(lt), right(rt) {}
};
class ResDataBST
{

private:
    BinaryNode *root;
    ResData winner;

    void make_Empty(BinaryNode *&t);
    bool insert_Res(const ResData &elem, BinaryNode *&t);
    bool remove_Res(const ResData &elem, BinaryNode *&t);
    BinaryNode *FindMin(BinaryNode *&t) const;
    bool contains_Res(string name, BinaryNode *&t);
    BinaryNode *search_Res(string name, BinaryNode *&t);
    BinaryNode *clone(BinaryNode *t) const;
    const ResData find_winner(int ArrNum, BinaryNode *&t);
    BinaryNode *cloneAll(BinaryNode *t) const;
    void printNodeDataS(BinaryNode *&t);
    void printNodeData(BinaryNode *t);

public:
    ResDataBST();
    ResDataBST(const ResDataBST &NewBST);
    ResDataBST &operator=(const ResDataBST &NewBST);
    ~ResDataBST();
    bool isEmpty() const;
    void makeACopy(const ResDataBST &restau);
    void MakeEmpty();
    bool InsertRes(const ResData &x);
    bool RemoveRes(const ResData &x);
    bool ContainsRes(const string);
    ResData &search(const string name);
    const ResData FindWinner(int arrNum);
    int IDCalculator(string Name);
    void PrintAll();
    BinaryNode *getRoot();    
};
#endif
