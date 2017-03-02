#include <iostream>
#include <string>
#include <vector>

using namespace std;


enum AtomType { Terminal, NonTerminal };

enum Code { ELTER, IDNTER };

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

struct lexical_unit{
	string chaine;
	int action;
	AtomType type;
	Code code;
};

vector<char> separators;
vector<string> symbols;
vector<string> lexicalErrors;
vector<lexical_unit> lexicalUnits;

vector<lexical_unit> lexicalTerminal;
vector<lexical_unit> lexicalNonTerminal;

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

void printForest(){

	cout << "++++++++++ A1 +++++++++"<< endl;

	node* A1 = genConc( genStar( genConc(
	 genConc( genConc( genAtom("N", 0, NonTerminal),
	  genAtom("fleche", 0, Terminal)), genAtom("E",0,NonTerminal)),
	   genAtom(";",1,Terminal))), genAtom(";",0,Terminal) ) ;

	printTree(A1);

	cout << "++++++++++ A2 +++++++++"<< endl;

	node* A2 = genAtom("IDNTER", 0, NonTerminal);
 
	printTree(A2);

	cout << "++++++++++ A3 +++++++++"<< endl;

	node* A3 = genConc ( genStar( genConc ( genAtom("+",0,Terminal), genAtom("T", 0, NonTerminal))), genAtom("T",0,NonTerminal) );

	printTree(A3);

	cout << "++++++++++ A4 +++++++++"<< endl;

	node* A4 = genConc ( genStar( genConc ( genAtom("F",0,NonTerminal), genAtom(".", 0, NonTerminal))), genAtom("F",0,NonTerminal) );

	printTree(A4);

	cout << "++++++++++ A5 +++++++++"<< endl;

	node* A5 = genUnion( genUnion( genUnion( genUnion( genConc( genConc( 
					genAtom("(/",0,Terminal),genAtom("E",0,NonTerminal) ),
						genAtom( "/)",0,Terminal)),
							genConc( genConc( genAtom( "[",0,Terminal ), genAtom( "E",0,NonTerminal )),
									genAtom( "]",0,Terminal )) ), 
							genConc( genConc( genAtom( "(",0,Terminal ), genAtom( "E",0,NonTerminal )),
									genAtom( ")",0,Terminal )) ), genAtom( "ELTER",0,Terminal)) , genAtom( "IDNTER",0,Terminal));

	printTree(A5);
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
		charac == ';' || charac == ':' || charac == ',' || charac == '<' || charac == '>' || charac == '\''|| charac == '@' || 
		charac == '!' || charac == '?'){
			
			return true;
	}

	return false;
}

bool isPotentialySymbolDouble(char charac){
	if (charac == '=' || charac == '|' || charac == '&' || charac == '<' || charac == '>' || charac == ':' || charac == '+' ||
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
			return(secondChar == '-' || secondChar == '=' || secondChar == '>');
		break;
		case '!':
			return(secondChar == '=');
		break;
	}

	return false;
}

bool isInteger(char charac){
	if (charac == '0' || charac == '1' || charac == '2' || charac == '3' || charac == '4' || charac == '5' || 
		charac == '6' || charac == '7' || charac == '8' || charac == '9'){
		return true;
	}

	return false;
}

bool isNumber(string unit){

	for (int i = 0; i < unit.size(); ++i){
		if (!isInteger(unit.at(i))){
			return false;
		}
	}

	return true;
}

bool isString(string unit){

	if (unit != ""){
		if(unit.at(0) == '\"' && unit.at(unit.size()-1 == '\"')){
			return true;
		}
	}

	return false;
}

int actionOfLexicalUnit(string unit){

	string action;

	for(int i = 0 ; i < unit.size() ; ++i){
		action = unit.substr(i+1, unit.size()-1);

		if (unit.at(i) == '#'){
			if (isNumber(action)){
				return atoi(action.c_str());
			}else{
				return -2;
			}
		}
	}

	return -1;
}

string rmActionFromLexicalUnit(string unit){

	string unitWithoutAction = "";
	unsigned int i = 0;

	while( (unit.at(i) != '#') && (i < unit.size()) ){
		unitWithoutAction += unit.at(i);
		++i;
	}

	return unitWithoutAction;
}

void addLexicalUnit(string chaine, int action, AtomType type, Code code){

	lexical_unit currentLexical;

	currentLexical.chaine = chaine;
	currentLexical.action = action; // -1 mean no action
	currentLexical.type = type;
	currentLexical.code = code;

	lexicalUnits.push_back(currentLexical);

	if(type == Terminal){
		lexicalTerminal.push_back(currentLexical);
	}else{
		lexicalNonTerminal.push_back(currentLexical);
	}
}

