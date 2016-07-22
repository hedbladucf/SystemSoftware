// Name: Luis Duque and Oscar Hedblad
// PIDs: L2946242 and O3415424
// System Software COP 3402
// Instructor: Euripides Montagne
// Program 4: Compiler

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

// Declare functions
void program();
void block(int level);
void statement(int level);
void condition(int level);
void expression(int level);
void term(int level);
void factor(int level);
void getToken();
void error(int type);
int parser();
void storeSymbol(int type, char nam[11], int value);
void emit(int op, int r, int l, int m);
int getIndex(char *target);
void printCode();


// Declare variables
RealToken current;
int tokeIndex = 0;
int i = 0;
int isComma = 1;
int hasError = 0;
int numSymbols = 0;
int cx = 0;
int isRelational = 0;
int rp = 0;
int ctemp = 0;
int cx1 = 0;
int cx2 = 0;
int numVars = 0;
int stackPtr = 4;
int numLoads = 0;
int OpPtr = 0;
int justNeg =0;
int justAdd = 0;
int numBefore = 0;
int sum = 0;
int sum2 = 0;

int val = 0;
char name[11];

int parser(){
	
	program();
	return cx;
}

void program(){
	
	getToken();
	block(0);
	//printf("%d\n",current.TokenType);
	if (current.TokenType != periodsym){
		error(9);}
	if (hasError == 0)
		printf("No errors, program is syntactically correct.\n\n");
}


void block(int level){
	
	int tempVars;
	int codeIndex;
	int tempStack;
	
	codeIndex = cx;
	tempStack = stackPtr;
	stackPtr = 4;
	tempVars = numVars;
	numVars = 0;
	emit(JMP, 0, 0, 0);
	
	// Constant Declaration
	if (current.TokenType == constsym){
		
		// Keep checking for constants until a non-comma is found.
		while (isComma == 1){
			getToken();
			if (current.TokenType != identsym) error(4);
			else strcpy(name, current.value);
			getToken();
			if (current.TokenType == becomessym) error(1);
			else if (current.TokenType != eqlsym) error(3);
			getToken();
			if (current.TokenType != numbersym) error(2);
			else val = atoi(current.value);
			getToken();
			if (current.TokenType != commasym)
				isComma = 0;
			storeSymbol(1, name, val);
		}
		
		isComma = 1;
		if (current.TokenType != semicolonsym) error(5);
		getToken();
	}
	
	// Variable Declaration
	if (current.TokenType == intsym){
		
		// Keep checking for variables until a non-comma is found.
		while (isComma == 1){
			getToken();
			if (current.TokenType != identsym) error(4);
			else strcpy(name, current.value);
			getToken();
			if (current.TokenType != commasym)
				isComma = 0;
			storeSymbol(2, name, level);
			numVars++;
		}
		
		isComma = 1;
		if (current.TokenType != semicolonsym) error(5);
		getToken();
	}
	
	// Procedure Declaration
	while (current.TokenType == procsym){
		getToken();
		if (current.TokenType != identsym) error(4);
		else {
			strcpy(name, current.value);
			storeSymbol(3, name, level);
		}

		getToken();
		if (current.TokenType != semicolonsym) error(5);
		getToken();
		block(level+1);
		if (current.TokenType != semicolonsym) error(5);
		getToken();
	}
	
	
	code[codeIndex].m = cx;
	emit(INC, 0, 0, 4 + numVars);
	stackPtr = tempStack;
	numVars = tempVars;
	
	statement(level);
	emit(RTN, 0, 0, 0);
}


