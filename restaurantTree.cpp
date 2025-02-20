#include<iostream>
#include<array>
#include <fstream>
#include <sstream>
#include <limits>
#include "restaurantTree.h"
void restaurantTree::insert(string st, string name, int numEmp, BSNode*& t)
{
	int id = IDtCalculator(name);//calculate the id to build the BST according to the name of the restaurant
	if (t == nullptr) { //if the tree is empty create a new node
		t = new BSNode(st, name, numEmp, nullptr, nullptr);
	}
	else if (id < t->restaurantID) { //if the id of the new restaurant is less then the id of the restaurant insert in left
		insert(st, name, numEmp, t->left);
	}
	else if (id > t->restaurantID) { //if the id of the new restaurant is greater then the id of the restaurant insert in left
		insert(st, name, numEmp, t->right);
	}
	//else { // if the restaurant exist throw an exception
	//	throw runtime_error("The restaurant exists already");

	//}
}

void restaurantTree::makEmpty(BSNode*& t)
{
	if (t != nullptr) {
		makEmpty(t->left);//delete the left subtree
		makEmpty(t->right);// delete the right subtree
		delete t;//delete the root
	}
	t = nullptr;
}

void restaurantTree::printTree(BSNode*& t)
{
	if (t != nullptr) {
		printTree(t->left);//print the left subtree
		// print the root
		cout << "the restaurant name " << t->restaurantName << endl;
		cout << "the restaurant id " << t->restaurantID << endl;
		cout << " the number of employees " << t->NumEmployees << endl;
		cout << " the restaurant is :" << t->state << endl;
		cout << " the cuisines : " << endl;
		for (int i = 0; i < 5; i++) {
			cout << t->cuisines[i] << " ";
		}
		cout << endl;
		//print the right subtree
		printTree(t->right);
	}

}

int restaurantTree::numbNodes(BSNode*& t)
{   //calculate the number of restaurants
	if (t == nullptr) {
		return 0;
	}
	else {
		return 1 + numbNodes(t->left) + numbNodes(t->right);
	}
}


void restaurantTree::removeRestaurant(int id, BSNode*& t)
{
	if (t == nullptr) return;//the tree is empty
	//search the restaurant in the left or in the right then remove it
	else if (id < t->restaurantID) {
		removeRestaurant(id, t->left);
	}
	else if (id > t->restaurantID) {
		removeRestaurant(id, t->right);
	}
	else if (t->left != nullptr && t->right != nullptr) {
		t->restaurantID = findMin(t->right)->restaurantID;
		removeRestaurant(t->restaurantID, t->right);
	}
	else {
		BSNode* old = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete old;
	}
	cout << "restau removed " << endl;
}
void restaurantTree::modifyrestaurant(int id, BSNode*& t)
{ //modify the restaurant propertied except the name can not be modified 'registre de commerce'
	if (t == nullptr) {//if we search all the tree and can't find it or if the tree is empty
		cout << "restaurant not found" << endl;
	}
	else {
		if (id < t->restaurantID) {//if less search in the left and modify
			modifyrestaurant(id, t->left);
		}
		else if (id > t->restaurantID) {//if greater search in the left and modify
			modifyrestaurant(id, t->right);
		}
		else {
			int c;
			cout << "enter a choice " << endl;
			cout << " 1 to change the state " << endl;
			cout << " 2 to change the cuisines " << endl;
			cout << "3 to remove the restaurant " << endl;
			cout << "4 to change the number of employees" << endl;
			cin >> c;
			if (c == 1) {
				char s;
				cout << "enter a state  " << endl;
				cout << " F -> franchised" << endl;
				cout << " O->owned " << endl;
				cin >> s;
				t->setState(s);
			}
			else if (c == 2) {
				string cuis[5] = {};
				cout << "enter the cuisines " << endl;
				for (int i = 0; i < 5; i++) {
					cin >> cuis[i];
				}
				t->setcuisines(cuis);
			}
			else if (c == 3) {
				cout << " remove the restaurant" << endl;
				removeRestaurant(id, t);
			}
			else if (c == 4) {
				int num;
				cout << "enter the number of employee " << endl;
				cin >> num;
				t->setnumEmp(num);
			}
			else {
				cout << c << " invalid" << endl;
			}



		}
	}
}

void restaurantTree::traversetree(BSNode*& t)
{
	if (t) {
		traversetree(t->left);
		//insertIntoFile(t,t->state,t->restaurantName,t->NumEmployees,t->cuisines);
		insertIntoFileBST(t->state, t->restaurantName,  t->NumEmployees);
		traversetree(t->right);
	}
}

restaurantTree::restaurantTree() :root(nullptr)
{
	fillFromFileBST();
}

restaurantTree::~restaurantTree()
{ //destructor
	/*makEmpty(root);*/
	makeFileEmptyBST("mybstfile.txt");
	fillFinalFileBST();
}

bool restaurantTree::isEmpty() const
{ //check if empty tree
	return root == nullptr;
}

void restaurantTree::insert(string st, string name, int numEmp)
{ //call insert helper
	insert(st, name, numEmp, root);
}

