#ifndef __PCODEINTERPRETER_HPP__
#define __PCODEINTERPRETER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include "../Analyzer/tools.hpp"

using namespace std;




void initPile(vector<int> pcodepile, vector<int> pileXpile);
void execPcode();
void interpret(int instr);

#endif // __PCODEINTERPRETER_HPP__