void statement(int level){
	//printf("STATMENT %d = %d\n", tokeIndex,current.TokenType);
	
	// Identifier
	if (current.TokenType == identsym){
		
		int i;
		
		i = getIndex(current.value);
		if (i == -1) {error(11);	// Undeclared Identifier
			printf("%s Undeclared\n", current.value);

		}
		else if (symbol_table[i].kind != 2) error(12);
		
		getToken();
		//printf("%d\n",current.TokenType);
		if (current.TokenType != becomessym) error(13);
		getToken();
		expression(level);
		emit(STO, rp-1, level - symbol_table[i].level, symbol_table[i].addr);
		rp -= numLoads;
		numLoads = 0;
		justAdd = 0;
		numBefore = 0;
	}
	
	// Call
	else if (current.TokenType == callsym){
		int z;
		
		getToken();
		if (current.TokenType != identsym) error(27);
		z = getIndex(current.value);
		if (z == -1) {error(11);
			printf("Undeclared Procedure\n");
		}
		else if (symbol_table[z].kind == 3)
			emit(CAL, 0, level - symbol_table[z].level, symbol_table[z].addr);
		else error(15);
		getToken();
	}
	
	// Begin
	else if (current.TokenType == beginsym){
		getToken();
		statement(level);
		while (current.TokenType == semicolonsym) {
			getToken();
			statement(level);
		}
		if (current.TokenType != endsym) error(19);
		getToken();
	}
	
	// If
	else if (current.TokenType == ifsym){
		int elseIndex;
		
		getToken();
		condition(level);
		if (current.TokenType != thensym) error(16);
		getToken();
		ctemp = cx;
		emit(JPC, rp-1, 0, 0);
		rp--;/// NOT SURE for now
		//printf("If %d = %d\n", tokeIndex,current.TokenType);
		statement(level);
		if (current.TokenType == elsesym)
		{
			elseIndex = cx;
			code[ctemp].m = cx+1;
			emit(JMP, 0, 0,0);
			getToken();
			
			statement(level);
			code[elseIndex].m = cx;
		}
		else code[ctemp].m = cx;
	}
	

	
	// While
	else if (current.TokenType == whilesym){
		cx1 = cx;
		getToken();
		condition(level);
		cx2 = cx;
		emit(JPC, rp-1, 0, 0);
		rp--;
		if (current.TokenType != dosym) error(18);
		getToken();
		statement(level);
		emit(JMP, 0, 0, cx1);
		code[cx2].m = cx;
	}
	
	// Read
	else if (current.TokenType == readsym){
		getToken();
		if (current.TokenType != identsym) error(27);
		else {
			int i = getIndex(current.value);
			if (i == -1) error(11);
			else {
				emit(SIO2, rp, 0, 2);
				emit(STO, rp, level - symbol_table[i].level, symbol_table[i].addr);
				rp-= numLoads;
				numLoads = 0;
			}
		}

		getToken();
	}
	
	// Write
	else if (current.TokenType == writesym){
		getToken();
		if (current.TokenType != identsym) error(27);
		else {
			int i = getIndex(current.value);
			if (i == -1){ error(11);
			printf("WRITE/n");
			}
			else {
				emit(LOD, rp, level-symbol_table[i].level, symbol_table[i].addr);
				emit(SIO1, rp, 0, 1);
			}
		}
		
		getToken();
	}
	
	else;
		
}

void condition(int level){
	
	int cond;
	int operation;
	
	// Odd
	if (current.TokenType == oddsym){
		getToken();
		expression(level);
		emit(ODD, rp, 0, 0);
		rp++;
	}
	
	// not odd
	else {
		cond = 0;
		expression(level);
		operation = current.TokenType;
		if ((current.TokenType == eqlsym) || (current.TokenType == neqsym) || (current.TokenType == lessym)
			|| (current.TokenType == leqsym) || (current.TokenType == gtrsym) ||
			(current.TokenType == geqsym)) isRelational = 1;
		if (isRelational == 0) error(20);
		isRelational = 0;
		getToken();
		expression(level);
		switch (operation)
		{
			case eqlsym: cond = 18; break;
			case neqsym: cond = 19; break;
			case lessym: cond = 20; break;
			case leqsym: cond = 21; break;
			case gtrsym: cond = 22; break;
			case geqsym: cond = 23; break;
			default: cond = 0; break;
		}
		emit(cond, rp, rp-2, rp-1);		//// NEED TO FIX
		rp++;
	}
}

