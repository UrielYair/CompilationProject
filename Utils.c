#include "Utils.h"
#include "Parser.h"
#include <string.h>



int yywrap() { return 1; }

void printValidTokenToOutputFile(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine)
{
	fprintf(outputFile, "Token of kind %s was found at line : %u, lexeme : %s.\n", tokenToString(kind), numOfLine, lexeme);
}

void printInvalidTokenToConsole(char* lexeme, int numOfLine)
{
	printf("The character %c at line : %u does not begin any legal token in the language.\n", *lexeme, numOfLine);
}

int parseInputFile(char* inputFile, char* lexicalOutputFileName, char* syntacticOutputFileName)
{
	int lineNumber = 1;

	// Openin files for input: 
	yyin = fopen(inputFile, "r");
	
	// I/O Validation:
	if (yyin == NULL) {
		printf("Error! opening input file failed.\n");
		return 1;
	}

	// Openin files for lexical output:
	yyout = fopen(lexicalOutputFileName, "w");

	if (yyout == NULL) {
		printf("Error! creating output file for lexical analyzing failed.\n");
		fclose(yyin);
		return 1;
	}
	
	// Openin files for syntactic output:
	FILE *syntacticOutput = fopen(syntacticOutputFileName, "w");
		
	if (syntacticOutput == NULL) {
		printf("Error! creating output file for syntactical analyzing failed.\n");
		fclose(yyin);
		fclose(yyout);
		return 1;
	}

	// Parseing input file:
	yylex(); //in order to save the first token in memory.
	parse_PROGRAM(syntacticOutput);
	match(TOKEN_END_OF_FILE);

	//free nodes and memory:
	// **** free free free ****
	// TODO: implement free memory method.

	// Files closing:
	fclose(yyin);
	fclose(yyout);
	fclose(syntacticOutput);
	return 0;
}

