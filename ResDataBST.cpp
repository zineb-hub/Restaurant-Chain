#include "ResDataBST.h"
#include "Date.h"
#include <iostream>
using namespace std;

void ResDataBST::make_Empty(BinaryNode *&t)
{
    if (t == nullptr)
        return;

    make_Empty(t->left);
    make_Empty(t->right);

    delete t;
    t = nullptr; // Fix: Set the pointer to nullptr after deletion
}

bool ResDataBST::insert_Res(const ResData &elem, BinaryNode *&t)
{ // insert a restaurant
    if (t == nullptr)
    {
        t = new BinaryNode(elem, nullptr, nullptr);
        return 1;
    }
    else if (IDCalculator(elem.NameR) < IDCalculator(t->element.NameR))
        insert_Res(elem, t->left);
    else if (IDCalculator(elem.NameR) > IDCalculator(t->element.NameR))
        insert_Res(elem, t->right);
    else
        return 0;
}
bool ResDataBST::remove_Res(const ResData &elem, BinaryNode *&t)
{ // insert a restaurant
    if (t == nullptr)
        return 0;
    else if (IDCalculator(elem.NameR) < IDCalculator(t->element.NameR))
        remove_Res(elem, t->left);
    else if (IDCalculator(elem.NameR) > IDCalculator(t->element.NameR))
        remove_Res(elem, t->right);
    else if (t->left != nullptr && t->right != nullptr)
    {
        t->element = FindMin(t->right)->element;
        remove_Res(t->element, t->right);
    }
    else
    {
        BinaryNode *old = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete old;
    }
}

BinaryNode *ResDataBST::FindMin(BinaryNode *&t) const
{ // finds the min by using the ID
    if (t == nullptr)
        return nullptr;
    if (t->left == nullptr)
        return t;

    return FindMin(t->left);
}

BinaryNode *ResDataBST::cloneAll(BinaryNode *t) const
{ // copy a all the tree via the root
    if (t == nullptr)
        return nullptr;
    else
        return new BinaryNode{t->element, clone(t->left), clone(t->right)};
}

bool ResDataBST::contains_Res(string name, BinaryNode *&t)
{
    if (t == nullptr)
        return false;

    if (IDCalculator(name) < IDCalculator(t->element.NameR))
        return contains_Res(name, t->left);

    else if (IDCalculator(name) > IDCalculator(t->element.NameR))
        return contains_Res(name, t->right);

    else
        return true;
}

BinaryNode *ResDataBST::search_Res(string name, BinaryNode *&t)
{ // searches for a restaurant and return the pointer to the node
    if (t == nullptr)
        return nullptr;

    if (IDCalculator(name) < IDCalculator(t->element.NameR))
        return search_Res(name, t->left);

    else if (IDCalculator(name) > IDCalculator(t->element.NameR))
        return search_Res(name, t->right);

    else
        return t;
}

BinaryNode *ResDataBST::clone(BinaryNode *node) const
{

    if (node == nullptr)
    {
        return nullptr;
    }
    else
    {
        BinaryNode *newNode = new BinaryNode(node->element);
        newNode->left = clone(node->left);
        newNode->right = clone(node->right);
        return newNode;
    }
}

const ResData ResDataBST::find_winner(int ArrNum, BinaryNode *&t)
{ // finds the winner restaurant and returns its data
    ResData winner;

    if (t == nullptr || ArrNum > 4 || ArrNum < 0)
        return ResData();

    double tamount = t->element.calcPrizeCriterion(t->element.GetArr(ArrNum));
    ResData lwinner = find_winner(ArrNum, t->left);
    ResData rwinner = find_winner(ArrNum, t->right);
    double leftamount = t->left->element.calcPrizeCriterion(t->left->element.GetArr(ArrNum));
    double rightamount = t->right->element.calcPrizeCriterion(t->right->element.GetArr(ArrNum));
    if (tamount > leftamount && tamount > rightamount)
    {
        winner = t->element;
    }
    else if (leftamount > rightamount)
    {
        winner = lwinner;
    }
    else if (leftamount < rightamount)
    {
        winner = rwinner;
    }
    return winner;
}

