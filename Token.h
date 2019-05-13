#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern FILE *yyin, *yyout;

typedef enum eTOKENS
{
	/* ID: */
		TOKEN_ID,

	/* Keywords: */
		TOKEN_KW_INTEGER,
		TOKEN_KW_REAL,
		TOKEN_KW_PROGRAM,
		TOKEN_KW_END,
		TOKEN_KW_VOID,
		TOKEN_KW_RETURN,
		TOKEN_END_OF_FILE,
	
	/* Separation Sighns: */
		TOKEN_COMMA,
		TOKEN_SEMICOLON,

	/* Brackets: */
		/* {} */
		TOKEN_OPEN_CURLY_BRACKETS,
		TOKEN_CLOSE_CURLY_BRACKETS,
		/* [] */
		TOKEN_OPEN_SQUARE_BRACKETS,
		TOKEN_CLOSE_SQUARE_BRACKETS,
		/* () */
		TOKEN_OPEN_ROUND_BRACKETS,
		TOKEN_CLOSE_ROUND_BRACKETS,

	/* Arithmetics: */
		TOKEN_ARITHMETIC_MULTIPLICATION,
		TOKEN_ARITHMETIC_DIVISION,
		TOKEN_ARITHMETIC_ASSIGNMENT,

	/* Numbers: */
		TOKEN_INT_NUMBER,
		TOKEN_REAL_NUMBER

	 
	/*
		Examples of tokens, add/change according to your needs.
		
		Alternative way to define tokens:
		#define TOKEN_INTEGER 1
		#define TOKEN_IF 2
		...........
	*/
}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token *next_token();
Token *back_token();

bool match(eTOKENS expectedToken);
const char* tokenToString(enum eTOKENS kind);
eTOKENS handleValidToken(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine);
Token* peekN(Token* current, int numberOfNexts);
Token* getCurrentToken();

#endif