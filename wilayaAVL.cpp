#include "wilayaAVL.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
int MAX(int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}
static int IDCalculator(std::string name) {
    int sum = 0;
    for (char c : name) {
        sum += static_cast<int>(toupper(c));
        sum *= 10;
    }
    return sum;
}
wilayaAVL::RestauNode::RestauNode(string name) : nameRest(name), next(nullptr) {
    IDRest = IDCalculator(name);
}

wilayaAVL::arrayCaseRList::arrayCaseRList(string rName, RestauNode* pToLinkedList) :regionName(rName), pointerToLinkedList(pToLinkedList)
{
}

wilayaAVL::arrayCase1To2::arrayCase1To2(string rName, vector<arrayCaseRList>* pToVector) :regionName(rName), pointerToVector(pToVector)
{
}

wilayaAVL::arrayCase::arrayCase(string rName, vector<arrayCase1To2>* pToVector) :regionName(rName), pointerToVector(pToVector)
{
}

// Constructor definition for BSNode
wilayaAVL::AVLNode::AVLNode(string name) : elementName(name), left(nullptr), right(nullptr), nextTree(nullptr), restaulist(nullptr) {
    elementID = IDCalculator(name);
}

wilayaAVL::wilayaAVL() : root(nullptr) {
    fillFromFile();
}


void wilayaAVL::setId(AVLNode* node, string name)
{
    node->elementID = IDCalculator(name);
}

int wilayaAVL::getId(AVLNode* node) const
{
    return node->elementID;
}

void wilayaAVL::setName(AVLNode* node, string name)
{
    root->elementName = name;
}

string wilayaAVL::getName(AVLNode* node) const
{
    return node->elementName;
}



wilayaAVL::AVLNode** wilayaAVL::getWilayaOrCreat(string& wilayaName)
{
    int wilayaID = IDCalculator(wilayaName);
    if (root == nullptr) {
        root = new AVLNode(wilayaName);
        return &root;
    }
    AVLNode** current = &root;
    // Search for the wilaya
    while (*current != nullptr)
    {
        if (wilayaID < (*current)->elementID)
        {
            current = &((*current)->left);
        }
        else if (wilayaID > (*current)->elementID)
        {
            current = &((*current)->right);
        }
        else
        {
            return current;
        }
    }

    *current = new AVLNode(wilayaName);
    balance(*current);
    return current;
}

wilayaAVL::AVLNode** wilayaAVL::getCityOrDistrictOrCreat(AVLNode** wilayaOrCityNode, string& cityOrDistrictName)
{
    int cityOrDistrictID = IDCalculator(cityOrDistrictName);

    if (!((*wilayaOrCityNode)->nextTree))
    {
        (*wilayaOrCityNode)->nextTree = new AVLNode(cityOrDistrictName);
        return &((*wilayaOrCityNode)->nextTree);
    }

    AVLNode** cityOrDistrict = &((*wilayaOrCityNode)->nextTree);
    // Search for the city
    while (*cityOrDistrict != nullptr)
    {
        if (cityOrDistrictID < (*cityOrDistrict)->elementID)
        {
            cityOrDistrict = &((*cityOrDistrict)->left);
        }
        else if (cityOrDistrictID > (*cityOrDistrict)->elementID)
        {
            cityOrDistrict = &((*cityOrDistrict)->right);
        }
        else
        {
            return cityOrDistrict;
        }
    }

    *cityOrDistrict = new AVLNode(cityOrDistrictName);
    balance(*cityOrDistrict);
    return cityOrDistrict;
}

