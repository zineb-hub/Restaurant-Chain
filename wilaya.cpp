#include "wilaya.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

static int IDCalculator(string name)
{ // function that calculate the id according to the ASCII
    int id = 0;
    for (char c : name)
    {
        id += (int)c;
        id *= 10;
    }
    return abs(id);
}

wilaya::RestauNode::RestauNode(string name) : nameRest(name), next(nullptr)
{
    IDRest = IDCalculator(name);
}

wilaya::arrayCaseRList::arrayCaseRList(string rName, RestauNode* pToLinkedList) : regionName(rName), pointerToLinkedList(pToLinkedList)
{
}

wilaya::arrayCase1To2::arrayCase1To2(string rName, vector<arrayCaseRList>* pToVector) : regionName(rName), pointerToVector(pToVector)
{
}

wilaya::arrayCase::arrayCase(string rName, vector<arrayCase1To2>* pToVector) : regionName(rName), pointerToVector(pToVector)
{
}

// Constructor definition for BSNode
wilaya::BSNode::BSNode(string name) : elementName(name), left(nullptr), right(nullptr), nextTree(nullptr), restaulist(nullptr)
{
    elementID = IDCalculator(name);
}

wilaya::wilaya() : root(nullptr)
{
    cout << "Enter The Country Name: "; // asking for the country name in the constructor bc it should be setted once each time the program is started
    string countryName;
    getline(cin, countryName);
    setCountryName(countryName);
    fillFromFile(); // Filling all the DSA so the in case the user want to reach one of the the time complexity will not be O(N) however it will be as the DSA used
}

void wilaya::setCountryName(string& name)
{
    countryName = name;
}

string wilaya::getCountryName()
{
    return countryName;
}

void wilaya::setId(BSNode* node, string name)
{
    node->elementID = IDCalculator(name);
}

int wilaya::getId(BSNode* node) const
{
    return node->elementID;
}

void wilaya::setName(BSNode* node, string name)
{
    root->elementName = name;
}

string wilaya::getName(BSNode* node) const
{
    return node->elementName;
}

wilaya::BSNode** wilaya::getWilayaOrCreat(string& wilayaName)
{
    int wilayaID = IDCalculator(wilayaName); // Set the id for the Wilaya
    if (root == nullptr)
    { // In case it is the first Wilaya to be added
        root = new BSNode(wilayaName);
        return &root;
    }
    BSNode** current = &root; // pointer to a pointer(makes searching easier)
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
            return current; // Wilaya found and returned
        }
    }

    // Wilaya not found --> created
    *current = new BSNode(wilayaName);
    return current;
}
// The same for the city and District since the structure and the paramaters are the same(optima purposes )
wilaya::BSNode** wilaya::getCityOrDistrictOrCreat(BSNode** wilayaOrCityNode, string& cityOrDistrictName) // The same functionality as the prev function
{
    int cityOrDistrictID = IDCalculator(cityOrDistrictName);

    if (!((*wilayaOrCityNode)->nextTree))
    {
        (*wilayaOrCityNode)->nextTree = new BSNode(cityOrDistrictName);
        return &((*wilayaOrCityNode)->nextTree);
    }

    BSNode** cityOrDistrict = &((*wilayaOrCityNode)->nextTree);
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

    *cityOrDistrict = new BSNode(cityOrDistrictName);
    return cityOrDistrict;
}

bool wilaya::addRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName)
{
    BSNode** wil = getWilayaOrCreat(wilayaName);                      // The node of the Wilaya to be added for the restaurant
    BSNode** city = getCityOrDistrictOrCreat(wil, cityName);          // The node of the City to be added for the restaurant(uses the wilaya node to traverse it for time complexity purposes)
    BSNode** district = getCityOrDistrictOrCreat(city, districtName); // The node of the District to be added for the restaurant(uses the city node to traverse it for time complexity purposes)

    RestauNode** restaurant = &((*district)->restaulist); // the head of the linked list of those restaurants in that specific district

    if (!(*restaurant)) // in case no restaurants are there yet in that district, a linked list will be created
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
            return false; // restaurant already exists
        }
        // Check if this is the last node in the list
        if (!temp->next)
        {
            // Insert the restaurant at the end(found)
            temp->next = new RestauNode(restaurantName);
            return true;
        }
        temp = temp->next;
    }

    // This should not be reached, but adding a return statement to avoid warnings
    return false;
}

