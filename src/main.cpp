#include <iostream>
#include <string>

#include "Forest/forest.hpp"
#include "Analyzer/scanner.hpp"
#include "Analyzer/analyzer.hpp"

int main(){

	putFileIntoString("GPL.txt");

	// map<string,node*> forest = buildForest();
	// printForest(forest);

	lexical_unit* lu = scanner();
	lu = scanner();
	lu = scanner();
	lu = scanner();
	lu = scanner();
	lu = scanner();
	lu = scanner();
	lu = scanner();
	lu = scanner();
	lu = scanner();


	cout << "------------------------------------" << endl;
	cout << "Lexical units Terminal" << endl;
	printLexicalUnits("lexicalTerminal");

	cout << "------------------------------------" << endl;
	cout << "Lexical units NonTerminal" << endl;
	printLexicalUnits("lexicalNonTerminal");

	// printSymbols();
	// printSeparators();
	printLexicalErrors();

	return 0;
}
