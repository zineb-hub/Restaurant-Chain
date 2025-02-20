#include "ResDataAVL.h"
#include "ResData.h"

int ResDataAVL::max(int l1, int l2)
{
    if (l1 > l2)
        return l1;
    else
        return l2;
}

int ResDataAVL::height(BinaryNode*& r)
{
    if (r == nullptr)
        return -1;
    else if (r->left == nullptr && r->right == nullptr)
        return 0;
    else
        return max(height(r->left), height(r->right)) + 1;
}

void ResDataAVL::rotateRight(BinaryNode*& r)
{
    if (r == nullptr)
        return;
    else
    {
        BinaryNode* leftChild = r->left;
        BinaryNode* rightSubtree = leftChild->right;

        leftChild->right = r;
        r->left = rightSubtree;
        r = leftChild; // update the root
    }
}
void ResDataAVL::rotateLeft(BinaryNode*& r)
{
    if (r == nullptr)
        return;
    else
    {
        BinaryNode* rightChild = r->right;
        BinaryNode* leftSubtree = rightChild->left;

        rightChild->left = r;
        r->right = leftSubtree;
        r = rightChild;
    }
}
void ResDataAVL::rotateLeftRight(BinaryNode*& r)
{
    rotateLeft(r);
    rotateRight(r);
}
void ResDataAVL::rotateRightLeft(BinaryNode*& r)
{
    rotateRight(r);
    rotateLeft(r);
}

void ResDataAVL::make_Empty(BinaryNode*& t)
{
    if (t == nullptr)
        return;
    make_Empty(t->left);
    make_Empty(t->right);
    delete t;
}
bool ResDataAVL::insert_Res(const ResData& elem, BinaryNode*& t)
{ // insert a restaurant
    if (t == nullptr)
    {
        t = new BinaryNode(elem, nullptr, nullptr);
        return true; // since it's the root no need to rotate, it's balanced
    }

    else if (IDCalculator(elem.NameR) < IDCalculator(t->element.NameR))
    {
        if (!insert_Res(elem, t->left))
            return false;
    }

    else if (IDCalculator(elem.NameR) > IDCalculator(t->element.NameR))
    {
        if (!insert_Res(elem, t->right))
            return false;
    } // return false if it didn't insert

    // checking the balance
    int balanceFact = height(t->left) - height(t->right);
    if (balanceFact > 1 && elem.NameR < t->left->element.NameR) // left left child
        rotateRight(t);
    else if (balanceFact < -1 && elem.NameR > t->right->element.NameR) // right right child
        rotateLeft(t);
    else if (balanceFact > 1 && elem.NameR > t->left->element.NameR) // left right child
        rotateRightLeft(t);
    else if (balanceFact < -1 && elem.NameR < t->right->element.NameR) // right left child
        rotateLeftRight(t);
    return true;
}

bool ResDataAVL::remove_Res(const ResData& elem, BinaryNode*& t)
{ // insert a restaurant
    if (t == nullptr)
        return 0;
    else if (IDCalculator(elem.NameR) < IDCalculator(t->element.NameR))
        if (!remove_Res(elem, t->left))
            return false;
        else if (IDCalculator(elem.NameR) > IDCalculator(t->element.NameR))
            remove_Res(elem, t->right);
        else if (t->left != nullptr && t->right != nullptr)
        {
            t->element = FindMin(t->right)->element;
            remove_Res(t->element, t->right);
        }
        else
        {
            BinaryNode* old = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete old;
        }
    // checking the balance
    int balanceFact = height(t->left) - height(t->right);
    if (balanceFact > 1 && elem.NameR < t->left->element.NameR) // left left child
        rotateRight(t);
    else if (balanceFact < -1 && elem.NameR > t->right->element.NameR) // right right child
        rotateLeft(t);
    else if (balanceFact > 1 && elem.NameR > t->left->element.NameR) // left right child
        rotateRightLeft(t);
    else if (balanceFact < -1 && elem.NameR < t->right->element.NameR) // right left child
        rotateLeftRight(t);
    return true;
}
BinaryNode* ResDataAVL::FindMin(BinaryNode*& t)
{ // finds the min by using the ID
    if (t == nullptr)
        return nullptr;
    if (t->left == nullptr)
        return t;

    return FindMin(t->left);
}

BinaryNode* ResDataAVL::search_Res(string name, BinaryNode*& t)
{ // searches for a restaurant and return the pointer to the node
    if (root == nullptr)
        return nullptr;

    if (IDCalculator(name) < IDCalculator(t->element.NameR))
        return search_Res(name, t->left);

    else if (IDCalculator(name) > IDCalculator(t->element.NameR))
        return search_Res(name, t->right);

    else
        return t;
}
BinaryNode* ResDataAVL::clone(BinaryNode* t) const
{ // copy a tree via the root
    if (t == nullptr)
        return nullptr;
    else
        return new BinaryNode{ t->element.copy_obj(), clone(t->left), clone(t->right) };
}

