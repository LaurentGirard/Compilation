#include <iostream>
#include <string>

#include "Forest/forest.hpp"
#include "Analyzer/scanner.hpp"
#include "Analyzer/analyzer.hpp"

int main(){

	// printForest();

	scanner("1coucou \"\" \"->\" -> rioe54ere \"coucou lol\" dsf \"-> ioehrz#2 \" 654654 iorueko#23k\" ezae");

	cout << "------------------------------------" << endl;
	cout << "All lexical units" << endl;
	printLexicalUnits("lexicalUnits");


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
