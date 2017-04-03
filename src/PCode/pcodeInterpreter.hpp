#ifndef __PCODEINTERPRETER_HPP__
#define __PCODEINTERPRETER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>

using namespace std;

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
static const int DEL = 26;
/*----OperatorsLOG---*/
static const int AND = 27;
static const int OR = 28;
static const int NOT = 29;


void initPile(vector<int> pcodepile, vector<int> pileXpile);
void execPcode();
void interpret(int instr);

#endif // __PCODEINTERPRETER_HPP__
