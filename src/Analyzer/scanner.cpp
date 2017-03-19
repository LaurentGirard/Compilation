#include "scanner.hpp"
#include "../Forest/forest.hpp"

map<string,node*> forest;

vector<char> separators;
vector<string> symbols;
vector<string> lexicalErrors;
// vector<lexical_unit> lexicalUnits;
vector<lexical_unit*> lexicalTerminal;
vector<lexical_unit*> lexicalNonTerminal;

vector<node*> pile;

string toScan;// = "\"izhe\"        \"->\" -> rioe54e+re dzenl#3 \"coucou lol\" dsf \"-> ioehrz#2 \" 654654 iorueko#23k ezae";

unsigned int scanIterator = 0;

void putFileIntoString(string filename){

	ifstream in(filename);
	string toString((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

	toScan = toString;
}

bool isSeparator(char charac){

	return (charac == ' ' || charac == '	' || charac == '\n');
}

bool isSymbol(char charac){

	return (charac == '+' || charac == '-' || charac == '=' || charac == '*' || charac == '&' || charac == '|' || charac == '/' || 
			charac == '^' || charac == '(' || charac == ')' || charac == '[' || charac == ']' || charac == '{' || charac == '}' ||
			charac == ';' || charac == ':' || charac == ',' || charac == '<' || charac == '>' || charac == '\''|| charac == '@' || 
			charac == '!' || charac == '?');

}

bool isPotentialySymbolDouble(char charac){
	return (charac == '=' || charac == '|' || charac == '&' || charac == '<' || 
			charac == '>' || charac == ':' || charac == '+' ||
			charac == '-' || charac == '!');

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
	return (charac == '0' || charac == '1' || charac == '2' || charac == '3' || charac == '4' || 
			charac == '5' || charac == '6' || charac == '7' || charac == '8' || charac == '9');
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

	return (unit.size() > 1 && 
			unit.at(0) == '\"' && 
			unit.at(unit.size()-1) == '\"');
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

	return 0;
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

lexical_unit* newLexicalUnit(string chaine, int action, AtomType type, Code code){

	lexical_unit* currentLexical = new lexical_unit();

	currentLexical->chaine = chaine;
	currentLexical->action = action; // 0 mean no action
	currentLexical->type = type;
	currentLexical->code = code;

	if(type == Terminal){
		lexicalTerminal.push_back(currentLexical);
	}else{
		lexicalNonTerminal.push_back(currentLexical);
	}

	return currentLexical;
}

lexical_unit* analyseUnit(string unit){

	int action;
	if (unit != "") {
		if(!isInteger(unit.at(0))){
			action = actionOfLexicalUnit(unit);
			// find the action of a string if there is one (-2 for an error, -1 for a unit without code and the code otherwise)

			if(isString(unit)){
				return newLexicalUnit(unit, 0, Terminal, ELTER);
			}else if (action == 0){
				if (unit.at(0) == '\"'){
					lexicalErrors.push_back(unit);
				}else{
					return newLexicalUnit(unit, action, NonTerminal, IDNTER);
				}
			}else if (action == -2){
				lexicalErrors.push_back(unit);
			}else{
				return newLexicalUnit(rmActionFromLexicalUnit(unit), action, NonTerminal, IDNTER);
			}
		}else if(isNumber(unit)){
			return newLexicalUnit(unit, 0, Terminal, ELTER);
		}else{
			lexicalErrors.push_back(unit);
		}
	}

	return NULL;
}

lexical_unit* scanner(){

	cout << "i have to scan this : " << toScan.substr(scanIterator, toScan.size()) << endl;

	string unit = "";
	char currentChar;
	string doubleSymbol, newDoubleSymbol;
	bool isFillingString = false;
	int nbSeparators = 0;
	unsigned int j;

	for(int i = scanIterator ; i < toScan.size() ; ++i){
		currentChar = toScan.at(i);
		cout << "currentchar is : " << currentChar << endl;
		// Construction d'une chaine de caractère
		if(currentChar == '\"'){

			isFillingString = !isFillingString;

			if(!isFillingString){
				unit += currentChar;
				scanIterator += unit.size() + nbSeparators;
				return analyseUnit(unit);
			}
		}

		if(!isFillingString){
			if(isSymbol(currentChar) || isSeparator(currentChar)){
		cout << "unit is : " << unit << endl;

				if (isPotentialySymbolDouble(currentChar) && (i != toScan.size()-1)){
					if (isSymbolDouble(currentChar, toScan.at(i+1))){
						doubleSymbol = currentChar;
						doubleSymbol += toScan.at(i+1);

						if(unit != "" && unit.at(0) == '\"'){

							if(toScan.at(i+2) == '\"'){
								newDoubleSymbol = "\"" + doubleSymbol +"\"";
								scanIterator += newDoubleSymbol.size() + nbSeparators;
								return newLexicalUnit(newDoubleSymbol, 0, Terminal, ELTER);
							}else{
								newDoubleSymbol = "\"" + doubleSymbol;
								scanIterator += newDoubleSymbol.size() + nbSeparators;
								lexicalErrors.push_back(newDoubleSymbol);
								return NULL;
							}
						}else{
							scanIterator += doubleSymbol.size() + nbSeparators;
							return newLexicalUnit(doubleSymbol, 0, Terminal, ELTER);	// if the double symbol isn't a string then we add it as a NonTerminal
						}

					}else{
						symbols.push_back(string(1,currentChar)); // Cast Char to String				}
						scanIterator = i+1;
						return newLexicalUnit(string(1,currentChar), 0, Terminal, ELTER);
					}
				}else if (isSymbol(currentChar)){
					symbols.push_back(string(1,currentChar)); // Cast Char to String
					scanIterator = i+1;
					return newLexicalUnit(string(1,currentChar), 0, Terminal, ELTER);

				}else if (isSeparator(currentChar)){
					j = i;
					while(isSeparator(toScan.at(j))){
						separators.push_back(currentChar);
						nbSeparators++;
						++j;
					}
				}

				scanIterator += unit.size() + nbSeparators;
				return analyseUnit(unit);

			}else{
				unit += currentChar;
			}
		}else{
			unit += currentChar;
		}

	}

	scanIterator += unit.size() + nbSeparators;
	return analyseUnit(unit); // analyse the last unit found
}


void printLexicalUnits(string lexicalType){

	vector<lexical_unit*> listLexical;

	if(lexicalType == "lexicalTerminal"){
		listLexical = lexicalTerminal;
	}else if("lexicalNonTerminal"){
		listLexical = lexicalNonTerminal;
	}

	for (int i = 0; i < listLexical.size(); ++i){
		cout << "----------------" << endl;

		cout << "Chaine : " << listLexical[i]->chaine << endl;
		cout << "Action : " << listLexical[i]->action << endl;
		cout << "type : " << listLexical[i]->type << endl;
		cout << "Code : " << listLexical[i]->code << endl;

	}
}

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


// ------------------------------------------
// Analyseur

bool callAnalyzer(node* ptr){

	// init the global string toScan from the file
	putFileIntoString("GPL.txt");

	// scan the first lexical unit
	lexical_unit* lu;

	do{
		lu = scanner();
	}while(lu == NULL);

	forest = buildForest();

	return analyzer(ptr, lu);

}

bool analyzer(node* ptr, lexical_unit* lu){

	switch(ptr->op){

		case Conc:
			if(analyzer(ptr->typeNode.conc->left, lu)){
				return analyzer(ptr->typeNode.conc->right, lu);
			}else{
				return false;
			}
		break;

		case Union:
			if (analyzer(ptr->typeNode.unio->left, lu)){
				return true;
			}else{
				return analyzer(ptr->typeNode.unio->right, lu);
			}
		break;

		case Star:
			return analyzer(ptr->typeNode.star->son, lu);	
		break;

		case Un:
			return analyzer(ptr->typeNode.un->son, lu);
		break;

		case Atom:
			switch(ptr->typeNode.atom->type){

				case Terminal:
					if(ptr->typeNode.atom->code == lu->chaine){
						if (ptr->typeNode.atom->action != 0){
							G0Action(ptr->typeNode.atom->action);
							do{
								lu = scanner();
							}while(lu == NULL);
						}else{
							return false;
						}
					} 
				break;

				case NonTerminal:
					if (analyzer(forest[ptr->typeNode.atom->code], lu)){
						if (ptr->typeNode.atom->action != 0){
							G0Action(ptr->typeNode.atom->action);
						}
						return true;
					}else{
						return false;
					}
				break;
			}
		break;
	}

	return false; // probably to remove in the future
}

void G0Action(int action){
	node* T1;
	node* T2;

	switch(action){

		case 1:
			T1 = unstack();
			T2 = unstack();
			// what's this ?
			// forest[T2->code + 5] = T1;
		break;

		case 2:
			// what's this ? I'm probably wrong
			stack(genAtom(lexicalTerminal[0]->chaine, 
						  lexicalTerminal[0]->action, 
						  lexicalTerminal[0]->type));
		break;

		case 3:			
			T1 = unstack();
			T2 = unstack();
			stack(genUnion(T1,T2));
		break;

		case 4:
			T1 = unstack();
			T2 = unstack();
			stack(genConc(T1,T2));
		break;

		case 5:
			// what's this ?
			// if()
		break;

		case 6:
			T1 = unstack();
			stack(genStar(T1));
		break;

		case 7:
			T1 = unstack();
			stack(genUn(T1));
		break;
	}
}

void stack(node* tree){
	pile.push_back(tree);
}

node* unstack(){
	node* lastNode = pile[pile.size()-1];
	pile.pop_back();

	return lastNode;
}