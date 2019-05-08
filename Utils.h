#ifndef UTILS_H
#define UTILS_H

#include "Token.h"

int yywrap();
void printValidTokenToOutputFile(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine);
void printInvalidTokenToConsole(char* lexeme, int numOfLine);
int lexCheck(char* inputFile, char* outputFile);
void makeCurrentNodeToTheFirstNodeAndCurrentIndexToZero();

#endif