restaurantTree::BSNode* restaurantTree::findMin(BSNode* t)
{ //find the min node in BST 
	if (t == nullptr) return nullptr;//empty tree
	else if (t->left == nullptr) return t; // tree has only one node
	else return findMin(t->left);//search in the left -left (until a leaf is reached)
}

void restaurantTree::makEmpty()
{ //call makempty helper with the root to make all the tree empty
	makEmpty(root);
}

void restaurantTree::printTree()
{ //call printtree helper with the root to print all the tree
	printTree(root);
}

int restaurantTree::getResturantnum()
{ //call numNodes that calculate the number of nodes so the number of restaurants
	//cout << "here restua num" << endl;
	return numbNodes(root);
}

void restaurantTree::removeRestaurant(string name)
{  //cal remove helper
	int id = IDtCalculator(name);
	removeRestaurant(id, root);
}

//geters and setrs 
string restaurantTree::BSNode::getrestauname() const
{
	return restaurantName;
}

void restaurantTree::BSNode::setrestauid(string name)
{
	restaurantID = IDtCalculator(name);
}

int restaurantTree::BSNode::getrestauid() const
{
	return restaurantID;
}

void restaurantTree::BSNode::setnumEmp(int n)
{
	NumEmployees = n;
}

int restaurantTree::BSNode::getnumEmp() const
{
	return NumEmployees;
}

void restaurantTree::BSNode::setcuisines(string cuis[5])
{
	for (int i = 0; i < 5; i++) {
		cuisines[i] = cuis[i];
	}
}

array<string, 5> restaurantTree::BSNode::getcuisines() const
{
	array<string, 5>result;
	for (int i = 0; i < 5; i++) {
		result[i] = cuisines[i];
	}
	return result;
}

void restaurantTree::BSNode::setState(char c)
{
	if (toupper(c) == 'F') {
		state = "franchised";
	}
	else if (toupper(c) == 'O') {
		state = "owned";
	}

}

string restaurantTree::BSNode::getState()
{
	return state;
}



void restaurantTree::modifyrestaurant(const string& restname)
{ //call modify helper
	int id = IDtCalculator(restname);
	modifyrestaurant(id, root);
}

void restaurantTree::fillFromFileBST()
{

	// Open the file in input mode
	ifstream inFile("mybstfile.txt", std::ios::in);

	// Check if the file is opened successfully
	if (!inFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}

	string line;
	while (std::getline(inFile, line)) {

		// Use stringstream to parse the line
		std::stringstream ss(line);
		std::string st;//read the state
		std::string name;//read the name of the restaurant
		string cuis[5];//read the 5 cuisines
		// Read the values separated by a comma
		getline(ss, st, ',');
		getline(ss, name, ',');
	
		int numEmp = 0;
		/*if (!numberOfEmployeesStr.empty()) {
			numEmp = stoi(numberOfEmployeesStr);
		}
		else { numberOfEmployeesStr = "undifined"; }*/
		/*if (!IDstr.empty()) {
			int id = std::stoi(IDstr);
		}*/

		ss >> numEmp;
		ss.ignore(numeric_limits<streamsize>::max(), '\n');  // Consume the newline character
		for (int i = 0; i < 5; i++) {
		   getline(ss, cuis[i], ',');
		}

		insert(st, name, numEmp);
	}


}

void restaurantTree::makeFileEmptyBST(const string& filename)
{
	std::ofstream file(filename, std::ofstream::trunc);

	if (!file.is_open()) {
		cerr << "Error opening file: " << filename << endl;
	}
	else {
		cout << "File " << filename << " created and cleared." << endl;
		file.close();
	}
}

void restaurantTree::insertIntoFileBST(const string& st, const string& name, const int& numEmp)
{
	ofstream outFile("mybstfile.txt", std::ios::out | std::ios::app);
	if (!outFile.is_open()) {
		cerr << "Error opening the file!" << endl;
		return;
	}
	array<string, 5> cuis = root->getcuisines();
	outFile << st << "," << name << "," << numEmp << "," << cuis[0] << "," << cuis[1] << "," << cuis[2] << "," << cuis[3] << "," << cuis[4] << "." << endl;
	outFile.close();
}

void restaurantTree::fillFinalFileBST()
{
	string filename = "mybstfile.txt";
	makeFileEmptyBST(filename);
	traversetree(root);
}




restaurantTree::BSNode::BSNode() :state(" "), restaurantName(" "), restaurantID(0), NumEmployees(0),
left(nullptr), right(nullptr)
{ // default constructor of BSNode
	for (int i = 0; i < 5; i++) {
		cuisines[i] = " ";
	}
}

restaurantTree::BSNode::BSNode(string st, string name, int numEmp, BSNode* lt, BSNode* rt) :state(st), restaurantName(name),
restaurantID(IDtCalculator(name)), NumEmployees(numEmp), left(lt), right(rt)
{//  constructor of BSNode

	cuisines[0] = "Algerian";
	cuisines[1] = "Syrian";
	cuisines[2] = "Chinese";
	cuisines[3] = "Indian";
	cuisines[4] = "European";

}


void restaurantTree::BSNode::setrestauname(string name)
{
	restaurantName = name;
}
int IDtCalculator(string name)
{ // function that calculate the id according to the ASCII
	int id = 0;
	for (char c : name) {
		id += (int)c;
		id *= 10;
	}
	return abs(id);
}


