#pragma once
#ifndef RESTAURAAVL_H
#define RESTAUAVL_H
#include<iostream>
#include<array>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;
int IDtCalculator(string name);
class restauAVL
{
private:
	struct AVLNode
	{
		string state;// Wanna know if it is owned or franchised
		string restaurantName;
		int restaurantID;//ASCII 
		int NumEmployees;
		string cuisines[5];
		AVLNode* left;
		AVLNode* right;
		int height;

		AVLNode();
		AVLNode(string st, string name, int numEmp, AVLNode* lt, AVLNode* rt);
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
	AVLNode* root;
	int numbNodes(AVLNode*& t);//calculate number of nodes
	void makEmpty(AVLNode*& t);//make the tree empty
	void printTree(AVLNode* t)const; //print helper
	void insert(string st, string name, int numEmp, AVLNode*& t);//insert helper
	void balance(AVLNode*& t);// function to balance the tree and protect the proprety of avl tree
	void rotateWithLeftChild(AVLNode*& c);//to do left child rotation
	void doubleWithLeftChild(AVLNode*& t);// to do double left child rotation
	void rotateWithRightChild(AVLNode*& t); //to do right child rotation
	void doubleWithRightChild(AVLNode*& t);//to do double right child rotation
	int height(AVLNode* t);//function that calculate the height of the avl tree

	AVLNode* findMin(AVLNode* t);// function that find the min node in avl tree
	void remove(int id, AVLNode*& t);//remove helper(remove a node)
	// void saveinfile(const string& fname, AVLNode* t);
	// void readfromfile(const string& fname, AVLNode*& t);

	void modifyrestaurant(int id, AVLNode*& t);//modify helper
	void traversetree(AVLNode*& t);//function that traverse the tree
public:
	restauAVL();
	~restauAVL();
	bool isEmpty()const;
	void printTree()const;
	void makEmpty();
	void insert(string st, string name, int numEmp);
	void removeRestau(string nameR);
	void modifyrestaurantAVL(string name);
	int getResturantnum();
	//void saveinfileAVL(const string& fname);
	//void readfromfileAVL(const string& fname);
	AVLNode* getroot()const;
	//to handle files 
	void fillFromFile();
	void makeFileEmpty(const std::string& filename);
	void insertIntoFile(string& st, string& name, int& id, int& numEmp, string* cuis);
	void fillFinalFile();

};
#endif

