#include <iostream>
#include <string>
#include <vector>

using namespace std;


enum AtomType { Terminal, NonTerminal };

enum Operation { Conc, Union, Star, Un, Atom };

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

vector<char> separators;
vector<string> symbols;
vector<string> lexicalUnits;

node* genConc(node* left, node* right){
	node* n = new node();

	n->op = Conc;

	n->typeNode.conc = new type_conc();
	n->typeNode.conc->left = left;
	n->typeNode.conc->right = right;

	return n;
}

node* genUnion(node* left, node* right){
	node* n = new node();

	n->op = Union;

	n->typeNode.unio = new type_union();
	n->typeNode.unio->left = left;
	n->typeNode.unio->right = right;

	return n;
}

node* genStar(node* s){
	node* n = new node();

	n->op = Star;

	n->typeNode.star = new type_star();
	n->typeNode.star->son = s;

	return n;
}

node* genUn(node* s){
	node* n = new node();

	n->op = Un;

	n->typeNode.un = new type_un();
	n->typeNode.un->son = s;

	return n;
}

node* genAtom(string code, int action, AtomType type){
	node* n = new node();

	n->op = Atom;
	n->typeNode.atom = new type_atom();

	n->typeNode.atom->code = code;
	n->typeNode.atom->action = action;
	n->typeNode.atom->type = type;

	return n;
}

/**
* Les atomes doivent être sur les fils droits en priorité pour un affichage correct.
*
*/ 

void printTreeRec(node* n, int cpt){
	string term;
	unsigned int i;

	for( i = 0 ; i < cpt+1 ; ++i ){
		cout << "---";
	}

	switch(n->op){
		case Atom:
			if ( n->typeNode.atom->type == 0 )
				term = "Terminal";
			else
				term = "Non Terminal";

			cout << "> Atom " <<  n->typeNode.atom->code << " " << n->typeNode.atom->action << " " << term << endl;
		break;
		case Conc:
			cout << "> Conc" <<  endl;
			printTreeRec(n->typeNode.conc->right, cpt+1);
			printTreeRec(n->typeNode.conc->left, cpt+1);
		break;
		case Union:
			cout << "> Union" << endl;
			printTreeRec(n->typeNode.unio->right, cpt+1);
			printTreeRec(n->typeNode.unio->left, cpt+1);
		break;
		case Star:
			cout << "> Star" << endl;
			printTreeRec(n->typeNode.star->son, cpt+1);
		break;
		case Un:
			cout << "> Un" << endl;
			printTreeRec(n->typeNode.un->son, cpt+1);
		break;
	}
}

void printTree(node* n){

	printTreeRec(n, 0);
}

bool isSeparator(char charac){

	if (charac == ' ' || charac == '	' || charac == '\n'){
		return true;
	}

	return false;
}

bool isSymbol(char charac){

	if (charac == '+' || charac == '-' || charac == '=' || charac == '*' || charac == '&' || charac == '|' || charac == '/' || 
		charac == '^' || charac == '(' || charac == ')' || charac == '[' || charac == ']' || charac == '{' || charac == '}' ||
		charac == '#' || charac == ';' || charac == ':' || charac == ',' || charac == '<' || charac == '>' || charac == '\"'||
		charac == '\''|| charac == '@' || charac == '!' || charac == '?'){
		
		return true;
	}

	return false;
}

bool isPotentialySymbolDouble(char charac){
	if (charac == '=' || charac == '|' || charac == '&'  || charac == '<' || charac == '>' || charac == ':' || charac == '+' ||
		charac == '-' || charac == '!'){
		return true;
	}

	return false;
}

bool isSymbolDouble(char firstChar, char secondChar){
	switch(firstChar){
		case '=':
			return(secondChar == '=');
		break;
		case '|':
			return(secondChar == '|');
		break;
		case '&':
			return (secondChar == '&');
		break;
		case '<':
			return(secondChar == '<' || secondChar == '=');
		break;
		case '>':
			return(secondChar == '>' || secondChar == '=');
		break;
		case ':':
			return(secondChar == ':');
		break;
		case '+':
			return(secondChar == '+' || secondChar == '=');
		break;
		case '-':
			return(secondChar == '-' || secondChar == '=');
		break;
		case '!':
			return(secondChar == '=');
		break;
	}

	return false;
}