void expression(int level){
	
	int addop = 0;
	if (current.TokenType == plussym || current.TokenType == minussym){
		addop = current.TokenType;
		getToken();
		term(level);
		if (addop == minussym){
			emit(NEG, rp, rp-1, 0);   // NEGATE   //// NEED TO FIX
			rp++;
			OpPtr++;
			justNeg = 1;
		}
	}
	else term(level);
	while (current.TokenType == plussym || current.TokenType == minussym){
		addop = current.TokenType;
		getToken();
		term(level);
		if (addop == plussym){
			sum = rp-2-OpPtr-numBefore;
			sum2 =rp-2-numBefore;
			if(justNeg ==1)
				emit(ADD, rp, sum, rp-1);	// ADD		   //// NEED TO FIX
			else
				emit(ADD, rp, sum2, rp-1);
			justAdd = 1;
		}
		else {
			sum = rp-2-OpPtr-numBefore;
			sum2 =rp-2-numBefore;
			//printf("numBefore: %d\n", numBefore);
			//printf("sum2: %d\n", sum2);
			if(justNeg ==1)
				emit(SUB, rp, sum, rp-1);	// ADD		   //// NEED TO FIX
			else
				emit(SUB, rp, sum2, rp-1);	// SUBTRACT		   //// NEED TO FIX
			justAdd =1;
		}
		rp++;
		numLoads++;
		OpPtr = 0;
		//printf("JustAdd: %d\n", justAdd);
	}
}

void term(int level){
	
	int mulop = 0;
	factor(level);
	while (current.TokenType == multsym || current.TokenType == slashsym){
		mulop = current.TokenType;
		getToken();
		factor(level);
		if (mulop == multsym){
			if(justNeg ==1)
				emit(MUL, rp, rp-2-OpPtr, rp-1);	// ADD		   //// NEED TO FIX
			else
				emit(MUL, rp, rp-2, rp-1);	// MULTIPLY

		}
		else{
			if(justNeg ==1)
				emit(DIV, rp, rp-2-OpPtr, rp-1);	// ADD		   //// NEED TO FIX
			else
				emit(DIV, rp, rp-2, rp-1);
		}
				// DIVIDE
		rp++;
		numLoads++;
	}
}

void factor(int level){
	
	int i, lev, address, value;
	
	while ((current.TokenType == identsym) || (current.TokenType == numbersym) || (current.TokenType == lparentsym)){
	
		// Identifier
		if (current.TokenType == identsym){
			i = getIndex(current.value);
			if (i == -1) {error(11); // undeclared identifier
				printf("FACTOR\n");
			}
			else
			{
				int kind = symbol_table[i].kind;
				lev = symbol_table[i].level;
				address = symbol_table[i].addr;
				value = symbol_table[i].val;
				if (kind == 1){
					emit(LIT, rp, 0, value);
					//printf("emmitting LIT\n");
					rp++;
					numLoads++;
					justNeg = 0;
					//printf("justAdd: %d\n", justAdd);
					if (justAdd ==1)
						numBefore++;
					//printf("numBeforeRightafter: %d\n", numBefore);
				}
				else if (kind == 2){
					emit(LOD,rp, level - lev, address);
					rp++;
					numLoads++;
					justNeg = 0;
					if (justAdd ==1)
						numBefore++;
				}				
					
				else error(21); // Expression must not contain a procedure identifier
			}			
			getToken();
		}
		// Number
		else if (current.TokenType == numbersym){
			if (atoi(current.value) > 4000)
			{
				error(25); //This number is too large.
				val = 0;
			}
			emit(LIT, rp,0,atoi(current.value));
			rp++;
			numLoads++;
			justNeg = 0;
			if (justAdd ==1)
				numBefore++;
			
			getToken();
		}
		// Parenthesis
		else if (current.TokenType == lparentsym){
			getToken();
			expression(level);
			if (current.TokenType != rparentsym) error(22);
			getToken();
		}
		else error(28);
	}
}



// Gets the next token.
void getToken(){
	
	strcpy(current.value, token[tokeIndex].value);
	current.TokenType = token[tokeIndex].TokenType;	
	tokeIndex++;
	// printf("toke index = %d    toke = %s\n", tokeIndex,current.value);
}

