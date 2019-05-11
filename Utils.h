#ifndef UTILS_H
#define UTILS_H

#include "Token.h"

int yywrap();
void printValidTokenToOutputFile(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine);
void printInvalidTokenToConsole(char* lexeme, int numOfLine);
int parseInputFile(char* inputFile, char* outputFile);


#endif