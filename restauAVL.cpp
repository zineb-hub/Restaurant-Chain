#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>

#include "restauAVL.h"
int IDtCalculator(string name)
{ // function that calculate the id according to the ASCII
	int id = 0;
	for (char c : name) {
		id += (int)c;
		id *= 10;
	}
	return abs(id);
}
int restauAVL::numbNodes(AVLNode*& t)
{
	if (t == nullptr) {
		return 0;
	}
	else {
		return 1 + numbNodes(t->left) + numbNodes(t->right);
	}
}
void restauAVL::makEmpty(AVLNode*& t)
{
	if (t != nullptr) {
		makEmpty(t->left);
		makEmpty(t->right);
		delete t;
	}
	t = nullptr;
}

void restauAVL::printTree(AVLNode* t)const
{
	if (t != nullptr) {
		printTree(t->left);
		cout << "the restaurant name " << t->restaurantName << endl;
		cout << "the restaurant id " << t->restaurantID << endl;
		cout << " the number of employees " << t->NumEmployees << endl;
		cout << " the restaurant is :" << t->state << endl;
		//cout << "the height " << t->height << endl;
		cout << " the cuisines : " << endl;
		for (int i = 0; i < 5; i++) {
			cout << t->cuisines[i] << " ";
		}
		cout << endl;
		printTree(t->right);
	}
}

void restauAVL::insert(string st, string name, int numEmp, AVLNode*& t)
{
	int id = IDtCalculator(name);
	if (t == nullptr) {
		t = new AVLNode(st, name, numEmp, nullptr, nullptr);
	}
	else if (id < t->restaurantID) {
		insert(st, name, numEmp, t->left);
	}
	else if (id > t->restaurantID) {
		insert(st, name, numEmp, t->right);
	}
	/*else {
		throw runtime_error("The restaurant exists already");

	}*/
	balance(t);
}

void restauAVL::balance(AVLNode*& t)
{
	if (t == nullptr) return;
	if (height(t->left) - height(t->right) > 1) {//violation of avl property(in left subtree)
		if (height(t->left->left) > height(t->left->right)) { //left left problem
			rotateWithLeftChild(t);
		}
		else {//left right problem
			doubleWithLeftChild(t);
		}
	}
	else if (height(t->right) - height(t->left) > 1) {//violation of avl property(in right subtree)
		if (height(t->right->right) > height(t->right->left)) {//right right problem
			rotateWithRightChild(t);
		}
		else {//right LEFT problem
			doubleWithRightChild(t);
		}
	}
	//change the hight of the tree
	t->height = max(height(t->left), height(t->right)) + 1;
}

void restauAVL::rotateWithLeftChild(AVLNode*& c)
{ //left rotation
	AVLNode* p = c->left;
	c->left = p->right;
	p->right = c;
	c->height = max(height(c->left), height(c->right)) + 1;
	p->height = max(height(p->left), c->height) + 1;
	c = p;
}

void restauAVL::doubleWithLeftChild(AVLNode*& t)
{ //double rotation
	rotateWithRightChild(t->left);
	rotateWithLeftChild(t);
}

void restauAVL::rotateWithRightChild(AVLNode*& p)
{//right rotation
	AVLNode* c = p->right;
	p->right = c->left;
	c->left = p;
	p->height = max(height(p->left), height(p->right)) + 1;
	c->height = max(height(c->right), c->height) + 1;
	p = c;
}

void restauAVL::doubleWithRightChild(AVLNode*& t)
{//douuble rotation
	rotateWithLeftChild(t->right);
	rotateWithRightChild(t);
}

int restauAVL::height(AVLNode* t)
{//return the height id empty tree is -1
	if (t == nullptr) return -1;
	return t->height;
}

restauAVL::AVLNode* restauAVL::findMin(AVLNode* t)
{ //search the min node
	if (t == nullptr)
		return nullptr;//if empty tree

	if (t->left == nullptr)//if we reach the leaf in the left
		return t;

	return findMin(t->left);
}

