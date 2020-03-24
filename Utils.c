#include "Utils.h"
#include "Parser.h"
#include <string.h>

FILE* semanticOutput = NULL;

int yywrap() { return 1; }

void printValidTokenToOutputFile(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine)
{
	fprintf(outputFile, "Token of kind %s was found at line : %u, lexeme : %s.\n", tokenToString(kind), numOfLine, lexeme);
}

void printInvalidTokenToConsole(char* lexeme, int numOfLine)
{
	printf("The character %c at line : %u does not begin any legal token in the language.\n", *lexeme, numOfLine);
	next_token();
}

int parseInputFile(char* inputFile, char* lexicalOutputFileName, char* syntacticOutputFileName, char* semanticOutputFileName)
{
	int lineNumber = 1;

	// Openin files for input: 
	yyin = fopen(inputFile, "r");
	
	// I/O Validation:
	if (yyin == NULL) {
		printf("Error! opening input file failed.\nPress any key to continue\n");
		printf("***********************************************************************************************\n");
		printf("Press any key to continue\n");
		getchar();
		return 1;
	}

	// Openin files for lexical output:
	yyout = fopen(lexicalOutputFileName, "w");

	if (yyout == NULL) {
		printf("Error! creating output file for lexical analyzing failed.\nPress any key to continue\n");
		printf("***********************************************************************************************\n");
		printf("Press any key to continue\n");
		getchar();
		fclose(yyin);
		return 1;
	}
	
	// Openin files for syntactic output:
	FILE *syntacticOutput = fopen(syntacticOutputFileName, "w");
		
	if (syntacticOutput == NULL) {
		printf("Error! creating output file for syntactical analyzing failed.\nPress any key to continue\n");
		printf("***********************************************************************************************\n");
		printf("Press any key to continue\n");
		getchar();
		fclose(yyin);
		fclose(yyout);
		return 1;
	}

	// Openin files for semantic output:
	semanticOutput = fopen(semanticOutputFileName, "w");

	if (semanticOutput == NULL) {
		printf("Error! creating output file for semantic analyzing failed.\nPress any key to continue\n");
		printf("***********************************************************************************************\n");
		printf("Press any key to continue\n");
		getchar();
		fclose(yyin);
		fclose(yyout);
		fclose(syntacticOutput);
		return 1;
	}

	// Parsing input file:
	parse_PROGRAM(syntacticOutput);
	match(TOKEN_END_OF_FILE);

	//free nodes and memory:
	freeMemory();

	// Files closing:
	fclose(yyin);
	fclose(yyout);
	fclose(syntacticOutput);
	fclose(semanticOutput);

	printf("***********************************************************************************************\n");
	printf("Press any key to continue\n");
	getchar();

	return 0;
}