bool wilayaAVL::addRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName)
{
    AVLNode** wil = getWilayaOrCreat(wilayaName);
    AVLNode** city = getCityOrDistrictOrCreat(wil, cityName);
    AVLNode** district = getCityOrDistrictOrCreat(city, districtName);

    RestauNode** restaurant = &((*district)->restaulist);

    if (!(*restaurant))
    {
        *restaurant = new RestauNode(restaurantName);
        return true;
    }

    // Check for the existence of the restaurant
    RestauNode* temp = *restaurant;
    while (temp)
    {
        if (temp->nameRest == restaurantName)
        {
            return false;
        }
        // Check if this is the last node in the list
        if (!temp->next)
        {
            // Insert the restaurant at the end
            temp->next = new RestauNode(restaurantName);
            return true;
        }
        temp = temp->next;
    }

    // This should not be reached, but adding a return statement to avoid warnings
    return false;
}



wilayaAVL::AVLNode** wilayaAVL::getWilaya(string& wilayaName)
{
    int wilayaID = IDCalculator(wilayaName);
    if (root == nullptr) {
        return nullptr;
    }
    AVLNode** current = &root;
    // Search for the wilaya
    while (*current != nullptr)
    {
        if (wilayaID < (*current)->elementID)
        {
            current = &((*current)->left);
        }
        else if (wilayaID > (*current)->elementID)
        {
            current = &((*current)->right);
        }
        else
        {
            return current;
        }
    }

    return nullptr;
}

wilayaAVL::AVLNode** wilayaAVL::getCityOrDistrict(AVLNode** wilayaOrCityNode, string& cityOrDistrictName)
{
    int cityOrDistricID = IDCalculator(cityOrDistrictName);

    if (!((*wilayaOrCityNode)->nextTree))
    {
        return nullptr;
    }

    AVLNode** cityOrDistrict = &((*wilayaOrCityNode)->nextTree);
    // Search for the city
    while (*cityOrDistrict != nullptr)
    {
        if (cityOrDistricID < (*cityOrDistrict)->elementID)
        {
            cityOrDistrict = &((*cityOrDistrict)->left);
        }
        else if (cityOrDistricID > (*cityOrDistrict)->elementID)
        {
            cityOrDistrict = &((*cityOrDistrict)->right);
        }
        else
        {
            return cityOrDistrict;
        }
    }
    return nullptr;
}

bool wilayaAVL::deleteRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName)
{
    AVLNode** wil = getWilaya(wilayaName);
    if (!(*wil))
    {
        return false;
    }

    AVLNode** city = getCityOrDistrict(wil, cityName);
    if (!(*city))
    {
        return false;
    }

    AVLNode** district = getCityOrDistrict(city, districtName);
    if (!(*district))
    {
        return false;
    }

    // Found the district
    RestauNode** temRestaurant = &(*district)->restaulist;

    while (*temRestaurant != nullptr)
    {
        if ((*temRestaurant)->nameRest == restaurantName)
        {
            // Found the restaurant to delete
            RestauNode* deleteNode = *temRestaurant;
            *temRestaurant = (*temRestaurant)->next;
            delete deleteNode;
            return true;
        }
        temRestaurant = &(*temRestaurant)->next;
    }
    // Restaurant not found in this district
    return false;
}

bool wilayaAVL::doesExistCity(string& wilayaName, string& cityName)
{
    if (!(doesExistWilaya(wilayaName)))//in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity 
    {
        return false;
    }

    AVLNode** wil = getWilaya(wilayaName);

    int cityID = IDCalculator(cityName);

    if (!((*wil)->nextTree))
    {
        return false;
    }

    AVLNode** city = &((*wil)->nextTree);
    // Search for the city
    while (*city != nullptr)
    {
        if (cityID < (*city)->elementID)
        {
            city = &((*city)->left);
        }
        else if (cityID > (*city)->elementID)
        {
            city = &((*city)->right);
        }
        else
        {
            return true;
        }
    }
    return false;
}

