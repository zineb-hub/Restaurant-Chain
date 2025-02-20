#pragma once
#ifndef RESTAURANTTREE_H
#define RESTAURANTTREE_H
#include<iostream>
#pragma once
#include <algorithm>
#include<fstream>
#include<array>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;
int IDtCalculator(string name);
class restaurantTree
{
private:
	struct BSNode
	{
		string state;// Wanna know if it is owned or franchised
		string restaurantName;
		int restaurantID;//ASCII 
		int NumEmployees;
		string cuisines[5];
		BSNode* left;
		BSNode* right;
		BSNode();
		BSNode(string st, string name, int numEmp, BSNode* lt, BSNode* rt);
	public:
		void setrestauname(string name);
		string getrestauname()const;
		void setrestauid(string name);
		int getrestauid()const;
		void setnumEmp(int n);
		int getnumEmp()const;
		void setcuisines(string cuis[5]);
		array<string, 5> getcuisines()const;
		void setState(char c);
		string getState();
	};
	BSNode* root;
	//help functions
	void insert(string st, string name, int numEmp, BSNode*& t);
	void makEmpty(BSNode*& t);
	void printTree(BSNode*& t);
	int numbNodes(BSNode*& t);
	BSNode* findMin(BSNode* t);
	void removeRestaurant(int id, BSNode*& t);
	/*void saveinfile(const string&fname,BSNode*t);
	void readfromfile(const string& fname, BSNode*& t);*/
	void modifyrestaurant(int id, BSNode*& t);
	void traversetree(BSNode*& t);//function that traverse the tree
public:
	restaurantTree();
	~restaurantTree();
	bool isEmpty()const;
	void insert(string st, string name, int numEmp);
	void makEmpty();
	void printTree();
	int getResturantnum();
	void removeRestaurant(string name);
	/*void saveinfile(const string& fname);
	void readfromfile(const string& fname);*/
	void modifyrestaurant(const string& restname);
	//to handle files 
	void fillFromFileBST();
	void makeFileEmptyBST(const std::string& filename);
	void insertIntoFileBST(const string& st,const string& name, const int& numEmp);
	void fillFinalFileBST();
};
#endif


