// yourClass.h
#ifndef WILAYA_H
#define WILAYA_H

#pragma once
#include <iostream>
#include <vector>
using namespace std;

class wilaya
{ // This class will be linking all of my trees
public:
	struct RestauNode
	{								  // A struct for the nodes of the linked list that has the names of the restaurants
		string nameRest;			  // The name of the restaurant
		int IDRest;					  // The id of the restaurant
		RestauNode* next;			  // A pointer to the next restaurant in the linked list
		RestauNode(string name = ""); // Constructor to initialize the name of the restaurant
	};
	// The next three structs were made to return the data that will be used in time(the exact restaurants in the exact region the user looking for)
	struct arrayCaseRList // A struct for the cases for an array that each case have a name of the wanted district and a pointer to a linked list that has the restaurants of that district
	{
		string regionName;															 // The name of the district
		const RestauNode* pointerToLinkedList;										 // Pointer to the linked list that has the restaurants, it should be const so it does not modify be mistake the data
		arrayCaseRList(string regionName = "", RestauNode* pToLinkedList = nullptr); // constructor to the struct to initialise the name of the district
	};
	struct arrayCase1To2 // A struct for the case when the needed data is each all the restaurant of a wilaya, then to return them organized we will use that each case has the name of the city and a pointer to a vect of the type of the prev struct
	{
		string regionName;																	// the name of the city
		vector<arrayCaseRList>* pointerToVector;											// Pointer to vector of the type arrayCaseRList, so that each city has its districts and each district its restaurants
		arrayCase1To2(string regionName = "", vector<arrayCaseRList>* pToVector = nullptr); // a constructor to initialize the name of the city
	};
	struct arrayCase // A struct in case the needed data is all Wilayas restaurants
	{
		string regionName;															   // the name of the Wialay
		vector<arrayCase1To2>* pointerToVector;										   // Pointer to vector of type arrayCase1To2 that has for each Wilaya its cities and for each city its districts and for each district its restaurants
		arrayCase(string regionName = "", vector<arrayCase1To2>* pToVector = nullptr); // A constructor to initialize the name of the Wilaya
	};

	struct BSNode
	{							  // A common node for all the trees for simplicity purposes that will be used for Wilaya, City and Distric since their data are similar
		string elementName;		  // The name of the region
		int elementID;			  // according to the IDCalculator function
		BSNode* left;			  // the left child of that node
		BSNode* right;			  // the right child of that node
		BSNode* nextTree;		  // the next tree refer to the tree of the cities of that wilaya if the node is Wilaya, and refer to the tree of districts of that city node and so on
		RestauNode* restaulist;	  // This is for simplicity and optimization purposes and is used only when the node is a district which has a linked list contains the restaurants of that region
		BSNode(string name = ""); // A constructor to initialize the name of that region
	};

	wilaya();				 // constructor for the class
	string getCountryName(); // Returns the name of the country

	// Functions for inserting a restaurant in its proper position
	bool addRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName); // Adds restaurant to the specific region (Wilaya, City, District) and returns true if it was successfully added and false otherwise(in case it already exists for example)

	// Functions for deleting a restaurant
	bool deleteRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName); // Deletes specified restaurant (Wilaya, City, District) and returns true if it was successfully deleted and false otherwise(in case it does not already exists for example)
	// hi
	bool doesExistCity(string& wilayaName, string& cityName);

	bool doesExistDistrict(string& wilayaName, string& cityName, string& districtName);
	bool doesExistRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName);
	bool doesExistWilaya(string& wilayaName);

	// Functions to return vectors that will be used in time functions in case the user want the sales in a specific region in a specific period
	vector<arrayCase> getCountryRestaurants();															  // in case the user has asked for all the restaurants of the country it will be returned organized each in its Wilaya, City, District respectively
	vector<arrayCase1To2> getWilayaRestaurants(string& name);											  // in case the user has asked for all the restaurants of A specific Wilaya it will be returned organized each in its City, District respectively
	vector<arrayCaseRList> getCityRestaurants(string& wilayaName, string& cityName);					  // in case the user has asked for all the restaurants of A specific City it will be returned organized each in its District respectively
	const RestauNode* getDistrictRestaurants(string& wilayaName, string& cityName, string& districtName); // Will return a const linked list that has the restaurants of a specified District
	void makeFileEmpty();																				  // Makes the file empty

	~wilaya(); // In the destructor the function fillFinalFile will be called when the user want to quit the program then the data will be modifed and updated

private:
	BSNode* root;		// always points to the head of the tree in all cases if it was a tree of Wilaya, Cities or Districts
	string countryName; // This is only initialized once and refer to all the country name

	// Getters and Setters:
	void setCountryName(string& name);		 // This is always asked for the user each time once he opens the program to be sure that is the needed country
	void setId(BSNode* node, string name);	 // Set the Id of that node
	int getId(BSNode* node) const;			 // Get the Id of that node
	void setName(BSNode* node, string name); // Set the name of the region of that node
	string getName(BSNode* node) const;		 // get the name of the region of that node

	// Functions for inserting a restaurant in its proper position
	BSNode** getWilayaOrCreat(string& name);												// A helper function for the function addRestaurant that returns the node of the specific Wilaya with that name
	BSNode** getCityOrDistrictOrCreat(BSNode** wilayaOrCityNode, string& wilayaOrCityName); // A helper function for the function addRestaurant that returns the node of the specific City or District with that name

	// Functions for deleting a restaurant
	BSNode** getWilaya(string& name);												 // A helper function for deleteRestaurant...
	BSNode** getCityOrDistrict(BSNode** wilayaOrCityNode, string& wilayaOrCityName); // A helper function...

	// Helper Functions to return vectors that will be used in time functions in case the user want the sales in a specific region in a specific period
	vector<arrayCase> linkRestaurantsInCountry(BSNode* cityNode, vector<arrayCase>& currentList);			   // A helper function for getCountryRestaurants
	vector<arrayCase1To2> linkRestaurantsInWilaya(BSNode* cityNode, vector<arrayCase1To2>& cityNames);		   // A helper function for getWilayaRestaurants
	vector<arrayCaseRList> linkRestaurantsInCity(BSNode* districtNode, vector<arrayCaseRList>& districtNames); // A helper function for getCityRestaurants

	// Helper Functions for handling the files so the data of the user kept stored even when closing the program
	void fillFromFile();																			   // This will be used in the constructor so all the trees gets filled exactly like how the user let it last time before closing the program
	void traverseWilaya(BSNode*);																	   // A helper function to be used to traverse the Wilaya trees to be inserted in the file at the end of the program
	void traverseCity(BSNode*, string& wilayaName);												   // A helper function to be used to traverse the City trees to be inserted in the file at the end of the program
	void traverseDistrict(BSNode*, string& wilayaName, string& cityName);							   // A helper function to be used to traverse the District trees to be inserted in the file at the end of the program
	void insertIntoFile(RestauNode* node, string& wilayaName, string& cityName, string& districtName); // insert into the file row per row, each for a specific restaurant that has its Wilaya, City, District

	void fillFinalFile(); // This function will be used with the function insertIntoFile and makeFileEmpty so that the file gets empty and refilled again in case of modification in the trees so it guarantees that the time complexity stays the same optimized and to prevent it from going to O(N)
};

#endif