bool wilayaAVL::doesExistDistrict(string& wilayaName, string& cityName, string& districtName)
{
    if (!(doesExistWilaya(wilayaName)))//in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity 
    {
        return false;
    }
    AVLNode** wil = getWilaya(wilayaName);
    if (!doesExistCity(wilayaName, cityName))
    {
        return false;
    }
   AVLNode** city = &((*wil)->nextTree);

    int DistricID = IDCalculator(districtName);

    if (!((*city)->nextTree))
    {
        return false;
    }

    AVLNode** district = getCityOrDistrict(city, districtName);
    // Search for the city
    while (*district != nullptr)
    {
        if (DistricID < (*district)->elementID)
        {
            district = &((*district)->left);
        }
        else if (DistricID > (*district)->elementID)
        {
            district = &((*district)->right);
        }
        else
        {
            return true;
        }
    }
    return false;
}

bool wilayaAVL::doesExistRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName)
{
    if (!(doesExistWilaya(wilayaName)))//in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity 
    {
        return false;
    }
    AVLNode** wil = getWilaya(wilayaName);
    if (!doesExistCity(wilayaName, cityName))
    {
        return false;
    }
    AVLNode** city = &((*wil)->nextTree);
    if (!doesExistDistrict(wilayaName, cityName, districtName))
    {
        return false;
    }
    AVLNode** district = &((*city)->nextTree);

    // Found the district
    RestauNode** temRestaurant = &(*district)->restaulist;//District found and reaching the linked list of the restaurants

    while (*temRestaurant != nullptr)//traversing the linked list
    {
        if ((*temRestaurant)->nameRest == restaurantName)
        {
            //restaurant found
            return true;
        }
        temRestaurant = &(*temRestaurant)->next;
    }
    // Restaurant not found in this district
    return false;
}



bool wilayaAVL::doesExistWilaya(string& wilayaName)
{
    int wilayaID = IDCalculator(wilayaName);
    if (root == nullptr) {//there is no tree for Wilayas
        return false;
    }
    AVLNode** current = &root;
    // Search for the wilaya
    while (*current != nullptr)
    {
        if (wilayaID < (*current)->elementID)
        {
            current = &((*current)->left);
        }
        else if (wilayaID > (*current)->elementID)
        {
            current = &((*current)->right);
        }
        else
        {
            return true;//wilaya found and returned
        }
    }

    return false;//in case the wilaya 	if (!(*wil))//in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity 

}

void wilayaAVL::setCountryName(const string& name)
{
    countryName = name;
}



vector<wilayaAVL::arrayCase> wilayaAVL::getCountryRestaurants()
{
    vector<wilayaAVL::arrayCase> list;
    return linkRestaurantsInCountry(root, list);
}

vector<wilayaAVL::arrayCase> wilayaAVL::linkRestaurantsInCountry(AVLNode* wilayaNode, vector<arrayCase>& currentList) {
    if (wilayaNode != nullptr) {
        // Link restaurants in the left subtree (prioritize left-to-right order)
        linkRestaurantsInCountry(wilayaNode->left, currentList);

        vector<wilayaAVL::arrayCase1To2>* cityNames = new vector<wilayaAVL::arrayCase1To2>;
        *cityNames = linkRestaurantsInWilaya(wilayaNode->nextTree, *cityNames);
        currentList.push_back({ wilayaNode->elementName, cityNames });

        // Link restaurants in the right subtree
        linkRestaurantsInCountry(wilayaNode->right, currentList);
    }

    return currentList;
}

vector<wilayaAVL::arrayCase1To2> wilayaAVL::getWilayaRestaurants(string& name)
{
    AVLNode** wil = getWilaya(name);
    vector<wilayaAVL::arrayCase1To2> cityNames;

    return linkRestaurantsInWilaya(*wil, cityNames);
}

vector<wilayaAVL::arrayCase1To2> wilayaAVL::linkRestaurantsInWilaya(AVLNode* cityNode, vector<arrayCase1To2>& cityNames) {
    if (cityNode != nullptr) {
        // Link restaurants in the left subtree (prioritize left-to-right order)
        linkRestaurantsInWilaya(cityNode->left, cityNames);

        // Create a new vector dynamically for each city
        vector<wilayaAVL::arrayCaseRList>* districtNames = new vector<wilayaAVL::arrayCaseRList>;
        *districtNames = linkRestaurantsInCity(cityNode->nextTree, *districtNames);
        cityNames.push_back({ cityNode->elementName, districtNames });

        // Link restaurants in the right subtree
        linkRestaurantsInWilaya(cityNode->right, cityNames);
    }

    return cityNames;
}

