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
string rmQuoteFromLexicalUnit(string unit);

/*-------Loaders----------*/
static const int LDA = 1;
static const int LDV = 2;
static const int LDC = 3;
/*----JumpInstructions----*/
static const int JMP = 4;
static const int JIF = 5;
static const int JSR = 6;
static const int RSR = 7;
/*---In/OutInstructions---*/
static const int RD = 8;
static const int RDLN = 9;
static const int WRT = 10;
static const int WRTLN = 11;
static const int AFF = 12;
static const int STOP = 13;
/*----OperatorsREL-------*/
static const int SUP = 14;
static const int SUPE = 15;
static const int INF = 16;
static const int INFE = 17;
static const int EG = 18;
static const int DIFF = 19;
/*----OperatorsArithm----*/
static const int ADD = 20;
static const int SUB = 21;
static const int DIV = 22;
static const int MULT = 23;
static const int NEG = 24;
static const int INC = 25;
static const int DEC = 26;
/*----OperatorsLOG---*/
static const int AND = 27;
static const int OR = 28;
static const int NOT = 29;

#endif // __TOOLS_HPP__