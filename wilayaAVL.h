#ifndef WILAYAAVL_H
#define WILAYAAVL_H

#pragma once
#include<iostream>
#include<vector>
using namespace std;
class wilayaAVL
{
public:
	struct RestauNode {
		string nameRest;
		int IDRest;
		RestauNode* next;
		RestauNode(string name = "");

	};
	struct AVLNode {
		string elementName;
		int elementID;//according to the ASCII
		AVLNode* left;
		AVLNode* right;
		AVLNode* nextTree;
		RestauNode* restaulist;
		int height;
		AVLNode(string name = "");
	};
	struct arrayCaseRList
	{
		string regionName;
		RestauNode* pointerToLinkedList; // Pointer to vector of the same type
		arrayCaseRList(string regionName = "", RestauNode* pToLinkedList = nullptr);
	};
	struct arrayCase1To2
	{
		string regionName;
		vector<arrayCaseRList>* pointerToVector; // Pointer to vector of the same type
		arrayCase1To2(string regionName = "", vector<arrayCaseRList>* pToVector = nullptr);
	};
	struct arrayCase
	{
		string regionName;
		vector<arrayCase1To2>* pointerToVector; // Pointer to vector of the same type
		arrayCase(string regionName = "", vector<arrayCase1To2>* pToVector = nullptr);
	};
	
	AVLNode* root;
	string countryName;
	string getCountryName()const;
	int calheight(AVLNode*& t);//calculate the height
	void makeEmpty(AVLNode*& t);
	void balance(AVLNode*& t);//balance the tree to protect avl property
	AVLNode* insert(AVLNode* node, string name);//insert avl node
	void rotateWithLeftChild(AVLNode*& c); //left child rotate
	void doubleWithLeftChild(AVLNode*& t);//double rotate left child
	void rotateWithRightChild(AVLNode*& p);//right child rotate
	void doubleWithRightChild(AVLNode*& t);//double rotate right child
	AVLNode* findMin(AVLNode* t);//searchh the min
	AVLNode* remove(int id, AVLNode*& t);//remove the node 
	wilayaAVL();//default constructor
	//wilaya(string name);
	void setId(AVLNode* node, string name);
	int getId(AVLNode* node)const;
	void setName(AVLNode* node, string name);
	string getName(AVLNode* node)const;
	//void setpointer(BSNode* root);
	AVLNode** getWilayaOrCreat(string& name);
	AVLNode** getCityOrDistrictOrCreat(AVLNode** wilayaName, string& cityName);
	bool addRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName);

	AVLNode** getWilaya(string& name);
	AVLNode** getCityOrDistrict(AVLNode** wilayaName, string& cityName);
	bool deleteRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName);
	bool doesExistCity(string& wilayaName, string& cityName);
	bool doesExistDistrict(string& wilayaName, string& cityName, string& districtName);
	bool doesExistRestaurant(string& wilayaName, string& cityName, string& districtName, string& restaurantName);
	bool doesExistWilaya(string& wilayaName);
	
	//Getters and Setters:
	void setCountryName(const string& name);//This is always asked for the user each time once he opens the program to be sure that is the needed country
	//bool restaurantIsExist(string& wilayaName, string& cityName, string& districtName, string& restaurantName);
	vector<arrayCase> getCountryRestaurants();
	vector<arrayCase> linkRestaurantsInCountry(AVLNode* cityNode, vector<arrayCase>& currentList);
	vector<arrayCase1To2> getWilayaRestaurants(string& name);
	vector<arrayCase1To2> linkRestaurantsInWilaya(AVLNode* cityNode, vector<arrayCase1To2>& cityNames);
	vector<arrayCaseRList> getCityRestaurants(string& wilayaName, string& cityName);
	vector<arrayCaseRList> linkRestaurantsInCity(AVLNode* districtNode, vector<arrayCaseRList>& districtNames);
	RestauNode* getDistrictRestaurants(string& wilayaName, string& cityName, string& districtName);
	//handle files
	void fillFromFile();
	void makeFileEmpty(const std::string& filename);
	void traverseWilaya(AVLNode*);
	void traverseCity(AVLNode*, string& wilayaName);
	void traverseDistrict(AVLNode*, string& wilayaName, string& cityName);
	void insertIntoFile(RestauNode*, string& wilayaName, string& cityName, string& districtName);
	void fillFinalFile();
	void makeEmpty();//make the tree empty
	~wilayaAVL();//destructor
};
#endif
