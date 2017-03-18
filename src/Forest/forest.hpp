#ifndef __FOREST_HPP__
#define __FOREST_HPP__

#include <iostream>
#include <string>
#include <map>
#include "../enumerations.hpp"

using namespace std;

struct type_conc{
	struct node* left;
	struct node* right;
};

struct type_union{
	struct node* left;
	struct node* right;
};

struct type_star{
	struct node* son;
};

struct type_un{
	struct node* son;
};

struct type_atom{
	string code;
	int action;
	AtomType type;
};

struct node{
	Operation op;
	union{
		type_conc* conc;
		type_union* unio;
		type_star* star;
		type_un* un;
		type_atom* atom;
	}typeNode;
};

node* genConc(node* left, node* right);
node* genUnion(node* left, node* right);
node* genStar(node* s);
node* genUn(node* s);
node* genAtom(string code, int action, AtomType type);

void printTreeRec(node* n, int cpt);
void printTree(node* n);
// need to change as a map
map<string,node*> buildForest();
void printForest(map<string,node*> forest);

#endif // __FOREST_HPP__
