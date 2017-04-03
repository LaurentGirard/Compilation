#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include "../enumerations.hpp"
#include "../Forest/forest.hpp"

using namespace std;

struct lexical_unit{
	string chaine;
	int action;
	AtomType type;
	string code;
};

string putFileIntoString(string filename);

bool isSeparator(char charac);
bool isSymbol(char charac);
bool isPotentialySymbolDouble(char charac);
bool isSymbolDouble(char firstChar, char secondChar);
bool isInteger(char charac);
bool isNumber(string unit);
bool isString(string unit);
int actionOfLexicalUnit(string unit);
string rmActionFromLexicalUnit(string unit);

void printLexicalUnit(lexical_unit* lu);
void printLexicalUnits(string lexicalType);
void printLexicalErrors();

#endif // __TOOLS_HPP__