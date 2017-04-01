#include "scanner.hpp"
#include "../Forest/forest.hpp"

map<string,node*> forest;

vector<char> separators;
vector<string> symbols;
vector<string> lexicalErrors;

vector<lexical_unit*> lexicalTerminal;
vector<lexical_unit*> lexicalNonTerminal;

stack<node*> pile;

string toScan;
lexical_unit* lu;

unsigned int scanIterator = 0;
bool isScanFinished = false;

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
			charac == '!' || charac == '?' || charac == '.');

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

	if(isString(unit)){
		unit = unit.substr(1, unit.size()-2);
	}

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
	unsigned int endOfCheck = unit.size();

	if(isString(unit)){
		i = 1;
		endOfCheck = unit.size()-1;
		unitWithoutAction += "\"";
	}

	while( (i < endOfCheck) && (unit.at(i) != '#') ){
		unitWithoutAction += unit.at(i);
		++i;
	}

	if(isString(unit)){
		unitWithoutAction += "\"";
	}

	return unitWithoutAction;
}

lexical_unit* newLexicalUnit(string chaine, int action, AtomType type, string code){

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
				cout << "unit : " << unit << " and action is " << action << endl;

				return newLexicalUnit(rmActionFromLexicalUnit(unit), action, Terminal, "ELTER");
			}else if (action == 0){
				if (unit.at(0) == '\"'){
					lexicalErrors.push_back(unit);
				}else{
					return newLexicalUnit(unit, action, NonTerminal, "IDNTER");
				}
			}else if (action == -2){
				lexicalErrors.push_back(unit);
			}else{
				return newLexicalUnit(rmActionFromLexicalUnit(unit), action, NonTerminal, "IDNTER");
			}
		}else if(isNumber(unit)){
			return newLexicalUnit(unit, 0, Terminal, "ELTER");
		}else{
			lexicalErrors.push_back(unit);
		}
	}

	return NULL;
}

