#include "tools.hpp"

string putFileIntoString(string filename){

	string toScan;

	ifstream in(filename);
	string toString((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

	toScan = toString;

	return toScan;
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
	return (
		charac == '=' || charac == '|' || charac == '&' || 
		charac == '<' || charac == '>' || charac == ':' || 
		charac == '+' || charac == '-' || charac == '!' || 
		charac == '(' || charac == '/');
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
		case '(':
			return(secondChar == '/');
		break;
		case '/':
			return(secondChar == ')');
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

string rmQuoteFromLexicalUnit(string unit){

	return unit.substr(1, unit.size()-2);
}