#include "analyzer.hpp"

bool analyzer(node * ptr){

	switch(ptr->op){

		case Conc:
			if(analyzer(ptr->typeNode.conc->left)){
				return analyzer(ptr->typeNode.conc->right);
			}else{
				return false;
			}
		break;

		case Union:
			if (analyzer(ptr->typeNode.unio->left)){
				return true;
			}else{
				return analyzer(ptr->typeNode.unio->right);
			}
		break;

		case Star:
			while (analyzer(ptr->typeNode.star->son)){

			}
		break;

		case Un:
			if(analyzer(ptr->typeNode.un->son)){

			}
		break;

		case Atom:
			switch(ptr->typeNode.atom->type){

				case Terminal:
					// if(ptr->typeNode.atom->code == code venant du scanner) // A remplir, je vois pas comment utiliser le scanner d'avant pour 
				break;

				case NonTerminal:
					
				break;
			}
		break;
	}

	return false; // à enlever surement
}