void scanner(string text){

	unsigned int i;

	string unit = "";
	char currentChar;
	string doubleSymbol;

	for(i = 0 ; i < text.size() ; ++i){
		currentChar = text.at(i);
		if(isSymbol(currentChar) || isSeparator(currentChar)){
			if (unit != "") {
				lexicalUnits.push_back(unit);
			}
			unit = "";

			if (isPotentialySymbolDouble(currentChar) && (i != text.size()-1)){
				if (isSymbolDouble(currentChar, text.at(i+1))){
					doubleSymbol = currentChar;
					doubleSymbol += text.at(i+1);
					symbols.push_back(doubleSymbol);
					i++;
				}
			}else if (isSymbol(currentChar)){
				symbols.push_back(string(1,currentChar)); // Cast Char to String

			}else if (isSeparator(currentChar)){
				separators.push_back(currentChar);
			}
		}else{
			unit += currentChar;
		}
	}
	// Add the last unit found
	lexicalUnits.push_back(unit);
}

void printLexicalUnits(){
	cout << "List of lexical units :" << endl;
	for (int i = 0; i < lexicalUnits.size(); ++i){
		cout << lexicalUnits[i] << endl;
	}
}

void printSeparators(){
	cout << "List of separators :" << endl;
	for (int i = 0; i < separators.size(); ++i){
		cout << i+1 << ")" << separators[i] << endl;
	}
}

void printSymbols(){
	cout << "List of symbols :" << endl;
	for (int i = 0; i < symbols.size(); ++i){
		cout << symbols[i] << endl;
	}
}

int main(){

	// cout << "++++++++++ A1 +++++++++"<< endl;

	// node* A1 = genConc( genStar( genConc(
	//  genConc( genConc( genAtom("N", 0, NonTerminal),
	//   genAtom("fleche", 0, Terminal)), genAtom("E",0,NonTerminal)),
	//    genAtom(";",1,Terminal))), genAtom(";",0,Terminal) ) ;

	// printTree(A1);

	// cout << "++++++++++ A2 +++++++++"<< endl;

	// node* A2 = genAtom("IDNTER", 0, NonTerminal);
 
	// printTree(A2);

	// cout << "++++++++++ A3 +++++++++"<< endl;

	// node* A3 = genConc ( genStar( genConc ( genAtom("+",0,Terminal), genAtom("T", 0, NonTerminal))), genAtom("T",0,NonTerminal) );

	// printTree(A3);

	// cout << "++++++++++ A4 +++++++++"<< endl;

	// node* A4 = genConc ( genStar( genConc ( genAtom("F",0,NonTerminal), genAtom(".", 0, NonTerminal))), genAtom("F",0,NonTerminal) );

	// printTree(A4);

	// cout << "++++++++++ A5 +++++++++"<< endl;

	// node* A5 = genUnion( genUnion( genUnion( genUnion( genConc( genConc( 
	// 				genAtom("(/",0,Terminal),genAtom("E",0,NonTerminal) ),
	// 					genAtom( "/)",0,Terminal)),
	// 						genConc( genConc( genAtom( "[",0,Terminal ), genAtom( "E",0,NonTerminal )),
	// 								genAtom( "]",0,Terminal )) ), 
	// 						genConc( genConc( genAtom( "(",0,Terminal ), genAtom( "E",0,NonTerminal )),
	// 								genAtom( ")",0,Terminal )) ), genAtom( "ELTER",0,Terminal)) , genAtom( "IDNTER",0,Terminal));

	// printTree(A5);

	scanner("coucou poney+++++++&&loul"); // TO DO Check le resultat

	printLexicalUnits();
	printSymbols();
	printSeparators();

	return 0;
}
