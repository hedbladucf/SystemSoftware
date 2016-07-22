// Name: Luis Duque and Oscar Hedblad
// PIDs: L2946242 and O3415424
// System Software COP 3402
// Instructor: Euripides Montagne
// Program 3: Parser and Code Generator

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

// Defines
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

// Declare Functions
void fetch();
void execute();
int base(int l, int base);
void printOpCode(int opCode);
void printInstructions();
void printExecution(int v);
int virtualMachine(int a, int v, int numLines);

// Declare and set sp, bp, pc, and ir.
int sp = 0;
int bp = 1;
int pc = 0;
instruction ir;

// Declare variables for number of instructions and several PC's
int numInstructions=0;
int oldPc = 0;
int realPc = 0;
int vMac = 0;
// Create stack, register array, and instruction array.
int* stack;
int* reg;

int virtualMachine(int a, int v, int numLines) 
{
	numInstructions = numLines;
	vMac = v;
	stack = calloc(MAX_STACK_HEIGHT, sizeof(int));
	reg = calloc(16, sizeof(int));
	
	//char filename[MAX_CODE_LENGTH];
//	
//	// Read in from file.
//	FILE* inputFile;
//	
//	printf("Enter file name: \n");
//	scanf("%s", filename);
//	
//	inputFile = fopen(filename, "r");
	
//	// If file exists, fill in the "code" array.
//	if (inputFile) {
//		int opCode, rCode, lCode, mCode;
//		int i = 0;
//		while (fscanf(inputFile, "%d", &opCode) != EOF) {
//			if (i > MAX_CODE_LENGTH){
//				printf("The file exceeds the Maximum Code Length of 500.\n");
//			}
//			fscanf(inputFile, "%d", &rCode);
//			fscanf(inputFile, "%d", &lCode);
//			fscanf(inputFile, "%d", &mCode);
//			code[i].op = opCode;
//			code[i].r = rCode;
//			code[i].l = lCode;
//			code[i].m = mCode;
//			i++;
//			numInstructions++;
//			
//		}
//	}
	
	
	if (a == 1)
	printInstructions();
	printf("Initial Values\t\t\t\tpc\tbp\tsp\n");

	// For loop to fetch, execute, and print the instructions.
	while(bp != 0)
	{
		fetch();
		
		// Set PC to the previous PC
		realPc = pc - 1;
		execute();
		
		// Set PC for cases where PC is changed (JMP, JPC, RTN, CAL
		if ((ir.op == 2) || (ir.op == 5) || (ir.op == 7)) {
			realPc = oldPc;
		}
		printExecution(vMac);
    }
	return 0;
}

// Fetch Cycle Function
void fetch(){
	ir = code[pc];
	pc++;
}

