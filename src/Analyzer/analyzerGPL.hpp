#ifndef __ANALYZERGPL_HPP__
#define __ANALYZERGPL_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include "tools.hpp"

using namespace std;

lexical_unit* newLexicalUnitGPL(string chaine, int action, AtomType type, Code code);
lexical_unit* analyseUnitGPL(string unit);
lexical_unit* scannerGPL();

void printLexicalUnitsGPL(string lexicalType);
void printLexicalErrorsGPL();

bool callAnalyzerGPL(map<string,node*> &forest);
bool analyzerGPL(node* ptr, map<string,node*> &forest);
void GPLAction(int action, map<string,node*> &forest);

#endif // __ANALYZERGPL_HPP__
