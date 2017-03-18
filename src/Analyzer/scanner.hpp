#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include "../enumerations.hpp"
#include "../Forest/forest.hpp"

using namespace std;

struct lexical_unit{
	string chaine;
	int action;
	AtomType type;
	Code code;
};

bool isSeparator(char charac);
bool isSymbol(char charac);
bool isPotentialySymbolDouble(char charac);
bool isSymbolDouble(char firstChar, char secondChar);
bool isInteger(char charac);
bool isNumber(string unit);
bool isString(string unit);
int actionOfLexicalUnit(string unit);
string rmActionFromLexicalUnit(string unit);
lexical_unit* newLexicalUnit(string chaine, int action, AtomType type, Code code);
lexical_unit* analyseUnit(string unit);
lexical_unit* scanner();
void printLexicalUnit(lexical_unit* lu);
void printLexicalUnits(string lexicalType);
void printLexicalErrors();

bool callAnalyzer(node * ptr);
bool analyzer(node* ptr);

#endif // __SCANNER_HPP__
