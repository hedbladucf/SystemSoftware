// Name: Luis Duque and Oscar Hedblad
// PIDs: L2946242 and O3415424
// System Software COP 3402
// Instructor: Euripides Montagne
// Program 3: Parser and Code Generator

// Constants
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define MAX_SYMBOL_TABLE_SIZE 20

typedef enum
{
	nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
	multsym = 6,  slashsym = 7, oddsym = 8,  eqlsym = 9, neqsym = 10,
	lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15,
	rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19,
	becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
	whilesym = 25, dosym = 26, callsym = 27, constsym = 28, intsym = 29,
	procsym = 30, writesym = 31, readsym = 32, elsesym = 33
} token_type;

typedef enum
{
	LIT = 1, RTN = 2, LOD = 3, STO = 4, CAL = 5, INC = 6, JMP = 7, JPC = 8,
	SIO1 = 9, SIO2 = 10, NEG = 11, ADD = 12, SUB = 13, MUL = 14, DIV = 15,
	ODD = 16, MOD = 17, EQL = 18, NEQ = 19, LSS = 20, LEQ = 21, GTR = 22,
	GEQ = 23
} op_code;


// Structs
typedef struct
{
	char value[11];
	int TokenType;
} RealToken; 


typedef struct instruction{
	int op; // opcode
	int r;	// reg
	int l;	// L
	int m;	// M
}instruction;


typedef struct
{
	int kind; 		// const = 1, var = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value)
	int level; 		// L level
	int addr; 		// M address
} symbol;


RealToken token[MAX_CODE_LENGTH];
instruction code[MAX_CODE_LENGTH];
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

// Parser
int parser();
void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void getToken();
void error(int type);
void storeSymbol(int type, char nam[11], int value);
void emit(int op, int r, int l, int m);
int getIndex(char *target);
void printCode();




// Scanner
int scanner(int l);
int tokenize(char* word, int tokenIndex);
int isSpecialChar(char character);
int isSpace(char a);
void printLexemeWord(int tokeType);

// Virtual Machine
int virtualMachine(int a, int v, int numLines);
void fetch();
void execute();
int base(int l, int base);
void printOpCode(int opCode);
void printInstructions();
void printExecution();