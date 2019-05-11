#include "Utils.h"
#include "Token.h"
#include <string.h>
#include "token.c"

int yywrap() { return 1; }

void printValidTokenToOutputFile(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine)
{
	fprintf(outputFile, "Token of kind %s was found at line : %u, lexeme : %s.\n", tokenToString(kind), numOfLine, lexeme);
}

void printInvalidTokenToConsole(char* lexeme, int numOfLine)
{
	printf("The character %c at line : %u does not begin any legal token in the language.\n", *lexeme, numOfLine);
}

int lexCheck(char* inputFile, char* outputFile)
{
	// Openin files for input and output:
	yyin = fopen(inputFile, "r");
	
	// I/O Validation:
	if (yyin == NULL) {
		printf("Error! opening input file failed.\n");
		return 1;
	}

	yyout = fopen(outputFile, "w");

	if (yyout == NULL) {
		printf("Error! creating output file failed.\n");
		fclose(yyin);
		return 1;
	}
	
	// Lexical check:
	yylex();

	// Files closing:
	fclose(yyin);
	fclose(yyout);
	return 0;
}

