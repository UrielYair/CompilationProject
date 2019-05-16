#include "Utils.h"
#include "Token.h"
#include <stdbool.h>
#include <stdlib.h>

/* 
This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

#define TOKEN_ARRAY_SIZE 100
int currentIndex = 0;
Node* currentNode = NULL;
int distanceFromLastIdentifiedToken = 0;	// will increase every time back_token() will be called.
											// and decrease when next_token	will be called.
											// the value will always be positive or zero.

/* This function creates a token and stores it in the storage. */
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	
	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);

			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;	
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);
	#ifdef _WIN32
		strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
	#else
		strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	#endif		
}

/* This function returns the token in the storage that is stored immediately before the current token (if exsits). */
Token* back_token() {
	
	// updating distance from last identified token.
	distanceFromLastIdentifiedToken++;
	
	if (currentIndex == 0)
	{
		// Previous node is needed.
		if (currentNode->prev == NULL)
		{
			// Previous node is not exist (the program is on the first node which created).
			printf("There is no previous node in the memory.\n");
			return NULL;
		}
		else
		{
			// Retrieving previous node.
			currentNode = currentNode->prev;
			currentIndex = 99;
			return &(currentNode->tokensArray)[currentIndex];
		}
	}

	else
	{
		// currentIndex value is between 1-99 so previous node is not needed.
		// ******************************************************************
		// Update currentIndex only and return token.
		currentIndex--;
		return &(currentNode->tokensArray)[currentIndex];
	}

}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token* next_token()
{
	if (distanceFromLastIdentifiedToken == 0)
	{
		// next token need to be found by lexical analyzer:
		yylex(); // will find a new valid token and store it. 
	}

	else
	{	
		// Next token already stored and need to be retrieved.
		// ***************************************************

		// updating distance from last identified token.
		distanceFromLastIdentifiedToken--;
		if (distanceFromLastIdentifiedToken < 0)
			distanceFromLastIdentifiedToken = 0;

		if (currentIndex == 99)
		{
			// Next node is needed.
			currentNode = currentNode->next;
			currentIndex = 0;
		}
		else
		{
			currentIndex++;  // Next node is not needed.
		}
				
	}
	return getCurrentToken();	//&(currentNode->tokensArray)[currentIndex];
}

bool match(eTOKENS expectedToken) 
{
	Token* currentToken = next_token();

	if (expectedToken != currentToken->kind)
	{
		printf("Expected token: %s at line %u,\nActual token : %s, lexeme %s.\n" ,
			tokenToString(expectedToken), currentToken->lineNumber, tokenToString(currentToken->kind), currentToken->lexeme);
		return false;
	}
	return true;
}

Token* peekN(Token* current, int numberOfNexts)
{
	int successfulNexts = 0;
	Token* returnToken = NULL;

	for (int i = 0; i < numberOfNexts; i++)
	{
		if (NULL != next_token())
			successfulNexts++;
	}

	if (successfulNexts == numberOfNexts)
	{
		// if none of the next_token() calls returns null, it will be fine to save the "peeked" token.
		// if not - it means that next_token() got out of bounds.
		returnToken = getCurrentToken();
	}

	for (int i = 0; i < successfulNexts; i++)
	{
		back_token();
	}
	
	return returnToken;
}

const char* tokenToString(enum eTOKENS kind)
{
	switch (kind)
	{
		case TOKEN_ID:						return "TOKEN_ID";
		case TOKEN_KW_INTEGER:				return "TOKEN_KW_INTEGER";
		case TOKEN_KW_REAL:					return "TOKEN_KW_REAL";
		case TOKEN_KW_PROGRAM:				return "TOKEN_KW_PROGRAM";
		case TOKEN_KW_END:					return "TOKEN_KW_END";
		case TOKEN_KW_VOID:					return "TOKEN_KW_VOID";
		case TOKEN_KW_RETURN:				return "TOKEN_KW_RETURN";
		case TOKEN_END_OF_FILE:				return "TOKEN_END_OF_FILE";
		case TOKEN_COMMA:					return "TOKEN_COMMA";
		case TOKEN_SEMICOLON:				return "TOKEN_SEMICOLON";
		case TOKEN_OPEN_CURLY_BRACKETS:		return "TOKEN_OPEN_CURLY_BRACKETS";
		case TOKEN_CLOSE_CURLY_BRACKETS:	return "TOKEN_CLOSE_CURLY_BRACKETS";
		case TOKEN_OPEN_SQUARE_BRACKETS:	return "TOKEN_OPEN_SQUARE_BRACKETS";
		case TOKEN_CLOSE_SQUARE_BRACKETS:	return "TOKEN_CLOSE_SQUARE_BRACKETS";
		case TOKEN_OPEN_ROUND_BRACKETS:		return "TOKEN_OPEN_ROUND_BRACKETS";
		case TOKEN_CLOSE_ROUND_BRACKETS:	return "TOKEN_CLOSE_ROUND_BRACKETS";
		case TOKEN_ARITHMETIC_MULTIPLICATION: return "TOKEN_ARITHMETIC_MULTIPLICATION";
		case TOKEN_ARITHMETIC_DIVISION:		return "TOKEN_ARITHMETIC_DIVISION";
		case TOKEN_ARITHMETIC_ASSIGNMENT:	return "TOKEN_ARITHMETIC_ASSIGNMENT";
		case TOKEN_INT_NUMBER:				return "TOKEN_INT_NUMBER";
		case TOKEN_REAL_NUMBER:				return "TOKEN_REAL_NUMBER";
		default:							return "enum not valid.";
	}
}

eTOKENS handleValidToken(FILE* outputFile, eTOKENS kind, char* lexeme, int numOfLine)
{
	create_and_store_token(kind, lexeme, numOfLine);
	printValidTokenToOutputFile(outputFile, kind, lexeme, numOfLine);
	return currentNode->tokensArray[currentIndex].kind;
}

Token* getCurrentToken()
{
	Token* currentToken = &currentNode->tokensArray[currentIndex];
	return currentToken;
}

void freeMemory()
{
	Node *nodeToFree, *nextNode;
	nodeToFree = getFirstNode();
	
	while (nodeToFree != NULL)
	{
		//saving next node:
		nextNode = nodeToFree->next;

		for (int i = 0; i < TOKEN_ARRAY_SIZE; i++)
		{
			if (&nodeToFree->tokensArray[i] == NULL)
				break;

			//free lexeme:
			free(nodeToFree->tokensArray[i].lexeme);
		}

		//free array:
		free(nodeToFree->tokensArray);

		//free current node:
		free(nodeToFree);

		nodeToFree = nextNode;
	}

	//free the whole node:
	free(nodeToFree);

	//Initial global variables with default values:
	currentIndex = 0;
	currentNode = NULL;
	distanceFromLastIdentifiedToken = 0;
}

Node* getFirstNode() 
{
	while (currentNode->prev != NULL)
		currentNode = currentNode->prev;
	return currentNode;
}