void restauAVL::remove(int id, AVLNode*& t)
{
	if (t == nullptr)
		return;//empty tree
	if (id < t->restaurantID)
		remove(id, t->left);//search in left to remove
	else if (t->restaurantID < id)
		remove(id, t->right);//search in right to emove

	else if (t->left != nullptr && t->right != nullptr) // Two children
	{
		t->restaurantID = findMin(t->right)->restaurantID;
		remove(t->restaurantID, t->right);
	}
	else
	{
		AVLNode* old = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete old;
	}
	//balance the tree after removing the node to protect the avl proprety from violation
	balance(t);
}



//void restauAVL::saveinfile(const string& fname, AVLNode* t)
//{
//	ofstream ofile(fname, std::ios::out | std::ios::app);
//	if (!ofile.is_open()) {
//		cerr << "failed to open the file: " << fname << endl;
//		return;
//	}
//	if (t != nullptr) {
//		saveinfile(fname, t->left);
//		ofile << t->restaurantName << "," << t->restaurantID;
//		ofile << "," << t->NumEmployees << "," << t->state << ",";
//		for (int i = 0; i < 5; i++) {
//			ofile << t->cuisines[i] << ",";
//		}
//		ofile << endl;
//		saveinfile(fname, t->right);
//
//	}
//	if (t == nullptr) {
//		ofile.close();
//		cout << "Restaurant information has been successfully added to the file." << endl;
//
//	}
//}

//void restauAVL::readfromfile(const string& fname, AVLNode*& t)
//{
//	ifstream ifile(fname, std::ios::in);
//	if (!ifile.is_open()) {
//		cerr << "failed to open the file: " << fname << endl;
//		return;
//	}
//	string line;
//	while (getline(ifile, line)) {
//		stringstream ss(line);
//		string restaurantName, state;
//		string restaurantIDstr, NumEmployeesstr;
//		getline(ss, restaurantName, ',');
//		getline(ss, restaurantIDstr, ',');
//		getline(ss, NumEmployeesstr, ',');
//		string cuisines[5];
//
//		for (int i = 0; i < 5; i++) {
//			getline(ss, cuisines[i], ',');
//		}
//		int NumEmployees = stoi(NumEmployeesstr);
//		int restaurantID = stoi(restaurantIDstr);
//		insert(state, restaurantName, NumEmployees, cuisines);
//	}
//	ifile.close();
//}