vector<wilayaAVL::arrayCaseRList> wilayaAVL::getCityRestaurants(string& wilayaName, string& cityName) {
    AVLNode** wil = getWilaya(wilayaName);
    AVLNode** city = getCityOrDistrict(wil, cityName);
    vector<wilayaAVL::arrayCaseRList> districtNames;
    return linkRestaurantsInCity((*city)->nextTree, districtNames);
}

vector<wilayaAVL::arrayCaseRList> wilayaAVL::linkRestaurantsInCity(AVLNode* districtNode, vector<arrayCaseRList>& districtNames) {
    if (districtNode != nullptr) {
        // Link restaurants in the left subtree (prioritize left-to-right order)
        linkRestaurantsInCity(districtNode->left, districtNames);

        // Correctly initialize arrayCaseRList with a pointer to the list of restaurants
        districtNames.push_back(arrayCaseRList(districtNode->elementName, districtNode->restaulist));


        // Link restaurants in the right subtree
        linkRestaurantsInCity(districtNode->right, districtNames);
    }
    return districtNames;
}

wilayaAVL::RestauNode* wilayaAVL::getDistrictRestaurants(string& wilayaName, string& cityName, string& districtName)
{
    AVLNode** wil = getWilaya(wilayaName);
    if (!(*wil))
    {
        return nullptr;
    }

    AVLNode** city = getCityOrDistrict(wil, cityName);
    if (!(*city))
    {
        return nullptr;
    }

    AVLNode** district = getCityOrDistrict(city, districtName);
    if (!(*district))
    {
        return nullptr;
    }
    return (*district)->restaulist;
}



void wilayaAVL::fillFromFile()
{
    // Open the file in input mode
    std::ifstream inFile("restaurantPlaces.txt", std::ios::in);

    // Check if the file is opened successfully
    if (!inFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }
    int i = 0;
    std::string line;
    while (std::getline(inFile, line)) {
        i++;
        // Use stringstream to parse the line
        std::stringstream ss(line);
        std::string wilayaName;
        std::string cityName;
        std::string districtName;
        std::string restaurantName;
        // Read the values separated by a comma
        std::getline(ss, wilayaName, ',');
        std::getline(ss, cityName, ',');
        std::getline(ss, districtName, ',');
        std::getline(ss, restaurantName, '.');

        addRestaurant(wilayaName, cityName, districtName, restaurantName);
    }

    // Close the file
    inFile.close();
}



void wilayaAVL::makeFileEmpty(const std::string& filename) {//not working here but working in the main when implemented there
    // Open the file in truncation mode to clear its content or create it if it doesn't exist
    std::ofstream file(filename, std::ofstream::trunc);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    else {
        std::cout << "File " << filename << " created and cleared." << std::endl;
        file.close();
    }
}

void wilayaAVL::traverseWilaya(AVLNode* node)
{
    if (node) {
        traverseWilaya(node->left);

        traverseCity(node->nextTree, node->elementName);

        traverseWilaya(node->right);
    }
}



void wilayaAVL::traverseCity(AVLNode* node, string& wilayaName)
{
    if (node) {
        traverseCity(node->left, wilayaName);

        traverseDistrict(node->nextTree, wilayaName, node->elementName);

        traverseCity(node->right, wilayaName);
    }
}

void wilayaAVL::traverseDistrict(AVLNode* node, string& wilayaName, string& cityName)
{
    if (node) {
        traverseDistrict(node->left, wilayaName, cityName);

        insertIntoFile(node->restaulist, wilayaName, cityName, node->elementName);

        traverseDistrict(node->right, wilayaName, cityName);
    }
}

