#include "analyzer.hpp"
#include "scanner.hpp"

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
			return analyzer(ptr->typeNode.star->son;
			
		break;

		case Un:
			return analyzer(ptr->typeNode.un->son);
		break;

		case Atom:
			switch(ptr->typeNode.atom->type){

				case Terminal:
					// if(ptr->typeNode.atom->code == code venant du scanner) // A remplir, je vois pas comment utiliser le scanner d'avant pour 
				break;

				case NonTerminal:
					// if analyzer(A[ptr->typeNode.atom->code]){
					//	if (ptr->typeNode.atom->action != 0){
					//		TODO: Function Gaction
					//	}
					// }else{
					// 	return false;
					// }
				break;
			}
		break;
	}

	return false; // à enlever surement
}