void restauAVL::modifyrestaurant(int id, AVLNode*& t)
{ //modify the restaurant propreties 


	if (t == nullptr) {
		cout << "restaurant not found" << endl;
	}
	else {
		if (id < t->restaurantID) {
			modifyrestaurant(id, t->left);
		}
		else if (id > t->restaurantID) {
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
				remove(id, t);
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

void restauAVL::traversetree(AVLNode*& t)
{ //to traverse the tree
	if (t) {
		traversetree(t->left);
		//insertIntoFile(t,t->state,t->restaurantName,t->NumEmployees,t->cuisines);
		insertIntoFile(t->state, t->restaurantName, t->restaurantID, t->NumEmployees, t->cuisines);
		traversetree(t->right);
	}
}

restauAVL::restauAVL() :root(nullptr)
{
	fillFromFile();
}

restauAVL::~restauAVL()
{ //make the tree empty
	makeFileEmpty("myfile.txt");
	fillFinalFile();
	//makEmpty();
}

bool restauAVL::isEmpty() const
{
	return root == nullptr;
}

void restauAVL::printTree() const
{
	if (isEmpty()) {//handle exception
		cout << "no restaurant " << endl;
	}
	else printTree(root);
}

void restauAVL::makEmpty()
{
	makEmpty(root);
}

void restauAVL::insert(string st, string name, int numEmp)
{
	insert(st, name, numEmp, root);
}

void restauAVL::removeRestau(string nameR)
{ //call the remove helper 
	int id = IDtCalculator(nameR);
	remove(id, root);
}

void restauAVL::modifyrestaurantAVL(string name)
{
	int id = IDtCalculator(name);
	char c;
	do {
		modifyrestaurant(id, root);
		cout << "enter N to stop modifying" << endl;
		cin >> c;
	} while (toupper(c) != 'N');



}



restauAVL::AVLNode::AVLNode() :state(" "), restaurantName(" "), restaurantID(0), NumEmployees(0),
left(nullptr), right(nullptr)
{//default cunstroctor
	for (int i = 0; i < 5; i++) {
		cuisines[i] = " ";
	}
}

restauAVL::AVLNode::AVLNode(string st, string name, int numEmp, AVLNode* lt, AVLNode* rt) :state(st), restaurantName(name),
restaurantID(IDtCalculator(name)), NumEmployees(numEmp), left(lt), right(rt)
{//construcor


	cuisines[0] = "Algerian";
	cuisines[1] = "Syrian";
	cuisines[2] = "Chinese";
	cuisines[3] = "Indian";
	cuisines[4] = "European";
}
void restauAVL::AVLNode::setrestauname(string name)
{
	restaurantName = name;
}

string restauAVL::AVLNode::getrestauname() const
{
	return restaurantName;
}

void restauAVL::AVLNode::setrestauid(string name)
{
	restaurantID = IDtCalculator(name);
}

int restauAVL::AVLNode::getrestauid() const
{
	return restaurantID;
}

void restauAVL::AVLNode::setnumEmp(int n)
{
	NumEmployees = n;
}

int restauAVL::AVLNode::getnumEmp() const
{
	return NumEmployees;
}

void restauAVL::AVLNode::setcuisines(string cuis[5])
{
	for (int i = 0; i < 5; i++) {
		cuisines[i] = cuis[i];
	}
}

array<string, 5> restauAVL::AVLNode::getcuisines() const
{
	array<string, 5>result;
	for (int i = 0; i < 5; i++) {
		result[i] = cuisines[i];
	}
	return result;
}

void restauAVL::AVLNode::setState(char c)
{
	if (toupper(c) == 'F') {
		state = "franchised";
	}
	else if (toupper(c) == 'O') {
		state = "owned";
	}

}

string restauAVL::AVLNode::getState()
{
	return state;
}

int restauAVL::getResturantnum()
{
	return numbNodes(root);
}

//void restauAVL::saveinfileAVL(const string& fname)
//{
//	
//	saveinfile(fname, root);
//}
//
//void restauAVL::readfromfileAVL(const string& fname)
//{
//	makEmpty(root);
//	readfromfile(fname, root);
//}

restauAVL::AVLNode* restauAVL::getroot() const
{
	return root;
}

void restauAVL::fillFromFile()
{

	// Open the file in input mode
	std::ifstream inFile("myfile.txt", std::ios::in);

	// Check if the file is opened successfully
	if (!inFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}

	std::string line;
	while (std::getline(inFile, line)) {

		// Use stringstream to parse the line
		std::stringstream ss(line);
		std::string st;//read the state
		std::string name;//read the name of the restaurant
		//std::string numberOfEmployeesStr;comment
		//std::string IDstr;
	    string cuis[5];//read the 5 cuisines
		// Read the values separated by a comma
		std::getline(ss, st, ',');
		std::getline(ss, name, ',');
		//std::getline(ss, numberOfEmployeesStr, ',');comment
		
		int numEmp = 0;
		/*if (!numberOfEmployeesStr.empty()) { 
			numEmp = stoi(numberOfEmployeesStr);
		}*/
		//comment
		ss >> numEmp;
		ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Consume the newline character

		for (int i = 0; i < 5; i++) {
			std::getline(ss, cuis[i], ',');

		}

		insert(st, name, numEmp);
	}

	// Close the file
	inFile.close();
}

void restauAVL::makeFileEmpty(const std::string& filename)
{
	std::ofstream file(filename, std::ofstream::trunc);

	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
	}
	else {
		std::cout << "File " << filename << " created and cleared." << std::endl;
		file.close();
	}
}

void restauAVL::insertIntoFile(string& st, string& name, int& id, int& numEmp, string* cuis)
{

	std::ofstream outFile("myfile.txt", std::ios::out | std::ios::app);
	if (!outFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return;
	}
	outFile << st << "," << name << "," << numEmp << "," << cuis[0] << "," << cuis[1] << "," << cuis[2] << "," << cuis[3] << "," << cuis[4] << "." << std::endl;
	outFile.close();

}

void restauAVL::fillFinalFile()
{
	std::string filename = "myfile.txt";
	makeFileEmpty(filename);
	traversetree(root);
}



//void restauAVL::saveinfileAVL(const string& fname)
//{
//	saveinfile(fname,root);
//}
//
//void restauAVL::readfromfileAVL(const string& fname)
//{
//	readfromfile(fname,root);
//}