wilaya::BSNode** wilaya::getWilaya(string& wilayaName)
{
    int wilayaID = IDCalculator(wilayaName);

    if (root == nullptr)
    {
        return nullptr;
    }

    BSNode** current = &root;

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

wilaya::BSNode** wilaya::getCityOrDistrict(BSNode** wilayaOrCityNode, string& cityOrDistrictName)
{
    cout << "hello 2" << endl;
    int cityOrDistricID = IDCalculator(cityOrDistrictName);

    if (!((*wilayaOrCityNode)->nextTree))
    {
        return nullptr;
    }

    BSNode** cityOrDistrict = &((*wilayaOrCityNode)->nextTree);
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

bool wilaya::deleteRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName)
{
    BSNode** wil = getWilaya(wilayaName);
    if (!(*wil)) // in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity
    {
        return false;
    }

    BSNode** city = getCityOrDistrict(wil, cityName); // Wilaya found and looking for the City
    if (!(*city))                                     // in case the City does not exist the searching will stop which is an obtimizations for time complexity
    {
        return false;
    }

    BSNode** district = getCityOrDistrict(city, districtName); // City found and looking for the District
    if (!(*district))                                          // in case the District does not exist the searching will stop which is an obtimizations for time complexity
    {
        return false;
    }

    // Found the district
    RestauNode** temRestaurant = &(*district)->restaulist; // District found and reaching the linked list of the restaurants

    while (*temRestaurant != nullptr) // traversing the linked list
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

bool wilaya::doesExistWilaya(string& wilayaName)
{
    int wilayaID = IDCalculator(wilayaName);
    if (root == nullptr)
    { // there is no tree for Wilayas
        return false;
    }
    BSNode** current = &root;
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
            return true; // wilaya found and returned
        }
    }

    return false; // in case the wilaya 	if (!(*wil))//in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity
}