const ResData ResDataAVL::find_winner(int ArrNum, BinaryNode*& t)
{ // finds the winner restaurant and returns its data
    if (t == nullptr)
        return ResData();
    if (t->element.calcPrizeCriterion(t->element.GetArr(ArrNum)) > winner.calcPrizeCriterion(winner.GetArr(ArrNum)))
    {
        winner = t->element;
    }

    // not complete

    return winner;
}

ResDataAVL::ResDataAVL() : root(nullptr) {}

ResDataAVL::ResDataAVL(const ResDataAVL& NewBST) : root{ nullptr }
{
    root = clone(NewBST.root);
}
ResDataAVL::~ResDataAVL()
{
    MakeEmpty();
}

void ResDataAVL::makeACopy(const ResDataAVL& restau)
{
    if (restau.root != nullptr)
    {
        root = cloneAll(restau.root);
    }
}

void ResDataAVL::MakeEmpty()
{
    make_Empty(root);
}

bool ResDataAVL::InsertRes(const ResData& x)
{
    insert_Res(x, root);
    return true;
}

bool ResDataAVL::RemoveRes(const ResData& x)
{
    remove_Res(x, root);
    return true;
}

ResData& ResDataAVL::search(const string name)
{
    return (search_Res(name, root))->element;
}

ResData ResDataAVL::FindWinner(int arrNum)
{ // index of arr of cuisine
    return find_winner(arrNum, root);
}
int ResDataAVL::IDCalculator(string name)
{ // function that calculate the id according to the ASCII
    int id = 0;
    for (char c : name)
    {
        id += (int)c;
        id *= 10;
    }
    return id;
}

void ResDataAVL::printNodeDataS(BinaryNode*& t)
{

    cout << "\t-Restaurant Name: " << t->element.NameR << endl
        << endl;
    for (int day = 0; day < 32; day++)
    {
        if (day != 31 && (t->element.Algerian[day].sales != -1 || t->element.Syrian[day].sales != -1 || t->element.Chinese[day].sales != -1 || t->element.Indian[day].sales != -1 || t->element.European[day].sales != -1))
        {
            cout << "Day " << day + 1 << " - " << endl;
            if (t->element.Algerian[day].sales != -1)
                cout << "Algerian cuisine: Sold $" << t->element.Algerian[day].sales << endl;
            if (t->element.Syrian[day].sales != -1)
                cout << "Syrian cuisine: Sold $" << t->element.Syrian[day].sales << endl;
            if (t->element.Chinese[day].sales != -1)
                cout << "Chinese cuisine: Sold $" << t->element.Chinese[day].sales << endl;
            if (t->element.Indian[day].sales != -1)
                cout << "Indian cuisine: Sold $" << t->element.Indian[day].sales << endl;
            if (t->element.European[day].sales != -1)
                cout << "European cuisine: Sold $" << t->element.European[day].sales << endl;
            cout << endl;
        }
        else if (t->element.Algerian[day].sales != -1 || t->element.Syrian[day].sales != -1 || t->element.Chinese[day].sales != -1 || t->element.Indian[day].sales != -1 || t->element.European[day].sales != -1)
        {
            cout << "\t     Total:\n";
            if (t->element.Algerian[day].sales != -1)
                cout << "Algerian cuisine: Sold a Total of$" << t->element.Algerian[day].sales << endl;
            if (t->element.Syrian[day].sales != -1)
                cout << "Syrian cuisine: Sold a Total of $" << t->element.Syrian[day].sales << endl;
            if (t->element.Chinese[day].sales != -1)
                cout << "Chinese cuisine: Sold a Total of $" << t->element.Chinese[day].sales << endl;
            if (t->element.Indian[day].sales != -1)
                cout << "Indian cuisine: Sold a Total of $" << t->element.Indian[day].sales << endl;
            if (t->element.European[day].sales != -1)
                cout << "European cuisine: Sold a Total of $" << t->element.European[day].sales << endl;
            cout << endl;
        }
    }
}

void ResDataAVL::printNodeData(BinaryNode* t)
{
    if (t != nullptr)
    {
        printNodeData(t->left);
        printNodeDataS(t);
        printNodeData(t->right);
    }
}

bool ResDataAVL::contains_Res(string name, BinaryNode*& t)
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

BinaryNode* ResDataAVL::cloneAll(BinaryNode* t) const
{
    if (t == nullptr)
        return nullptr;
    else
        return new BinaryNode{ t->element, clone(t->left), clone(t->right) };
}

void ResDataAVL::PrintAll()
{ // prints all of the tree
    if (root == nullptr)
        cout << "No Data yet." << endl;
    else
        printNodeData(root);
}

bool ResDataAVL::ContainsRes(const string name)
{
    return contains_Res(name, root);
}

BinaryNode* ResDataAVL::getRoot()
{
    return root;
}