// Execute Cycle Function
void execute(){
	// Switch statement for different operations
	switch(ir.op) 
	{
		case 1: //LIT
			reg[ir.r] = ir.m;
			break;
		case 2: //RTN
			sp = bp - 1;
			bp = stack[sp +3];
			oldPc = pc-1;
			pc = stack[sp+4];
			break;
		case 3: //LOD
			reg[ir.r] = stack[base(ir.l, bp) + ir.m];		
			break;
		case 4: // STO
			stack[base(ir.l, bp) + ir.m] = reg[ir.r];
			break;
		case 5: //CAL
			stack[sp+1] = 0;	// Space to return value
			stack[sp+2] = base(ir.l, bp);	// Static Link
			stack[sp+3] = bp;	// Dynamic Link
			stack[sp+4] = pc;	// Return address
			bp = sp+1;
			oldPc = pc-1;
			pc = ir.m;
			break;
		case 6: //INC
			sp = sp + ir.m;
			break;
		case 7: //JMP
			oldPc = pc-1;
			pc = ir.m;
			break;
		case 8: //JPC
			if (reg[ir.r] == 0){
				oldPc = pc-1;
				pc = ir.m;
				realPc = oldPc;
			}
			break;
		case 9: //SIO 1
			printf("\nRegister %d contains %d\n\n", ir.r, reg[ir.r]);;
			break;
		case 10: //SIO 2		
			scanf("%d", &reg[ir.r]);
			break;
		case 11: //NEG	
			reg[ir.r] = reg[ir.l] * -1;
			break;
		case 12: //ADD
			reg[ir.r] = reg[ir.l] + reg[ir.m];
			break;
		case 13: //SUB
			reg[ir.r] = reg[ir.l] - reg[ir.m];
			break;
		case 14: //MUL
			reg[ir.r] = reg[ir.l] * reg[ir.m];
			break;
		case 15: //DIV
			reg[ir.r] = reg[ir.l] / reg[ir.m];
			break;
		case 16: //ODD
			reg[ir.r] = reg[ir.r]%2;
			break;
		case 17: //MOD
			reg[ir.r] = reg[ir.l] % reg[ir.m];
			break;
		case 18: //EQL
			reg[ir.r] = reg[ir.l] == reg[ir.m];
			break;
		case 19: //NEQ
			reg[ir.r] = reg[ir.l] != reg[ir.m];
			break;
		case 20: //LSS
			reg[ir.r] = reg[ir.l] < reg[ir.m];
			break;
		case 21: //LEQ
			reg[ir.r] = reg[ir.l] <= reg[ir.m];
			break;
		case 22: //GTR
			reg[ir.r] = reg[ir.l] > reg[ir.m];
			break;
		case 23: //GEQ
			reg[ir.r] = reg[ir.l] >= reg[ir.m];
			break;
			
		default:
			break;
	}
}

// First part of the output; prints the instructions in the text file.
void printInstructions(){
    printf("Line\tOP\tR\tL\tM\t\n");
    int j = 0;
    for(j = 0; j < numInstructions; j++){
        printf("%d\t",j);
        printOpCode(code[j].op);
        printf("\t%d\t%d\t%d\n", code[j].r, code[j].l, code[j].m);
    } 
    printf("\n\n");
}

// Prints the instruction, pc, bp, sp, and the current stack during execution
void printExecution(int v){
	int k; 
	int max = 3;
	if(sp > 3) max = sp;
	
	if (v==1){

		//printf("Initial Values\t\t\t\tpc\tbp\tsp\n");
		
		printf("%d\t", realPc);
		printOpCode(code[realPc].op);
		printf("\t%d\t%d\t%d\t%d\t%d\t%d\t", code[realPc].r, code[realPc].l, code[realPc].m, pc, bp, sp);
		for(k = 1; k<=max; k++){
			//if (k > 6 && (k%4) == 3)
//				printf("|\t");
			printf("%d\t", stack[k]);
			
		}
		printf("\n");
	}
}

// Simply prints the char representation of the int opCode.
void printOpCode(int opCode){
	
    switch (opCode){
			
		case 1:
			printf("lit");
			break;
		case 2:
			printf("rtn");
			break;
		case 3:
			printf("lod");
			break;
		case 4:
			printf("sto");
			break;
		case 5:
			printf("cal");
			break;
		case 6:
			printf("inc");
			break;
		case 7:
			printf("jmp");
			break;
		case 8:
			printf("jpc");
			break;
		case 9:
			printf("sio");
			break;
		case 10:
			printf("sio");
			break;
		case 11:
			printf("neg");
			break;
		case 12:
			printf("add");
			break;
		case 13:
			printf("sub");
			break;
		case 14:
			printf("mul");
			break;
		case 15:
			printf("div");
			break;
		case 16:
			printf("odd");
			break;
		case 17:
			printf("mod");
			break;
		case 18:
			printf("eql");
			break;
		case 19:
			printf("neq");
			break;
		case 20:
			printf("lss");
			break;
		case 21:
			printf("leq");
			break;
		case 22:
			printf("gtr");
			break;
		case 23:
			printf("geq");
			break;
			
			
		default:
			break;
    }
}

// Helper function to find base L levels down
int base(int l, int base) // l stand for L in the instruction format
{  
	int b1; //find base L levels down
	b1 = base; 
	while (l > 0)
	{
		b1 = stack[b1 + 1];
		l--;
	}
	return b1;
}