bool wilaya::doesExistCity(string& wilayaName, string& cityName)
{
    if (!(doesExistWilaya(wilayaName))) // in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity
    {
        return false;
    }

    BSNode** wil = getWilaya(wilayaName);

    int cityID = IDCalculator(cityName);

    if (!((*wil)->nextTree))
    {
        return false;
    }

    BSNode** city = &((*wil)->nextTree);
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

bool wilaya::doesExistDistrict(string& wilayaName, string& cityName, string& districtName)
{
    if (!(doesExistWilaya(wilayaName))) // in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity
    {
        return false;
    }
    BSNode** wil = getWilaya(wilayaName);
    if (!doesExistCity(wilayaName, cityName))
    {
        return false;
    }
    BSNode** city = &((*wil)->nextTree);

    int DistricID = IDCalculator(districtName);

    if (!((*city)->nextTree))
    {
        return false;
    }

    BSNode** district = getCityOrDistrict(city, districtName);
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

bool wilaya::doesExistRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName)
{
    if (!(doesExistWilaya(wilayaName))) // in case the wilaya does not exist the searching will stop which is an obtimizations for time complexity
    {
        return false;
    }
    BSNode** wil = getWilaya(wilayaName);
    if (!doesExistCity(wilayaName, cityName))
    {
        return false;
    }
    BSNode** city = &((*wil)->nextTree);
    if (!doesExistDistrict(wilayaName, cityName, districtName))
    {
        return false;
    }
    BSNode** district = &((*city)->nextTree);

    // Found the district
    RestauNode** temRestaurant = &(*district)->restaulist; // District found and reaching the linked list of the restaurants

    while (*temRestaurant != nullptr) // traversing the linked list
    {
        if ((*temRestaurant)->nameRest == restaurantName)
        {
            // restaurant found
            return true;
        }
        temRestaurant = &(*temRestaurant)->next;
    }
    // Restaurant not found in this district
    return false;
}

vector<wilaya::arrayCase> wilaya::getCountryRestaurants()
{
    vector<wilaya::arrayCase> list;              // creating a vector to have in each case the name of the wilaya and a pointer to the vector of its cities
    return linkRestaurantsInCountry(root, list); // passing it to the helper function
}

vector<wilaya::arrayCase> wilaya::linkRestaurantsInCountry(BSNode* wilayaNode, vector<arrayCase>& currentList)
{
    if (wilayaNode != nullptr)
    {
        // Link restaurants in the left subtree (prioritize left-to-right order)
        linkRestaurantsInCountry(wilayaNode->left, currentList);

        // Create a new vector dynamically for each city
        vector<wilaya::arrayCase1To2>* cityNames = new vector<wilaya::arrayCase1To2>;
        *cityNames = linkRestaurantsInWilaya(wilayaNode->nextTree, *cityNames);
        currentList.push_back({ wilayaNode->elementName, cityNames });

        // Link restaurants in the right subtree
        linkRestaurantsInCountry(wilayaNode->right, currentList);
    }

    return currentList;
}

vector<wilaya::arrayCase1To2> wilaya::getWilayaRestaurants(string& name)
{
    BSNode** wil = getWilaya(name);          // getting the node of the wilaya to start traversing it(a helper function)
    vector<wilaya::arrayCase1To2> cityNames; // creating a vector to have in each case the name of the city of that wilaya and a pointer to the vector of its districts

    return linkRestaurantsInWilaya((*wil)->nextTree, cityNames); // passing it to the helper function
}

vector<wilaya::arrayCase1To2> wilaya::linkRestaurantsInWilaya(BSNode* cityNode, vector<arrayCase1To2>& cityNames)
{
    if (cityNode != nullptr)
    {
        // Link restaurants in the left subtree (prioritize left-to-right order)
        linkRestaurantsInWilaya(cityNode->left, cityNames);

        // Create a new vector dynamically for each city
        vector<wilaya::arrayCaseRList>* districtNames = new vector<wilaya::arrayCaseRList>;
        *districtNames = linkRestaurantsInCity(cityNode->nextTree, *districtNames);
        cityNames.push_back({ cityNode->elementName, districtNames });

        // Link restaurants in the right subtree
        linkRestaurantsInWilaya(cityNode->right, cityNames);
    }

    return cityNames;
}

vector<wilaya::arrayCaseRList> wilaya::getCityRestaurants(string& wilayaName, string& cityName)
{
    BSNode** wil = getWilaya(wilayaName);
    BSNode** city = getCityOrDistrict(wil, cityName);
    vector<wilaya::arrayCaseRList> districtNames;
    return linkRestaurantsInCity((*city)->nextTree, districtNames);
}

vector<wilaya::arrayCaseRList> wilaya::linkRestaurantsInCity(BSNode* districtNode, vector<arrayCaseRList>& districtNames)
{
    if (districtNode != nullptr)
    {
        // Link restaurants in the left subtree (prioritize left-to-right order)
        linkRestaurantsInCity(districtNode->left, districtNames);

        // Correctly initialize arrayCaseRList with a pointer to the list of restaurants
        districtNames.push_back(arrayCaseRList(districtNode->elementName, districtNode->restaulist));

        // Link restaurants in the right subtree
        linkRestaurantsInCity(districtNode->right, districtNames);
    }
    return districtNames;
}

const wilaya::RestauNode* wilaya::getDistrictRestaurants(string& wilayaName, string& cityName, string& districtName)
{
    BSNode** wil = getWilaya(wilayaName);
    cout << "passed wil" << endl;
    BSNode** city = getCityOrDistrict(wil, cityName);
    cout << "passed city" << endl;

    BSNode** district = getCityOrDistrict(city, districtName);
    cout << "passed district" << endl;

    return (*district)->restaulist;
}

void wilaya::fillFromFile()
{
    // Open the file in input mode
    std::ifstream inFile("restaurantPlaces.txt", std::ios::in);

    // Check if the file is opened successfully
    if (!inFile.is_open())
    {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
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

void wilaya::makeFileEmpty()
{
    string filename = "restaurantPlaces.txt";
    // Open the file in truncation mode to clear its content or create it if it doesn't exist
    std::ofstream file(filename, std::ofstream::trunc);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    else
    {
        std::cout << "File " << filename << " created and cleared." << std::endl;
        file.close();
    }
}

void wilaya::traverseWilaya(BSNode* node) // Traverse the tree of the Wilaya by recursion
{
    if (node)
    {
        traverseWilaya(node->left);

        traverseCity(node->nextTree, node->elementName); // For each Wilaya we traverse it cities and send it's name as well to be stored with it

        traverseWilaya(node->right);
    }
}

void wilaya::traverseCity(BSNode* node, string& wilayaName) // Traverse the tree of the City of that Wilaya by recursion
{
    if (node)
    {
        traverseCity(node->left, wilayaName);

        traverseDistrict(node->nextTree, wilayaName, node->elementName); // For each City we traverse it Districts and send it's name as well to be stored with it

        traverseCity(node->right, wilayaName);
    }
}

void wilaya::traverseDistrict(BSNode* node, string& wilayaName, string& cityName) // Traverse the tree of the Districts of that City by recursion
{
    if (node)
    {
        traverseDistrict(node->left, wilayaName, cityName);

        insertIntoFile(node->restaulist, wilayaName, cityName, node->elementName); // For each District we insert its all restaurants' data into the file

        traverseDistrict(node->right, wilayaName, cityName);
    }
}

void wilaya::insertIntoFile(RestauNode* node, string& wilayaName, string& cityName, string& districtName)
{
    std::ofstream outFile("restaurantPlaces.txt", std::ios::out | std::ios::app);
    if (!outFile.is_open())
    {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }
    while (node != nullptr) // looping through the linked list and inserting each restaurant data respectively
    {
        outFile << wilayaName << "," << cityName << "," << districtName << "," << node->nameRest << "." << std::endl;
        node = node->next;
    }
    outFile.close();
}

void wilaya::fillFinalFile()
{
    makeFileEmpty();
    traverseWilaya(root);
}

wilaya::~wilaya()
{
    fillFinalFile();
}