// Error handling
void error(int type) {
	
	hasError = 1;
	printf("ERROR (%d): ", type);
	switch (type)
	{
		case 1:
			printf("Use = instead of :=\".\n");
			break;
		case 2:
			printf("= must be followed by a number.\n");
			break;
		case 3:
			printf("Identifier must be followed by =.\n");
			break;
		case 4:
			printf("const/int must be followed by identifier.\n");
			break;
		case 5:
			printf("Semicolon or comma missing.\n");
			break;
		case 6:
			printf("Incorrect symbol after procedure declaration.\n");
			break;
		case 7:
			printf("Statement expected.\n");
			break;
		case 8:
			printf("Incorrect symbol after statement part in block.\n");
			break;
		case 9:
			printf("Period expected.\n");
			break;
		case 10:
			printf("Semicolon between statements missing.\n");
			break;
		case 11:
			printf("Undeclared identifier.\n");
			break;
		case 12:
			printf("Assignment to constant or procedure is not allowed.\n");
			break;
		case 13:
			printf("Assignment operator expected.\n");
			break;
		case 14:
			printf("Call must be followed by an identifier.\n");
			break;
		case 15:
			printf("Call of a constant or variable is meaningless.\n");
			break;
		case 16:
			printf("then expected.\n");
			break;
		case 17:
			printf("Semicolon or } expected.\n");
			break;
		case 18:
			printf("do expected.\n");
			break;
		case 19:
			printf("end expected.\n");
			break;
		case 20:
			printf("Relational operator expected.\n");
			break;
		case 21:
			printf("Expression must not contain a procedure identifier.\n");
			break;
		case 22:
			printf("Right parenthesis missing.\n");
			break;
		case 23:
			printf("The preceding factor cannot begin with this symbol.\n");
			break;
		case 24:
			printf("An expression cannot begin with this symbol.\n");
			break;
		case 25:
			printf("This number is too large.\n");
			break;
		case 26:
			printf("Left parenthesis missing.\n");
			break;
		case 27:
			printf("Identifier expected.\n");
			break;
		case 28:
			printf("Identifier, (, or number expected.\n");
		case 29:
			printf("No more registers available.\n");
			break;

		default:
			printf("No error specified.\n");
			break;
	}
	exit(0);
}

// Stores symbols in symbol table  *Value can be lex level
void storeSymbol(int type, char nam[11], int value) {
	
	// Store type and name of symbol
	symbol_table[numSymbols].kind = type;
	strcpy(symbol_table[numSymbols].name, name);
	
	// Constants
	if (type == 1){
		symbol_table[numSymbols].val = value;
		//printf("\nsymbol_table[%d]:\nKind = %d\n%s = %d\n\n", numSymbols, symbol_table[numSymbols].kind, symbol_table[numSymbols].name, symbol_table[numSymbols].val );
	}
	// Ints
	if (type == 2){
		symbol_table[numSymbols].level = value;
		symbol_table[numSymbols].addr = stackPtr;
		stackPtr++;
		//printf("\nsymbol_table[%d]:\nKind = %d\nVar name: %s\nLex Level: %d\nAddress: %d\n\n", numSymbols, symbol_table[numSymbols].kind, symbol_table[numSymbols].name, symbol_table[numSymbols].level, symbol_table[numSymbols].addr);

	}
	// Procedures
	if (type == 3){
		symbol_table[numSymbols].level = value;
		symbol_table[numSymbols].addr = cx;
		//printf("\n\nsymbol_table[%d]:\nkind = %d\n%s: L = %d, M = %d\n\n", numSymbols, symbol_table[numSymbols].kind, symbol_table[numSymbols].name, symbol_table[numSymbols].level, symbol_table[numSymbols].addr );
	}
	
	numSymbols++;
}


// Generates code and stores in the code array.
void emit(int op, int r, int l, int m)
{
	if (r > 15) error(29);	// NO more registers available.
	if (cx > MAX_CODE_LENGTH){
		printf("Error: Program is too long!\n");
		exit(0);
	}
	else
	{
		code[cx].op = op;
		code[cx].r = r;
		code[cx].l = l;
		code[cx].m = m;
      
//		printf("THIS IS OP: %d\n", code[cx].op);
//		printf("THIS IS R: %d\n", code[cx].r);
//		printf("THIS IS L: %d\n",  code[cx].l);
//		printf("THIS IS M: %d \n", code[cx].m);
		
	}
	cx++;
}


// Finds a value in the symbol table and returns it location.
// Returns -1 if it does not exist.

int getIndex(char *target)
{
	int i;
	for (i = 0; i < numSymbols; i++)
	{
		if (!strcmp(symbol_table[i].name, target))
			return i;
	}
	return -1;
}


// Prints the generated code
void printCode()
{
	int i;
	for (i = 0; i < cx; i++)
	{
		printf("\n%d %d %d %d\n", code[i].op, code[i].r, code[i].l, code[i].m);
		//fprintf(fout, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
	}
}

