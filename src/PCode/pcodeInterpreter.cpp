#include "pcodeInterpreter.hpp"

vector<int> pcode;
vector<int> pileX;
int c0;
int spx;

void interpret(int instr) {

	int tmp;

	switch(instr){

/*-------Loaders----------*/
			case LDA: 
				spx = spx + 1;
				pileX[spx] = pcode[c0 + 1];
				c0 = c0 + 2 ;
			break;

			case LDV: 
				spx = spx + 1;
				pileX[spx] = pileX[pcode[c0+1]];
				c0 = c0+2 ;
			break;

			case LDC: 
				spx = spx + 1;
				pileX[spx] = pcode[c0+1];
				c0 = c0+2;
			break;

	/*----JumpInstructions----*/
			case JMP: 
				c0 = pcode[c0+1];
			break;

			case JIF: 
 				if ( pileX[spx] == 0 ){
					c0 = pcode[c0+1];
				} else {
					c0 = c0+2;
				}
				spx = spx -1;

			break;

			case JSR: 
					//TO DO : saut de fonction
			break;

			case RSR: 
					// TO DO 
			break;
	/*---In/OutInstructions---*/
			case RD: 
				spx = spx + 1;
				cin >> tmp;
				pileX[spx] = tmp;
				c0 = c0 +1;
			break;

			case RDLN: 
				spx = spx + 1;
				cin >> tmp;
				cout <<"\n";
				pileX[spx] = tmp;
				c0 = c0 +1;
			break;

			case WRT: 
				tmp = pileX[spx] ;
				cout << tmp;
				spx = spx - 1 ;
				c0 = c0 + 1;
			break;

			case WRTLN: 
				tmp = pileX[spx] ;
				cout << tmp << endl;
				spx = spx - 1 ;
				c0 = c0 + 1;
			break;

			case AFF: 
				pileX[pileX[spx-1]] = pileX[spx];
				spx = spx - 2;
				c0 = c0 + 1;
			break;

			case STOP: 

			break;

	/*----OperatorsREL-------*/
			case SUP: 
				if (pileX[spx-1] > pileX[spx]) {
					pileX[spx-1] = 1;
				} else {
					pileX[spx-1] = 0;
				}
				spx = spx-1;
				c0 = c0+1;
			break;

			case SUPE: 
				if (pileX[spx-1] >= pileX[spx]) {
					pileX[spx-1] = 1;
				} else {
					pileX[spx-1] = 0;
				}
				spx = spx-1;
				c0 = c0+1;
			break;

			case INF: 
				if (pileX[spx-1] < pileX[spx]) {
					pileX[spx-1] = 1;
				} else {
					pileX[spx-1] = 0;
				}
				spx = spx-1;
				c0 = c0+1;
			break;

			case INFE: 
				if (pileX[spx-1] <= pileX[spx]) {
					pileX[spx-1] = 1;
				} else {
					pileX[spx-1] = 0;
				}
				spx = spx-1;
				c0 = c0+1;
			break;

			case EG: 
				if (pileX[spx-1] == pileX[spx]) {
					pileX[spx-1] = 1;
				} else {
					pileX[spx-1] = 0;
				}
				spx = spx-1;
				c0 = c0+1;
			break;

			case DIFF: 
				if (pileX[spx-1] != pileX[spx]) {
					pileX[spx-1] = 1;
				} else {
					pileX[spx-1] = 0;
				}
				spx = spx-1;
				c0 = c0+1;
			break;

	/*----OperatorsArithm----*/
			case ADD: 
				pileX[spx-1] = pileX[spx] + pileX[spx-1];
				spx = spx - 1;
				c0 = c0+1;
			break;

			case SUB: 
				pileX[spx-1] = pileX[spx-1] - pileX[spx];
				spx = spx - 1;
				c0 = c0+1;
			break;

			case DIV: 
				pileX[spx-1] = pileX[spx-1] / pileX[spx];
				spx = spx - 1;
				c0 = c0+1;
			break;

			case MULT: 
				pileX[spx-1] = pileX[spx-1]*pileX[spx];
				spx = spx - 1;
				c0 = c0+1;
			break;

			case NEG: 
				pileX[spx] = - pileX[spx];
				c0 = c0+1;
			break;

			case INC: 
				pileX[spx] = pileX[spx]+1;
				c0 = c0+1;

			break;

			case DEC: 
				pileX[spx] = pileX[spx]-1;
				c0 = c0+1;
			break;

			default: 
				//cout << "In default" << endl;

			break;

	}
}

void execPcode() {

	cout << "Début d'exécution de P_CODE." << endl;

	while (pcode[c0] != 13) {
		interpret(pcode[c0]);
	}

	cout << "Exécution du P_CODE terminée." << endl;
}

void initPile(vector<int> pcodepile, vector<int> pileXpile) {
	pcode = pcodepile;
	pileX = pileXpile;
	c0 = 0;
	spx = pileX.size()-1;

	for(int i = 0 ; i < 100 ; ++i){
		pileX.push_back(-10000);
	}

}
