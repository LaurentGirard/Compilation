#include "forest.hpp"

node* genConc(node* left, node* right){
	node* n = new node();

	n->op = Conc;

	n->typeNode.conc = new type_conc();
	n->typeNode.conc->left = left;
	n->typeNode.conc->right = right;

	return n;
}

node* genUnion(node* left, node* right){
	node* n = new node();

	n->op = Union;

	n->typeNode.unio = new type_union();
	n->typeNode.unio->left = left;
	n->typeNode.unio->right = right;

	return n;
}

node* genStar(node* s){
	node* n = new node();

	n->op = Star;

	n->typeNode.star = new type_star();
	n->typeNode.star->son = s;

	return n;
}

node* genUn(node* s){
	node* n = new node();

	n->op = Un;

	n->typeNode.un = new type_un();
	n->typeNode.un->son = s;

	return n;
}

node* genAtom(string code, int action, AtomType type){
	node* n = new node();

	n->op = Atom;
	n->typeNode.atom = new type_atom();

	n->typeNode.atom->code = code;
	n->typeNode.atom->action = action;
	n->typeNode.atom->type = type;

	return n;
}

/**
* Les atomes doivent être sur les fils droits en priorité pour un affichage correct.
*
*/ 

void printTreeRec(node* n, int cpt){
	string term;
	unsigned int i;

	for( i = 0 ; i < cpt+1 ; ++i ){
		cout << "---";
	}

	switch(n->op){
		case Atom:
			if ( n->typeNode.atom->type == 0 )
				term = "Terminal";
			else
				term = "Non Terminal";

			cout << "> Atom " <<  n->typeNode.atom->code << " " << n->typeNode.atom->action << " " << term << endl;
		break;
		case Conc:
			cout << "> Conc" <<  endl;
			printTreeRec(n->typeNode.conc->right, cpt+1);
			printTreeRec(n->typeNode.conc->left, cpt+1);
		break;
		case Union:
			cout << "> Union" << endl;
			printTreeRec(n->typeNode.unio->right, cpt+1);
			printTreeRec(n->typeNode.unio->left, cpt+1);
		break;
		case Star:
			cout << "> Star" << endl;
			printTreeRec(n->typeNode.star->son, cpt+1);
		break;
		case Un:
			cout << "> Un" << endl;
			printTreeRec(n->typeNode.un->son, cpt+1);
		break;
	}
}

void printTree(node* n){
	
	printTreeRec(n, 0);
}

map<string,node*> buildForest(){

	map<string,node*> forest;

	node* A1 = genConc( genStar( genConc(
		 genConc( genConc( genAtom("N", 0,NonTerminal),
		  genAtom("fleche", 0,Terminal)), genAtom("E",0,NonTerminal)),
		   genAtom(",",1,Terminal))), genAtom(";",0,Terminal) ) ;

	forest["S"] = A1;

	node* A2 = genAtom("IDNTER", 0, Terminal);

	forest["N"] = A2;

	node* A3 = genConc ( genAtom("T",0,NonTerminal), genStar( genConc ( genAtom("+", 0, Terminal), genAtom("T",0,NonTerminal) )));

	forest["E"] = A3;

	node* A4 = genConc ( genAtom("F",0,NonTerminal), genStar( genConc ( genAtom(".", 0, Terminal), genAtom("F",0,NonTerminal) )));

	forest["T"] = A4;

	// node* A5 = genUnion( genUnion( genUnion( genUnion( genConc( genConc( 
	// 				genAtom("(/",0,Terminal),genAtom("E",0,NonTerminal) ),
	// 					genAtom( "/)",0,Terminal)),
	// 						genConc( genConc( genAtom( "[",0,Terminal), genAtom( "E",0,NonTerminal )),
	// 								genAtom( "]",0,Terminal )) ), 
	// 						genConc( genConc( genAtom( "(",0,Terminal), genAtom( "E",0,NonTerminal )),
	// 								genAtom( ")",0,Terminal )) ), genAtom( "ELTER",0,Terminal)) , genAtom( "IDNTER",0,Terminal));

	node* A5 = genUnion(genUnion(genUnion(genUnion(genAtom( "IDNTER",0,Terminal), genAtom( "ELTER",0,Terminal)), genConc(genAtom( "(",0,Terminal),genConc(genAtom( "E",0,NonTerminal), genAtom( ")",0,Terminal)))), 	genConc(genAtom( "[",0,Terminal), genConc(genAtom("E",0,NonTerminal), genAtom( "]",0,Terminal)))), 	genConc(genAtom("(/",0,Terminal), genConc(genAtom("E",0,NonTerminal), genAtom( "/)",0,Terminal))));



	forest["F"] = A5;

	return forest;
}

void printForest(map<string,node*> forest){

	unsigned int i = 1;

	for (map<string,node*>::iterator it=forest.begin(); it!=forest.end(); ++it){
    	cout << "++++++++++ A" << i << " +++++++++"<< endl;
    	printTree(it->second);
    	++i;
    }
}