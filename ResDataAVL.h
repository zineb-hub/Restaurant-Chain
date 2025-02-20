#include "ResData.h"
struct BinaryNode{
    ResData element;
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode(const ResData& Element, BinaryNode* lt = nullptr, BinaryNode* rt = nullptr) : element(Element), left(lt), right(rt) {}
};

class ResDataAVL
{
private:
    BinaryNode* root;
    ResData winner;

    int max(int, int);
    int height(BinaryNode*& r);
    void rotateRight(BinaryNode*&);
    void rotateLeft(BinaryNode*&);
    void rotateLeftRight(BinaryNode*&);
    void rotateRightLeft(BinaryNode*&);
    void make_Empty(BinaryNode*&);
    bool insert_Res(const ResData&, BinaryNode*&);
    bool remove_Res(const ResData&, BinaryNode*&);
    BinaryNode* search_Res(string, BinaryNode*&);
    BinaryNode* clone(BinaryNode*) const;
    BinaryNode* FindMin(BinaryNode*&);
    const ResData find_winner(int, BinaryNode*&);
    void printNodeDataS(BinaryNode*& t);
    void printNodeData(BinaryNode* t);
    bool contains_Res(string name, BinaryNode*& t);
    BinaryNode* cloneAll(BinaryNode* t) const;
public:
    ResDataAVL();
    ResDataAVL(const ResDataAVL&);
    ~ResDataAVL();
    void makeACopy(const ResDataAVL& restau);
    void MakeEmpty();
    bool InsertRes(const ResData&);
    bool RemoveRes(const ResData&);
    ResData& search(const string);
    ResData FindWinner(int);
    int IDCalculator(string name);
    void PrintAll();
    bool ContainsRes(const string);
    BinaryNode* getRoot();
};
