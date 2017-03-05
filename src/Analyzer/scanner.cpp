#include "scanner.hpp"

vector<char> separators;
vector<string> symbols;
vector<string> lexicalErrors;
vector<lexical_unit> lexicalUnits;
vector<lexical_unit> lexicalTerminal;
vector<lexical_unit> lexicalNonTerminal;

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

			if(isString(unit)){
				addLexicalUnit(unit, -1, Terminal, ELTER);
			}else if (action == -1){
				if (unit.at(0) == '\"'){
					lexicalErrors.push_back(unit);
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

void scanner (string text){

	string unit = "";
	char currentChar;
	string doubleSymbol, newDoubleSymbol;
	bool isFillingString = false;

	for(int i = 0 ; i < text.size() ; ++i){
		currentChar = text.at(i);

		// Construction d'une chaine de caractère
		if(currentChar == '\"'){

			if(!isFillingString){
				analyseUnit(unit);
				unit = "";
			}
			isFillingString = !isFillingString;
		}

		if(!isFillingString){

			if(isSymbol(currentChar) || isSeparator(currentChar)){

				if (isPotentialySymbolDouble(currentChar) && (i != text.size()-1)){
					if (isSymbolDouble(currentChar, text.at(i+1))){
						doubleSymbol = currentChar;
						doubleSymbol += text.at(i+1);

						if(unit != "" && unit.at(0) == '\"'){

							if(text.at(i+2) == '\"'){
								newDoubleSymbol = "\"" + doubleSymbol +"\"";
								addLexicalUnit(newDoubleSymbol, -1, Terminal, ELTER);
								i = i + 2;
							}else{
								newDoubleSymbol = "\"" + doubleSymbol;
								lexicalErrors.push_back(newDoubleSymbol);
								++i;
							}
						}else{
							addLexicalUnit(doubleSymbol, -1, NonTerminal, IDNTER);	// if the double symbol isn't a string then we add it as a NonTerminal
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
		}else{
			unit += currentChar;
		}

	}

	analyseUnit(unit); // analyse the last unit found
}

void printLexicalUnits(string lexicalType){

	vector<lexical_unit> listLexical;

	if(lexicalType == "lexicalUnits"){
		listLexical = lexicalUnits;
	}else if(lexicalType == "lexicalTerminal"){
		listLexical = lexicalTerminal;
	}else if("lexicalNonTerminal"){
		listLexical = lexicalNonTerminal;
	}

	for (int i = 0; i < listLexical.size(); ++i){
		cout << "----------------" << endl;

		cout << "Chaine : " << listLexical[i].chaine << endl;
		cout << "Action : " << listLexical[i].action << endl;
		cout << "type : " << listLexical[i].type << endl;
		cout << "Code : " << listLexical[i].code << endl;

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