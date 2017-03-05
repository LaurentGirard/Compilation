#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include "../enumerations.hpp"

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
void addLexicalUnit(string chaine, int action, AtomType type, Code code);
void analyseUnit(string unit);
void scanner (string text);
void printLexicalUnits(string lexicalType);
void printLexicalErrors();

#endif // __SCANNER_HPP__