void analyseUnit(string unit){

	int action;

	if (unit != "") {
		if(!isInteger(unit.at(0))){
			action = actionOfLexicalUnit(unit);
			// find the action of a string if there is one (-2 for an error, -1 for a unit without code and the code otherwise)

			if (action == -1){
				if(isString(unit)){
					addLexicalUnit(unit, action, Terminal, ELTER);
				}else{
					addLexicalUnit(unit, action, NonTerminal, IDNTER);
				}
			}else if (action == -2){
				lexicalErrors.push_back(unit);
			}else{
				addLexicalUnit(rmActionFromLexicalUnit(unit), action, NonTerminal, IDNTER);
			}
		}else if(isNumber(unit)){
			addLexicalUnit(unit, -1, Terminal, ELTER);
		}else{
			lexicalErrors.push_back(unit);
		}
	}
}

// previous version of Scanner

// void scanner(string text){

// 	unsigned int i;

// 	string unit = "";
// 	char currentChar;
// 	string doubleSymbol;

// 	for(i = 0 ; i < text.size() ; ++i){
// 		currentChar = text.at(i);
// 		if(isSymbol(currentChar) || isSeparator(currentChar)){
// 			if (unit != "") {
// 				if (!isInteger(unit.at(0))){
// 					lexicalUnits.push_back(unit);
// 				}else if(isNumber(unit)){
// 					lexicalUnits.push_back(unit);
// 				}else{
// 					lexicalErrors.push_back(unit);
// 				}
// 			}
// 			unit = "";

// 			if (isPotentialySymbolDouble(currentChar) && (i != text.size()-1)){
// 				if (isSymbolDouble(currentChar, text.at(i+1))){
// 					doubleSymbol = currentChar;
// 					doubleSymbol += text.at(i+1);
// 					symbols.push_back(doubleSymbol);
// 					++i;
// 				}else{
// 					symbols.push_back(string(1,currentChar)); // Cast Char to String				}
// 				}
// 			}else if (isSymbol(currentChar)){
// 				symbols.push_back(string(1,currentChar)); // Cast Char to String

// 			}else if (isSeparator(currentChar)){
// 				separators.push_back(currentChar);
// 			} // TO DO -> Gérer les actions
// 		}else{
// 			unit += currentChar;
// 		}
// 	}
// 	// Add the last unit found
// 	lexicalUnits.push_back(unit);
// }

void scanner (string text){

	string unit = "";
	char currentChar;
	string doubleSymbol, newDoubleSymbol;

	for(int i = 0 ; i < text.size() ; ++i){
		currentChar = text.at(i);

		if(isSymbol(currentChar) || isSeparator(currentChar)){



			if (isPotentialySymbolDouble(currentChar) && (i != text.size()-1)){
				if (isSymbolDouble(currentChar, text.at(i+1))){
					doubleSymbol = currentChar;
					doubleSymbol += text.at(i+1);

					if(unit.at(0) == '\"' && text.at(i+2) == '\"'){

						newDoubleSymbol = "\"" + doubleSymbol +"\"";
						addLexicalUnit(newDoubleSymbol, -1, Terminal, ELTER);
						i = i + 2;
						// TO DO : check si ça marche ça
					}

				}else{
					symbols.push_back(string(1,currentChar)); // Cast Char to String				}
				}
			}else if (isSymbol(currentChar)){
				symbols.push_back(string(1,currentChar)); // Cast Char to String

			}else if (isSeparator(currentChar)){
				separators.push_back(currentChar);
			}


			analyseUnit(unit);
			unit = "";

		}else{
			unit += currentChar;
		}
	}
	analyseUnit(unit); // analyse the last unit found
}

void printLexicalUnits(vector<lexical_unit> listLexical){
	for (int i = 0; i < listLexical.size(); ++i){
		cout << "----------------" << endl;

		cout << "Chaine : " << listLexical[i].chaine << endl;
		cout << "Action : " << listLexical[i].action << endl;
		cout << "type : " << listLexical[i].type << endl;
		cout << "Code : " << listLexical[i].code << endl;

	}
}

// void printSeparators(){
// 	cout << "List of separators :" << endl;
// 	for (int i = 0; i < separators.size(); ++i){
// 		cout << i+1 << separators[i] << endl;
// 	}
// }

// void printSymbols(){
// 	cout << "List of symbols :" << endl;
// 	for (int i = 0; i < symbols.size(); ++i){
// 		cout << symbols[i] << endl;
// 	}
// }

void printLexicalErrors(){
	cout << "List of lexical errors :" << endl;
	for (int i = 0; i < lexicalErrors.size(); ++i){
		cout << lexicalErrors[i] << endl;
	}
}

int main(){

	// printForest();

	scanner("1coucou \"rioe54ere + ioehrz#2 654654 iorue#23ko");

	cout << "------------------------------------" << endl;
	cout << "All lexical units" << endl;
	printLexicalUnits(lexicalUnits);


	cout << "------------------------------------" << endl;
	cout << "Lexical units Terminal" << endl;
	printLexicalUnits(lexicalTerminal);

	cout << "------------------------------------" << endl;
	cout << "Lexical units NonTerminal" << endl;
	printLexicalUnits(lexicalNonTerminal);

	// printSymbols();
	// printSeparators();
	printLexicalErrors();

	return 0;
}
