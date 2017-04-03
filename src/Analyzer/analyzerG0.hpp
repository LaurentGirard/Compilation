#ifndef __ANALYZERGO_HPP__
#define __ANALYZERGO_HPP__

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

#endif // __ANALYZERGO_HPP__
