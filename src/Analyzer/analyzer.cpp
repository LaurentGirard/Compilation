// #include "analyzer.hpp"
// #include "scanner.hpp"

// using namespace std;

// unsigned int scanIterator = 0;

// // need it for nom, probably to delete when scanner will change
// bool callAnalyzer(node * ptr, string text){

// 	// scan the entire text (need to change)
// 	scanner(text);

// 	// analyzer will iterate on the global array filled by scanner (will probably change)
// 	return analyzer(ptr, 0);

// }

// bool analyzer(node * ptr){

// 	switch(ptr->op){

// 		case Conc:
// 			if(analyzer(ptr->typeNode.conc->left)){
// 				return analyzer(ptr->typeNode.conc->right);
// 			}else{
// 				return false;
// 			}
// 		break;

// 		case Union:
// 			if (analyzer(ptr->typeNode.unio->left)){
// 				return true;
// 			}else{
// 				return analyzer(ptr->typeNode.unio->right);
// 			}
// 		break;

// 		case Star:
// 			return analyzer(ptr->typeNode.star->son);
			
// 		break;

// 		case Un:
// 			return analyzer(ptr->typeNode.un->son);
// 		break;

// 		case Atom:
// 			switch(ptr->typeNode.atom->type){

// 				case Terminal:
// 					if(ptr->typeNode.atom->code == lexicalUnits[scanIterator]->code){
// 						if (ptr->typeNode.atom->action != 0){
// 							// GoAction sur ptr->typeNode.atom->action
// 							++scanIterator;
// 						}else{
// 							return false;
// 						}
// 					} 
// 				break;

// 				case NonTerminal:
// 					// if analyzer(A[ptr->typeNode.atom->code]){
// 					//	if (ptr->typeNode.atom->action != 0){
// 					//		TODO: Function Gaction
// 					//	}
// 					// }else{
// 					// 	return false;
// 					// }
// 				break;
// 			}
// 		break;
// 	}

// 	return false; // à enlever surement
// }