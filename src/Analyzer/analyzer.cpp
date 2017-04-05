#include "analyzer.hpp"

vector<char> separators;
vector<string> symbols;
vector<string> lexicalErrors;

vector<lexical_unit*> lexicalTerminal;
vector<lexical_unit*> lexicalNonTerminal;

stack<node*> pile;

string toScanGPL;
lexical_unit* lu;

unsigned int scanIterator = 0;
bool isScanFinished = false;


//---------------------------------------------------
// GPL Part

vector<char> separatorsProgram;
vector<string> symbolsProgram;
vector<string> lexicalErrorsProgram;

string toScanProgram;
lexical_unit* luProgram;

unsigned int scanIteratorProgram = 0;
bool isScanFinishedProgram = false;

//----------------------------------------------------

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

				return newLexicalUnit(rmQuoteFromLexicalUnit(rmActionFromLexicalUnit(unit)), action, Terminal, "ELTER");
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

	// cout << "i have to scan this : " << toScanGPL.substr(scanIterator, toScanGPL.size()) << endl;

	string unit = "";
	char currentChar;
	string doubleSymbol, newDoubleSymbol;
	bool isFillingString = false;
	int nbSeparators = 0;
	unsigned int j;
	int action;

	if(toScanGPL.substr(scanIterator, toScanGPL.size()).size() < 1){
		isScanFinished = true;
		return NULL;
	}else{
		for(int i = scanIterator ; i < toScanGPL.size() ; ++i){
			currentChar = toScanGPL.at(i);
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
					if (isPotentialySymbolDouble(currentChar) && (i != toScanGPL.size()-1)){


						if (isSymbolDouble(currentChar, toScanGPL.at(i+1))){
							doubleSymbol = currentChar;
							doubleSymbol += toScanGPL.at(i+1);

							if(unit != "" && unit.at(0) == '\"'){

								if(toScanGPL.at(i+2) == '\"'){
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
						while(isSeparator(toScanGPL.at(j))){
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
bool callAnalyzer(map<string,node*> &forest){

	// init the global string toScanGPL from the file
	toScanGPL = putFileIntoString("GPL.txt");

	// scan the first lexical unit
	do{
		lu = scanner();
	}while(lu == NULL);

	return analyzer(forest["S"], forest);
}

bool analyzer(node* ptr, map<string,node*> &forest){

	cout << "lexical unit : " << lu->chaine << endl;;

	switch(ptr->op){

		case Conc:
			cout << "i'm here Conc" << endl;

			if(analyzer(ptr->typeNode.conc->left, forest)){
				return analyzer(ptr->typeNode.conc->right, forest);
			}else{
				cout << "Conc false" << endl;
				return false;
			}

		break;

		case Union:
			cout << "i'm here Union" << endl;

			if (analyzer(ptr->typeNode.unio->left, forest)){
				return true;
			}else{
				return analyzer(ptr->typeNode.unio->right, forest);
			}
		break;

		case Star:
			cout << "i'm here star" << endl;
			while (analyzer(ptr->typeNode.star->son, forest)){
			}	
			return true;
		break;

		case Un:
			cout << "i'm here Un" << endl;
			if(analyzer(ptr->typeNode.un->son, forest)){
				return analyzer(ptr->typeNode.un->son, forest);
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
							G0Action(ptr->typeNode.atom->action, forest);
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
					if (analyzer(forest[ptr->typeNode.atom->code], forest)){
						if (ptr->typeNode.atom->action != 0){
							cout << "i made an action in NonTerminal" << endl;
							G0Action(ptr->typeNode.atom->action, forest);
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

void G0Action(int action, map<string,node*> &forest){
	node* T1 = new node();
	node* T2 = new node();

	switch(action){

		case 1:
			T1 = pile.top();
			pile.pop();
			T2 = pile.top();
			pile.pop();

			forest[T2->typeNode.atom->code] = T1;
		break;

		case 2:
			pile.push(genAtom(lu->chaine, lu->action, lu->type));
		break;

		case 3:			
			T1 = pile.top();
			pile.pop();
			T2 = pile.top();
			
			pile.pop();
			pile.push(genUnion(T2,T1));
		break;

		case 4:
			T1 = pile.top();
			pile.pop();
			T2 = pile.top();
			pile.pop();

			pile.push(genConc(T2,T1));
		break;

		case 5:
			pile.push(genAtom(lu->chaine, lu->action, lu->type));
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
}

//-------------------------------------------------------------
// GPL Part


lexical_unit* newLexicalUnitGPL(string chaine, int action, AtomType type, string code){

	lexical_unit* currentLexical = new lexical_unit();

	currentLexical->chaine = chaine;
	currentLexical->action = action; // 0 mean no action
	currentLexical->type = type;
	currentLexical->code = code;

	return currentLexical;
}

lexical_unit* analyseUnitGPL(string unit){

	int action;
	if (unit != "") {
		if(!isInteger(unit.at(0))){
			action = actionOfLexicalUnit(unit);
			// find the action of a string if there is one (-2 for an error, 0 for a unit without code and the code otherwise)

			if(isString(unit)){
				cout << "unit : " << unit << " and action is " << action << endl;

				return newLexicalUnitGPL(rmActionFromLexicalUnit(unit), action, Terminal, "ELTER");
			}else if (action == 0){
				if (unit.at(0) == '\"'){
					lexicalErrorsProgram.push_back(unit);
				}else if(isIdentificator(unit)){
					return newLexicalUnitGPL(unit, action, NonTerminal, unit);
				}else{
					return newLexicalUnitGPL(unit, action, NonTerminal, "IDENT");
				}
			}else if (action == -2){
				lexicalErrorsProgram.push_back(unit);
			}else{
				if(isIdentificator(unit)){
					return newLexicalUnitGPL(rmActionFromLexicalUnit(unit), action, NonTerminal, rmActionFromLexicalUnit(unit));
				}else{
					return newLexicalUnitGPL(rmActionFromLexicalUnit(unit), action, NonTerminal, "IDENT");
				}
			}
		}else if(isNumber(unit)){
			return newLexicalUnitGPL(unit, 0, Terminal, unit);
		}else{
			lexicalErrorsProgram.push_back(unit);
		}
	}

	return NULL;
}

lexical_unit* scannerGPL(){

	// cout << "i have to scan this : " << toScanProgram.substr(scanIteratorProgram, toScanProgram.size()) << endl;

	string unit = "";
	char currentChar;
	string doubleSymbol, newDoubleSymbol;
	bool isFillingString = false;
	int nbSeparators = 0;
	unsigned int j;
	int action;

	if(toScanProgram.substr(scanIteratorProgram, toScanProgram.size()).size() < 1){
		isScanFinishedProgram = true;
		return NULL;
	}else{
		for(int i = scanIteratorProgram ; i < toScanProgram.size() ; ++i){
			currentChar = toScanProgram.at(i);
			// Construction d'une chaine de caractère
			if(currentChar == '\"'){

				isFillingString = !isFillingString;

				if(!isFillingString){
					unit += currentChar;
					scanIteratorProgram += unit.size() + nbSeparators;
					return analyseUnitGPL(unit);
				}
			}

			if(!isFillingString){

				if(isSymbol(currentChar) || isSeparator(currentChar)){

					action = actionOfLexicalUnit(unit);
					if (isPotentialySymbolDouble(currentChar) && (i != toScanProgram.size()-1)){

						if (isSymbolDouble(currentChar, toScanProgram.at(i+1))){
							doubleSymbol = currentChar;
							doubleSymbol += toScanProgram.at(i+1);

							if(unit != "" && unit.at(0) == '\"'){

								if(toScanProgram.at(i+2) == '\"'){
									newDoubleSymbol = "\"" + doubleSymbol +"\"";
									scanIteratorProgram += newDoubleSymbol.size() + nbSeparators;
									return newLexicalUnitGPL(rmActionFromLexicalUnit(newDoubleSymbol), action, Terminal, newDoubleSymbol);
								}else{
									newDoubleSymbol = "\"" + doubleSymbol;
									scanIteratorProgram += newDoubleSymbol.size() + nbSeparators;
									lexicalErrorsProgram.push_back(newDoubleSymbol);
									return NULL;
								}
							}else{
								scanIteratorProgram += doubleSymbol.size() + nbSeparators;
								if(doubleSymbol == "->"){
									doubleSymbol = "fleche";
								}
								return newLexicalUnitGPL(rmActionFromLexicalUnit(doubleSymbol), action, Terminal, doubleSymbol);	// if the double symbol isn't a string then we add it as a NonTerminal
							}

						}else{
							symbolsProgram.push_back(string(1,currentChar)); // Cast Char to String				}
							scanIteratorProgram = i+1;
							return newLexicalUnitGPL(rmActionFromLexicalUnit(string(1,currentChar)), action, Terminal, string(1,currentChar));
						}
					}else if (isSymbol(currentChar)){
						symbolsProgram.push_back(string(1,currentChar)); // Cast Char to String
						scanIteratorProgram = i+1;
						return newLexicalUnitGPL(rmActionFromLexicalUnit(string(1,currentChar)), action, Terminal, string(1,currentChar));

					}else if (isSeparator(currentChar)){
						j = i;
						while(isSeparator(toScanProgram.at(j))){
							separatorsProgram.push_back(currentChar);
							nbSeparators++;
							++j;
						}
					}

					scanIteratorProgram += unit.size() + nbSeparators;
					return analyseUnitGPL(unit);

				}else{
					unit += currentChar;
				}
			}else{
				unit += currentChar;
			}

		}

		scanIteratorProgram += unit.size() + nbSeparators;
		return analyseUnitGPL(unit); // analyse the last unit found
	}

	return NULL;
}

void printSymbolsGPL(){
	cout << "List of symbols :" << endl;
	for (int i = 0; i < symbolsProgram.size(); ++i){
		cout << symbolsProgram[i] << endl;
	}
}

void printLexicalErrorsGPL(){
	cout << "List of lexical errors :" << endl;
	for (int i = 0; i < lexicalErrorsProgram.size(); ++i){
		cout << lexicalErrorsProgram[i] << endl;
	}
}


// ------------------------------------------
// Analyseur
bool callAnalyzerGPL(map<string,node*> forest){


	// init the global string toScanProgram from the file
	toScanProgram = putFileIntoString("Program.txt");

	// scan the first lexical unit
	do{
		luProgram = scannerGPL();
	}while(luProgram == NULL);

	return analyzerGPL(forest["Prgm"], forest);


}

bool analyzerGPL(node* ptr, map<string,node*> forest){

	cout << "lexical unit : " << luProgram->chaine << endl;;

	switch(ptr->op){

		case Conc:
			cout << "i'm here Conc" << endl;

			if(analyzerGPL(ptr->typeNode.conc->left, forest)){
				return analyzerGPL(ptr->typeNode.conc->right, forest);
			}else{
				cout << "Conc false" << endl;
				return false;
			}

		break;

		case Union:
			cout << "i'm here Union" << endl;

			if (analyzerGPL(ptr->typeNode.unio->left, forest)){
				return true;
			}else{
				return analyzerGPL(ptr->typeNode.unio->right, forest);
			}
		break;

		case Star:
			cout << "i'm here star" << endl;
			while (analyzerGPL(ptr->typeNode.star->son, forest)){
			}	
			return true;
		break;

		case Un:
			cout << "i'm here Un" << endl;
			if(analyzerGPL(ptr->typeNode.un->son, forest)){
				return analyzerGPL(ptr->typeNode.un->son, forest);
			}
			return true;
		break;

		case Atom:
			cout << "i'm here Atom" << endl;
			cout << "actual code : " << ptr->typeNode.atom->code << endl;
			cout << "LUcode : " << luProgram->code << endl;

			switch(ptr->typeNode.atom->type){

				case Terminal:
					if(ptr->typeNode.atom->code == luProgram->code){
						if (ptr->typeNode.atom->action != 0){
							cout << "i made an action in Terminal" << endl;
							// GPLAction(ptr->typeNode.atom->action);
						}
						cout << "Terminal code == LUcode" << endl;
						do{
							luProgram = scannerGPL();
						}while(luProgram == NULL && !isScanFinishedProgram);
						return true;
					}else{
						cout << "Terminal code != LUcode" << endl;
						return false;
					} 
				break;

				case NonTerminal:
					cout << "Non Terminal" << endl;
					if (analyzerGPL(forest[ptr->typeNode.atom->code], forest)){
						if (ptr->typeNode.atom->action != 0){
							cout << "i made an action in NonTerminal" << endl;
							// GPLAction(ptr->typeNode.atom->action);
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

void GPLAction(int action){


}

bool isIdentificator(string unit){

	for(unsigned int i = 0 ; i < lexicalTerminal.size() ; ++i){
		if(lexicalTerminal[i]->chaine == unit){
			return true;
		}
	}

	return false;
}
