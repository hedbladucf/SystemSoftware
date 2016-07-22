// Name: Luis Duque and Oscar Hedblad
// PIDs: L2946242 and O3415424
// System Software COP 3402
// Instructor: Euripides Montagne
// Program 3: Parser and Code Generator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

// Declare functions
int tokenize(char* word, int tokenIndex);
int isSpecialChar(char character);
int isSpace(char a);
int scanner(int l);
void printLexemeWord(int tokeType);

// Declare global variables
int TokenIndex = 0;
int numTokens = 0;
int isIdent = 0;
int comma = 0;
int comment = 0;
int tobecomment = 0;

int scanner(int l){

	char filename[20];
	int lengthOfFile = 0;
	char* word = calloc(30, sizeof(char));

	// Read in from file.
	FILE* inputFile;

	printf("Enter file name: \n");
	scanf("%s", filename);
	
	inputFile = fopen(filename, "r");
	
	// Get length of file
	fseek(inputFile, 0, SEEK_END);
	lengthOfFile = ftell(inputFile);
	fseek(inputFile, 0, SEEK_SET);
	
	char* code = calloc(lengthOfFile, sizeof(char));
	
	char character;
	int i = 0;
	int j = 0;
	int returnVal = 0;
	
	// Read in from file and print the contents.
	if (inputFile) {
		printf("Source Program:\n");
		while (fscanf(inputFile, "%c", &character) != EOF) {	
			printf("%c", character);
			code[i] = character;
			i++;
		}
		printf("\n\n");
		
		//printf("\n\nLexeme Table:\n\n");
		//printf("Lexeme      Token Type\n");
		
		for(i=0; i<lengthOfFile; i++){
	
			// Comments
			if (code[i] == '/' && code[i+1] == '*' && comment == 0)
				comment = 1;
			else if (i > 0 && code[i] == '/' && code[i-1] == '*' && comment == 1)
				tobecomment = 1;
			
			//printf("comment = %d\n", comment);
			
			if (comment == 0){
			
				// If White Space...
				if(isSpace(code[i]) == 1){
					word[j] = '\0';
					j=0;
					returnVal = tokenize(word, TokenIndex);
					
					// If there was an invalid symbol..
					if(returnVal == 1)	
						return 0;
					
					// If token was accepted
					//if(returnVal == 0)
						//printf("%-12s%d\n", token[TokenIndex-1].value, token[TokenIndex-1].TokenType);
					
				}
				// If Special Character and next character isn't...
				else if (isSpecialChar(code[i]) == 1 && isSpecialChar(code[i+1]) == 0){
					word[j] = code[i];
					word[j+1] = '\0';
					j=0;
					returnVal = tokenize(word, TokenIndex);
					if(returnVal == 1)	
						return 0;
					//if(returnVal == 0)
						//printf("%-12s%d\n", token[TokenIndex-1].value, token[TokenIndex-1].TokenType);
				}
				
				// If Special Character and next character is...
				else if (isSpecialChar(code[i]) == 1 && isSpecialChar(code[i+1]) == 1){
					
					//  Second char of Two-Character Specifier
					if ((word[0] == ':' && code[i] == '=') || (word[0] == '<' && code[i] == '>')
						|| (word[0] == '<' && code[i] == '=') || (word[0] == '>' && code[i] == '=')){
					
						word[j] = code[i];
						word[j+1] = '\0';
						j=0;
						returnVal = tokenize(word, TokenIndex);
						if(returnVal == 1)	
							return 0;
						//if(returnVal == 0)
							//printf("%-12s%d\n", token[TokenIndex-1].value, token[TokenIndex-1].TokenType);					
					}
					// One-Character Specifier
					else if (code[i] != ':' && code[i] != '<' && code[i] != '>') {
						word[j] = code[i];
						word[j+1] = '\0';
						j=0;
						returnVal = tokenize(word, TokenIndex);
						if(returnVal == 1)	
							return 0;
						//if(returnVal == 0)
							//printf("%-12s%d\n", token[TokenIndex-1].value, token[TokenIndex-1].TokenType);					
					}
					
					// First char of possible two-Character Specifier
					else {
						word[j] = code[i];
						j++;
					}
				}
				
				// If Neither are special characters...
				else if (isSpecialChar(code[i]) == 0 && isSpecialChar(code[i+1]) == 0){
					word[j] = code[i];
					j++;
				}
				
				// If is not Special character but next is...
				else if (isSpecialChar(code[i]) == 0 && isSpecialChar(code[i+1]) == 1){
					word[j] = code[i];
					word[j+1] = '\0';
					j=0;
					returnVal = tokenize(word, TokenIndex);
					if(returnVal == 1)	
						return 0;
					//if(returnVal == 0)
						//printf("%-12s%d\n", token[TokenIndex-1].value, token[TokenIndex-1].TokenType);
				}
			}
			if (tobecomment == 1){
				comment = 0;
				tobecomment = 0;
			}
		}
		
		if (l == 1){
			// Print Lexeme List
			printf("Lexeme List:\n");
			for (i = 0; i < numTokens; i++){
				printf("%d ", token[i].TokenType);
				if(token[i].TokenType == 2 || token[i].TokenType == 3)
					printf("%s ", token[i].value);
			}
			printf("\n");
			
			
			printf("\nLexeme List (Words):\n");
			for (i = 0; i < numTokens; i++){
				printLexemeWord(token[i].TokenType);
				if(token[i].TokenType == 2 || token[i].TokenType == 3)
					printf("%s ", token[i].value);
			}
			printf("\n\n");
		}
	}
	
	fclose(inputFile);
	return 0;
}

