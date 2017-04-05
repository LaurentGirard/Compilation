#include "analyzerGPL.hpp"

vector<char> separatorsProgram;
vector<string> symbolsProgram;
vector<string> lexicalErrorsProgram;

vector<lexical_unit*> lexicalTerminalProgram;
vector<lexical_unit*> lexicalNonTerminalProgram;

stack<node*> pileGPLaction;

string toScanProgram;
lexical_unit* luProgram;

unsigned int scanIteratorProgram = 0;
bool isScanFinishedProgram = false;


lexical_unit* newLexicalUnitGPL(string chaine, int action, AtomType type, string code){

	lexical_unit* currentLexical = new lexical_unit();

	currentLexical->chaine = chaine;
	currentLexical->action = action; // 0 mean no action
	currentLexical->type = type;
	currentLexical->code = code;

	if(type == Terminal){
		lexicalTerminalProgram.push_back(currentLexical);
	}else{
		lexicalNonTerminalProgram.push_back(currentLexical);
	}

	return currentLexical;
}

lexical_unit* analyseUnitGPL(string unit){

	int action;
	if (unit != "") {
		if(!isInteger(unit.at(0))){
			action = actionOfLexicalUnit(unit);
			// find the action of a string if there is one (-2 for an error, -1 for a unit without code and the code otherwise)

			if(isString(unit)){
				cout << "unit : " << unit << " and action is " << action << endl;

				return newLexicalUnitGPL(rmActionFromLexicalUnit(unit), action, Terminal, "ELTER");
			}else if (action == 0){
				if (unit.at(0) == '\"'){
					lexicalErrorsProgram.push_back(unit);
				}else{
					return newLexicalUnitGPL(unit, action, NonTerminal, "IDENT");
				}
			}else if (action == -2){
				lexicalErrorsProgram.push_back(unit);
			}else{
				return newLexicalUnitGPL(rmActionFromLexicalUnit(unit), action, NonTerminal, "IDENT");
			}
		}else if(isNumber(unit)){
			return newLexicalUnitGPL(unit, 0, Terminal, "ENT");
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


void printLexicalUnitsGPL(string lexicalType){

	vector<lexical_unit*> listLexical;

	if(lexicalType == "lexicalTerminalProgram"){
		listLexical = lexicalTerminalProgram;
	}else if("lexicalNonTerminalProgram"){
		listLexical = lexicalNonTerminalProgram;
	}

	for (int i = 0; i < listLexical.size(); ++i){
		cout << "----------------" << endl;

		cout << "Chaine : " << listLexical[i]->chaine << endl;
		cout << "Action : " << listLexical[i]->action << endl;
		cout << "type : " << listLexical[i]->type << endl;
		cout << "Code : " << listLexical[i]->code << endl;

	}
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
bool callAnalyzerGPL(map<string,node*> &forest){

	bool resultAnalyzer;

	// init the global string toScanProgram from the file
	toScanProgram = putFileIntoString("Program.txt");

	// scan the first lexical unit
	do{
		luProgram = scannerGPL();
	}while(luProgram == NULL);

	resultAnalyzer = analyzerGPL(forest["Prgm"], forest);

	return resultAnalyzer;
}

bool analyzerGPL(node* ptr, map<string,node*> &forest){

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

			switch(ptr->typeNode.atom->type){

				case Terminal:
					if(ptr->typeNode.atom->code == luProgram->code){
						if (ptr->typeNode.atom->action != 0){
							cout << "i made an action in Terminal" << endl;
							// GPLAction(ptr->typeNode.atom->action, forest);
						}
						cout << "Terminal code == chaine" << endl;
						do{
							luProgram = scannerGPL();
						}while(luProgram == NULL && !isScanFinishedProgram);
						return true;
					}else{
						cout << "Terminal code != chaine" << endl;
						return false;
					} 
				break;

				case NonTerminal:
					cout << "Non Terminal" << endl;
					if (analyzerGPL(forest[ptr->typeNode.atom->code], forest)){
						if (ptr->typeNode.atom->action != 0){
							cout << "i made an action in NonTerminal" << endl;
							// GPLAction(ptr->typeNode.atom->action, forest);
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

void GPLAction(int action, map<string,node*> &forest){
	// node* T1 = new node();
	// node* T2 = new node();

	// switch(action){

	// 	case 1:
	// 		T1 = pileGPLaction.top();
	// 		pileGPLaction.pop();
	// 		T2 = pileGPLaction.top();
	// 		pileGPLaction.pop();

	// 		forest[T2->typeNode.atom->code] = T1;
	// 	break;

	// 	case 2:
	// 		pileGPLaction.push(genAtom(luProgram->chaine, luProgram->action, luProgram->type));
	// 	break;

	// 	case 3:			
	// 		T1 = pileGPLaction.top();
	// 		pileGPLaction.pop();
	// 		T2 = pileGPLaction.top();
			
	// 		pileGPLaction.pop();
	// 		pileGPLaction.push(genUnion(T2,T1));
	// 	break;

	// 	case 4:
	// 		T1 = pileGPLaction.top();
	// 		pileGPLaction.pop();
	// 		T2 = pileGPLaction.top();
	// 		pileGPLaction.pop();

	// 		pileGPLaction.push(genConc(T2,T1));
	// 	break;

	// 	case 5:
	// 		pileGPLaction.push(genAtom(luProgram->chaine, luProgram->action, luProgram->type));
	// 	break;

	// 	case 6:
	// 		T1 = pileGPLaction.top();
	// 		pileGPLaction.pop();
	// 		pileGPLaction.push(genStar(T1));
	// 	break;

	// 	case 7:
	// 		T1 = pileGPLaction.top();
	// 		pileGPLaction.pop();
	// 		pileGPLaction.push(genUn(T1));
	// 	break;
	// }
}