lexical_unit* scanner(){

	// cout << "i have to scan this : " << toScan.substr(scanIterator, toScan.size()) << endl;

	string unit = "";
	char currentChar;
	string doubleSymbol, newDoubleSymbol;
	bool isFillingString = false;
	int nbSeparators = 0;
	unsigned int j;
	int action;

	if(toScan.substr(scanIterator, toScan.size()).size() < 1){
		isScanFinished = true;
		return NULL;
	}else{
		for(int i = scanIterator ; i < toScan.size() ; ++i){
			currentChar = toScan.at(i);
			// cout << "currentchar is : " << currentChar << endl;
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

					action = actionOfLexicalUnit(unit);
					if (isPotentialySymbolDouble(currentChar) && (i != toScan.size()-1)){


						if (isSymbolDouble(currentChar, toScan.at(i+1))){
							doubleSymbol = currentChar;
							doubleSymbol += toScan.at(i+1);

							if(unit != "" && unit.at(0) == '\"'){

								if(toScan.at(i+2) == '\"'){
									newDoubleSymbol = "\"" + doubleSymbol +"\"";
									scanIterator += newDoubleSymbol.size() + nbSeparators;
									return newLexicalUnit(rmActionFromLexicalUnit(newDoubleSymbol), action, Terminal, newDoubleSymbol);
								}else{
									newDoubleSymbol = "\"" + doubleSymbol;
									scanIterator += newDoubleSymbol.size() + nbSeparators;
									lexicalErrors.push_back(newDoubleSymbol);
									return NULL;
								}
							}else{
								scanIterator += doubleSymbol.size() + nbSeparators;
								if(doubleSymbol == "->"){
									doubleSymbol = "fleche";
								}
								return newLexicalUnit(rmActionFromLexicalUnit(doubleSymbol), action, Terminal, doubleSymbol);	// if the double symbol isn't a string then we add it as a NonTerminal
							}

						}else{
							symbols.push_back(string(1,currentChar)); // Cast Char to String				}
							scanIterator = i+1;
							return newLexicalUnit(rmActionFromLexicalUnit(string(1,currentChar)), action, Terminal, string(1,currentChar));
						}
					}else if (isSymbol(currentChar)){
						symbols.push_back(string(1,currentChar)); // Cast Char to String
						scanIterator = i+1;
						return newLexicalUnit(rmActionFromLexicalUnit(string(1,currentChar)), action, Terminal, string(1,currentChar));

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

	return NULL;
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

void printSymbols(){
	cout << "List of symbols :" << endl;
	for (int i = 0; i < symbols.size(); ++i){
		cout << symbols[i] << endl;
	}
}

void printLexicalErrors(){
	cout << "List of lexical errors :" << endl;
	for (int i = 0; i < lexicalErrors.size(); ++i){
		cout << lexicalErrors[i] << endl;
	}
}


// ------------------------------------------
// Analyseur

bool callAnalyzer(node* ptr){

	bool resultAnalyzer;
	// init the global string toScan from the file
	putFileIntoString("GPL.txt");
	// putFileIntoString("testGPL.txt");

	// scan the first lexical unit
	do{
		lu = scanner();
	}while(lu == NULL);

	forest = buildForest();

	printForest(forest);
	resultAnalyzer = analyzer(ptr);
	// printForest(forest);

	return resultAnalyzer;
}

bool analyzer(node* ptr){

	cout << "lexical unit : " << lu->chaine << endl;;

	switch(ptr->op){

		case Conc:
			cout << "i'm here Conc" << endl;

			if(analyzer(ptr->typeNode.conc->left)){
				return analyzer(ptr->typeNode.conc->right);
			}else{
				cout << "Conc false" << endl;
				return false;
			}

		break;

		case Union:
			cout << "i'm here Union" << endl;

			if (analyzer(ptr->typeNode.unio->left)){
				return true;
			}else{
				return analyzer(ptr->typeNode.unio->right);
			}
		break;

		case Star:
			cout << "i'm here star" << endl;
			while (analyzer(ptr->typeNode.star->son)){
			}	
			return true;
		break;

		case Un:
			cout << "i'm here Un" << endl;
			if(analyzer(ptr->typeNode.un->son)){
				return analyzer(ptr->typeNode.un->son);
			}
			return true;
		break;

		case Atom:
			cout << "i'm here Atom" << endl;
			cout << "actual code : " << ptr->typeNode.atom->code << endl;

			switch(ptr->typeNode.atom->type){

				case Terminal:
					if(ptr->typeNode.atom->code == lu->code){
						if (ptr->typeNode.atom->action != 0){
							cout << "i made an action in Terminal" << endl;
							G0Action(ptr->typeNode.atom->action);
						}
						cout << "Terminal code == chaine" << endl;
						do{
							lu = scanner();
						}while(lu == NULL && !isScanFinished);
						return true;
					}else{
						cout << "Terminal code != chaine" << endl;
						return false;
					} 
				break;

				case NonTerminal:
					cout << "Non Terminal" << endl;
					if (analyzer(forest[ptr->typeNode.atom->code])){
						if (ptr->typeNode.atom->action != 0){
							cout << "i made an action in NonTerminal" << endl;
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

	return false;
}

void G0Action(int action){
	node* T1 = NULL;
	node* T2 = NULL;
	cout << "action : " << action << endl;

	switch(action){

		case 1:
			T1 = pile.top();
			pile.pop();
			// cout << "code case 1 : " << T1->typeNode.atom->code << endl;
			T2 = pile.top();
			pile.pop();
			forest[T2->typeNode.atom->code] = T1;
		break;

		case 2:
			cout << "code case 2, I pushed : " << lu->chaine << endl;

			pile.push(genAtom(lu->code, lu->action, lu->type));
		break;

		case 3:			
			T1 = pile.top();
			pile.pop();
			T2 = pile.top();
			// cout << "code case 3 : " << T1->typeNode.atom->code << endl;
			// cout << "code case 3 : " << T2->typeNode.atom->code << endl;

			pile.pop();
			pile.push(genUnion(T1,T2));
		break;

		case 4:
			T1 = pile.top();
			pile.pop();
			T2 = pile.top();
			pile.pop();
			// cout << "code case 4 : " << T1->typeNode.atom->code << endl;
			// cout << "code case 4 : " << T2->typeNode.atom->code << endl;

			pile.push(genConc(T1,T2));
		break;

		case 5:
			cout << "code case 5, I pushed : " << lu->chaine << endl;

			pile.push(genAtom(lu->code, lu->action, lu->type));
		break;

		case 6:
			T1 = pile.top();
			pile.pop();
			pile.push(genStar(T1));
		break;

		case 7:
			T1 = pile.top();
			pile.pop();
			pile.push(genUn(T1));
		break;
	}

	delete(T1);
	delete(T2);
}