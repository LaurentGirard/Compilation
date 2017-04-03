#include <iostream>
#include <string>

#include "Analyzer/analyzerG0.hpp"

int main(){

	map<string,node*> forest = buildForest();
	
	bool analyzer = callAnalyzer(forest);

	printForest(forest);

	cout << "Result : " << analyzer << endl;
	cout << "------------------------------------" << endl;
	
	// cout << "Lexical units Terminal" << endl;
	// printLexicalUnits("lexicalTerminal");

	// cout << "------------------------------------" << endl;
	// cout << "Lexical units NonTerminal" << endl;
	// printLexicalUnits("lexicalNonTerminal");

	// printSymbols();

	// printLexicalErrors();

	return 0;
}