void wilayaAVL::insertIntoFile(RestauNode* node, string& wilayaName, string& cityName, string& districtName)
{
    std::ofstream outFile("restaurantPlaces.txt", std::ios::out | std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }
    while (node != nullptr)
    {
        outFile << wilayaName << "," << cityName << "," << districtName << "," << node->nameRest << "." << std::endl;
        node = node->next;
    }
    outFile.close();
}

void wilayaAVL::fillFinalFile()
{
    std::string filename = "restaurantPlaces.txt";
    makeFileEmpty(filename);
    traverseWilaya(root);
}

void wilayaAVL::makeEmpty()
{
    makeEmpty(root);
}

wilayaAVL::~wilayaAVL()
{
    fillFinalFile();
}

string wilayaAVL::getCountryName() const
{
    return countryName;
}

int wilayaAVL::calheight(AVLNode*& t)
{
    if (t == nullptr) return -1;
    return t->height;
}

void wilayaAVL::makeEmpty(AVLNode*& t)
{
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

void wilayaAVL::balance(AVLNode*& t)
{
    if (t == nullptr) return;
    if (calheight(t->left) - calheight(t->right) > 1) {
        if (calheight(t->left->left) > calheight(t->left->right)) {
            rotateWithLeftChild(t);
        }
        else {
            doubleWithLeftChild(t);
        }
    }
    else if (calheight(t->right) - calheight(t->left) > 1) {
        if (calheight(t->right->right) > calheight(t->right->left)) {
            rotateWithRightChild(t);
        }
        else {
            doubleWithRightChild(t);
        }
    }
    t->height = max(calheight(t->left), calheight(t->right)) + 1;
}

wilayaAVL::AVLNode* wilayaAVL::insert(AVLNode* node, string name)
{
    int id = IDCalculator(name);
    if (node == nullptr) {
        node = new AVLNode(name);
    }
    else if (id < node->elementID) {
        insert(node->left, name);
    }
    else if (id > node->elementID) {
        insert(node->right, name);
    }
    else {
        throw runtime_error("The restaurant exists already");
    }
    balance(node);
    return node;
}

void wilayaAVL::rotateWithLeftChild(AVLNode*& c)
{
    AVLNode* p = c->left;
    c->left = p->right;
    p->right = c;
    c->height = max(calheight(c->left), calheight(c->right)) + 1;
    p->height = max(calheight(p->left), calheight(p->right)) + 1;
    c = p;
}

void wilayaAVL::doubleWithLeftChild(AVLNode*& t)
{
    rotateWithRightChild(t->left);
    rotateWithLeftChild(t);
}

void wilayaAVL::rotateWithRightChild(AVLNode*& p)
{
    AVLNode* c = p->right;
    p->right = c->left;
    c->left = p;
    p->height = max(calheight(p->left), calheight(p->right)) + 1;
    c->height = max(calheight(c->right), calheight(c->left)) + 1;
    p = c;
}

void wilayaAVL::doubleWithRightChild(AVLNode*& t)
{
    rotateWithLeftChild(t->right);
    rotateWithRightChild(t);
}

wilayaAVL::AVLNode* wilayaAVL::findMin(AVLNode* t)
{
    if (t == nullptr)
        return nullptr;

    if (t->left == nullptr)
        return t;

    return findMin(t->left);
}
wilayaAVL::AVLNode* wilayaAVL::remove(int id, AVLNode*& t)
{
    if (t == nullptr)
        return nullptr;
    if (id < t->elementID)
        remove(id, t->left);
    else if (t->elementID < id)
        remove(id, t->right);

    else if (t->left != nullptr && t->right != nullptr) // Two children
    {
        t->elementID = findMin(t->right)->elementID;
        remove(t->elementID, t->right);
    }
    else
    {
        AVLNode* old = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete old;
    }
    balance(t);
    return t;
}