// Returns 1 if special character, 0 otherwise
int isSpecialChar(char a){
	
	if((a =='+') || (a == '-') || (a == '*') 
	   || (a == '/') || (a == '(') || (a == ')')
	   || (a == '=') || (a == ',') || (a == '.')
	   || (a == '<') || (a == '>') || (a == ';')
	   || (a == ':'))
		return 1;
	return 0;
}	

// Returns 1 if space, 0 otherwise
int isSpace(char a){
	if((a == ' ') || (a == '\n') || (a == '\t') || (a == '\r'))
		return 1;
	return 0;
}

// Sets the token array based on the code taken in
int tokenize(char* word, int tokenIndex){
	
	//printf("word: %s\n", word);
	int t=0;
	int returnValue = 0;
	
	//If the word is not empty
	if (word[0] != '\0'){
		if (isIdent == 1 && isalpha(word[0]) == 0){
			returnValue = 1;
			printf("\nERROR: Invalid Identifier '%s'.\n", word);
		}
		isIdent = 0;
	
		// Reserved Words
		if (strcmp(word, "begin") == 0)
			t = 21;
		else if (strcmp(word, "end") == 0)
			t = 22;
		else if (strcmp(word, "if") == 0)
			t = 23;
		else if (strcmp(word, "then") == 0)
			t = 24;
		else if (strcmp(word, "while") == 0){
			t = 25;
		}
		else if (strcmp(word, "do") == 0)
			t = 26;
		else if (strcmp(word, "call") == 0)
			t = 27;
		else if (strcmp(word, "const") == 0){
			t = 28;
			isIdent = 1;
		}
		else if (strcmp(word, "int") == 0){
			t = 29;
			isIdent = 1;
		}
		else if (strcmp(word, "procedure") == 0)
			t = 30;
		else if (strcmp(word, "write") == 0)
			t = 31;
		else if (strcmp(word, "read") == 0)
			t = 32;
		else if (strcmp(word, "else") == 0)
			t = 33;
		
		
		// Special Symbols
		else if (strcmp(word, "+") == 0)
			t = 4;
		else if (strcmp(word, "-") == 0)
			t = 5;
		else if (strcmp(word, "*") == 0)
			t = 6;
		else if (strcmp(word, "/") == 0)
			t = 7;
		else if (strcmp(word, "odd") == 0)
			t = 8;
		else if (strcmp(word, "=") == 0)
			t = 9;
		else if (strcmp(word, "<>") == 0)
			t = 10;
		else if (strcmp(word, "<") == 0)
			t = 11;
		else if (strcmp(word, "<=") == 0)
			t = 12;
		else if (strcmp(word, ">") == 0)
			t = 13;
		else if (strcmp(word, ">=") == 0)
			t = 14;
		else if (strcmp(word, "(") == 0)
			t = 15;
		else if (strcmp(word, ")") == 0)
			t = 16;
		else if (strcmp(word, ",") == 0){
			t = 17;
			if (comma == 1){
				isIdent = 1;
				comma = 0;
			}
		}
		else if (strcmp(word, ";") == 0)
			t = 18;
		else if (strcmp(word, ".") == 0)
			t = 19;
		else if (strcmp(word, ":=") == 0)
			t = 20;
		
		
		// Identifiers
		else if (isalpha(word[0]) != 0){
			t = 2;
			// Check if word is too long
			if (word[11] != '\0'){
				returnValue = 1;
				printf("\nERROR: Identifier Name Too Long.\nTerminating.");
			}
			comma = 1;
		}
		// Numbers
		else if (isdigit(word[0]) != 0){
			t = 3;
			// Check if number is too long
			if (word[5] != '\0'){
				returnValue = 1;
				printf("\nERROR: Number Too Long.\nTerminating.");
			}
			comma = 1;
		}
		
		else if (returnValue != 1) {
			printf("\nERROR: Invalid Symbol Detected.\nTerminating.");
			returnValue = 1;
		}
		// Set Token Array
		strcpy(token[TokenIndex].value, word);
		token[TokenIndex].TokenType = t;
		numTokens++;
		TokenIndex++;
	}
	else returnValue = 2;
	return returnValue;
}

void printLexemeWord(int tokeType){
	
	if (tokeType == 1) printf("nulsym ");
	if (tokeType == 2) printf("identsym ");
	if (tokeType == 3) printf("numbersym ");
	if (tokeType == 4) printf("plussym ");
	if (tokeType == 5) printf("minussym ");
	if (tokeType == 6) printf("multsym ");
	if (tokeType == 7) printf("slashsym ");
	if (tokeType == 8) printf("oddsym ");
	if (tokeType == 9) printf("eqlsym ");
	if (tokeType == 10) printf("neqsym ");
	if (tokeType == 11) printf("lessym ");
	if (tokeType == 12) printf("leqsym");
	if (tokeType == 13) printf("gtrsym ");
	if (tokeType == 14) printf("geqsym ");
	if (tokeType == 15) printf("lparentsym ");
	if (tokeType == 16) printf("rparentsym ");
	if (tokeType == 17) printf("commasym ");
	if (tokeType == 18) printf("semicolonsym ");
	if (tokeType == 19) printf("periodsym ");
	if (tokeType == 20) printf("becomessym ");
	if (tokeType == 21) printf("beginsym ");
	if (tokeType == 22) printf("endsym ");
	if (tokeType == 23) printf("ifsym ");
	if (tokeType == 24) printf("thensym ");
	if (tokeType == 25) printf("whilesym ");
	if (tokeType == 26) printf("dosym ");
	if (tokeType == 27) printf("callsym ");
	if (tokeType == 28) printf("constsym ");
	if (tokeType == 29) printf("intsym ");
	if (tokeType == 30) printf("procsym ");
	if (tokeType == 31) printf("writesym ");
	if (tokeType == 32) printf("readsym ");
	if (tokeType == 33) printf("elsesym ");
	
}