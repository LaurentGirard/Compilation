#ifndef __ANALYZER_HPP__
#define __ANALYZER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include "tools.hpp"

using namespace std;

lexical_unit* newLexicalUnit(string chaine, int action, AtomType type, Code code);
lexical_unit* analyseUnit(string unit);
lexical_unit* scanner();
void printLexicalUnit(lexical_unit* lu);
void printLexicalUnits(string lexicalType);
void printLexicalErrors();

bool callAnalyzer(map<string,node*> &forest);
bool analyzer(node* ptr, map<string,node*> &forest);
void G0Action(int action, map<string,node*> &forest);

//-------------------
// GPL Part

lexical_unit* newLexicalUnitGPL(string chaine, int action, AtomType type, Code code);
lexical_unit* analyseUnitGPL(string unit);
lexical_unit* scannerGPL();

void printLexicalErrorsGPL();

bool callAnalyzerGPL(map<string,node*> forest);
bool analyzerGPL(node* ptr, map<string,node*> forest);
void GPLAction(int action);
bool isIdentificator(string unit);

#endif // __ANALYZER_HPP__
