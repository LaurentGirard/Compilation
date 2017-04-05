#include <iostream>
#include <string>

#include "Analyzer/analyzer.hpp"
#include "PCode/pcodeInterpreter.hpp"

int main(){

	map<string,node*> forest = buildForest();
	
	bool analyzer = callAnalyzer(forest);

	printForest(forest);

	cout << "Result : " << analyzer << endl;
	cout << "------------------------------------" << endl;


	analyzer = callAnalyzerGPL(forest);

	cout << "Result : " << analyzer << endl;
	cout << "------------------------------------" << endl;

	printPCODE();

	/* Exemple du cours pour le petit programme SOMME */

	// cout << "Exemple d'interprétation du pcode avec le programme Somme vu en cours." << endl;
	// int pcodePile[] = {LDA,2,RD,AFF,LDA,1,LDC,0,AFF,LDA,0,LDC,1,AFF,LDV,0,LDV,2,INFE,JIF,37,LDA,1,LDV,1,LDV,0,ADD,AFF,LDA,0,LDV,0,INC,AFF,JMP,14,LDV,1,WRTLN,STOP};
	// int pileXpile[] = {0,0,0};

	// vector<int> pile1 (pcodePile, pcodePile + sizeof(pcodePile) / sizeof(int));
	// vector<int> pile2 (pileXpile, pileXpile + sizeof(pileXpile) / sizeof(int));

	initPile(getPCODE(),getPILEX());
	//initPile(pile1,pile2);
	execPcode();
	cout << "k" << endl;
	

	// cout << "Lexical units Terminal" << endl;
	// printLexicalUnits("lexicalTerminal");

	// cout << "------------------------------------" << endl;
	// cout << "Lexical units NonTerminal" << endl;
	// printLexicalUnits("lexicalNonTerminal");

	// printSymbols();

	// printLexicalErrors();

	return 0;
}
