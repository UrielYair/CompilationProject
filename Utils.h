#ifndef UTILS_H
#define UTILS_H

#include "Token.h"

extern FILE *semanticOutput = NULL;

extern int yylex();
int yywrap();
void printValidTokenToOutputFile(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine);
void printInvalidTokenToConsole(char* lexeme, int numOfLine);
int parseInputFile(char* inputFile, char* lexicalOutputFileName, char* syntacticOutputFileName);


#endif