void ResDataBST::printNodeDataS(BinaryNode *&t)
{

    cout << "\t\tRestaurant Name: " << t->element.NameR << endl
         << endl;
    for (int day = 0; day < 32; day++)
    {
        if (day != 31 && t->element.Algerian[day].sales != -1)
        {
            cout << "Day " << day + 1 << " - " << endl;
            cout << "Algerian cuisine: Sold $" << t->element.Algerian[day].sales << endl;
            cout << "Syrian cuisine: Sold $" << t->element.Syrian[day].sales << endl;
            cout << "Chinese cuisine: Sold $" << t->element.Chinese[day].sales << endl;
            cout << "Indian cuisine: Sold $" << t->element.Indian[day].sales << endl;
            cout << "European cuisine: Sold $" << t->element.European[day].sales << endl;
            cout << endl;
        }
        else if (t->element.Algerian[day].sales != -1)
        {
            cout << "\t\tTotal:\n";
            cout << "Algerian cuisine: Sold a Total of$" << t->element.Algerian[day].sales << endl;
            cout << "Syrian cuisine: Sold a Total of $" << t->element.Syrian[day].sales << endl;
            cout << "Chinese cuisine: Sold a Total of $" << t->element.Chinese[day].sales << endl;
            cout << "Indian cuisine: Sold a Total of $" << t->element.Indian[day].sales << endl;
            cout << "European cuisine: Sold a Total of $" << t->element.European[day].sales << endl;
            cout << endl;
        }
    }
}

void ResDataBST::printNodeData(BinaryNode *t)
{
    if (t != nullptr)
    {
        printNodeData(t->left);
        printNodeDataS(t);
        printNodeData(t->right);
    }
}

ResDataBST::ResDataBST() : root(nullptr) {}

ResDataBST::ResDataBST(const ResDataBST &NewBST) // copy constructor
{
    root = clone(NewBST.root);
}

ResDataBST &ResDataBST::operator=(const ResDataBST &NewBST)
{ // copy assignment(only copies the last 31 of the arrays

    if (this != &NewBST)
    {
        MakeEmpty();
        if (NewBST.root != nullptr)
        {
            root = cloneAll(NewBST.root);
        }
    }
    return *this;
}

ResDataBST::~ResDataBST()
{
    MakeEmpty();
}

void ResDataBST::MakeEmpty()
{
    make_Empty(root);
}

bool ResDataBST::isEmpty() const
{
    return root == nullptr;
}

void ResDataBST::makeACopy(const ResDataBST &restau)
{
    root = cloneAll(restau.root);
}

bool ResDataBST::InsertRes(const ResData &x)
{
    insert_Res(x, root);
}

bool ResDataBST::RemoveRes(const ResData &x)
{
    remove_Res(x, root);
}

bool ResDataBST::ContainsRes(const string name)
{
    return contains_Res(name, root);
}

ResData &ResDataBST::search(const string name)
{
    if (isEmpty())
        cout << "No Data yet." << endl;
    else if ((search_Res(name, root)) != nullptr)
        return (search_Res(name, root))->element;
    else
        cout << "Element Not Found" << endl;
}

const ResData ResDataBST::FindWinner(int arrNum)
{ // index of arr of cuisine
    return find_winner(arrNum, root);
}

void ResDataBST::PrintAll()
{ // prints all of the tree
    if (isEmpty())
        cout << "No Data yet." << endl;
    else
        printNodeData(root);
}

int ResDataBST::IDCalculator(string name)
{ // function that calculate the id according to the ASCII
    int id = 0;
    for (char c : name)
    {
        id += (int)c;
        id *= 10;
    }
    return id;
}

BinaryNode *ResDataBST::getRoot()
{
    return root;
}
